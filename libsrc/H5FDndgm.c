/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
/*
 *
 * Purpose:  A driver which stores the HDF5 data in NDGM using
 *    only the HDF5 public API. This driver is useful for distributed
 *    access to  hdf5 files.
 *    Derived from the "core" driver.
 */
#include "assert.h"
#include "hdf5.h"
#include "H5FDndgm.h"
#include "stdlib.h"
#include "Ndgm/ndgm.h"

/*
#define HDF_IO_DEBUG    1
*/

/*
#ifdef XDMF_NOT_USED
#undef XDMF_NOT_USED
#endif
*/

#ifndef FAIL
#define FAIL -1
#endif

#define HDF_NDGM_MARKER    "NDGM_ENTRY"
#define HDF_NDGM_ENTRY_LENGTH   80
#define HDF_NDGM_ENTRY_CHUNK    100

#if defined(IRIXN32)
#define HDF_NDGM_ENTRY_FORMAT  "%s %s %lld %lld"
#elif defined(IRIX64)
#define HDF_NDGM_ENTRY_FORMAT  "%s %s %ld %ld"
#elif defined(Linux)
#define HDF_NDGM_ENTRY_FORMAT  "%s %s %Ld %Ld"
#else
#define HDF_NDGM_ENTRY_FORMAT  "%s %s %Ld %Ld"
#endif

#undef MAX
#define MAX(X,Y)  ((X)>(Y)?(X):(Y))

#undef MIN
#define MIN(X,Y)  ((X)<(Y)?(X):(Y))

/* The driver identification number, initialized at runtime */
static hid_t H5FD_NDGM_g = 0;


/*
 * The description of a file belonging to this driver. The `eoa' and `eof'
 * determine the amount of hdf5 address space in use and the high-water mark
 * of the file (the current size of the underlying memory).
 */
typedef struct H5FD_ndgm_t {
    H5FD_t  pub;      /*public stuff, must be first  */
    char  *name;      /*for equivalence testing  */
    haddr_t  eoa;      /*end of allocated region  */
    haddr_t  eof;      /*current allocated size  */
    size_t  increment;    /*multiples for mem allocation  */
    NDGM_ADDR  entry_addr;    /* NDGM Address of this entry*/
    NDGM_ADDR  start;      /* Current NDGM Start Address */
    NDGM_ADDR  end;      /* Current NDGM End Address */
    char  entry_name[80];    /* Should be the same as name */
    char  entry[HDF_NDGM_ENTRY_LENGTH];
    int    dirty;
    NDGM_ADDR  buffer_begin_addr;
    NDGM_ADDR  buffer_next_addr;
    NDGM_LENGTH buffer_bytes_left;
    char  *data_buffer_ptr;
    char  data_buffer[NDGM_MAX_PACKET_SIZE];
} H5FD_ndgm_t;

/* Driver-specific file access properties */
typedef struct H5FD_ndgm_fapl_t {
    size_t  increment;    /*how much to grow memory  */
    char  *host;      /* Default Hostname ( if any ) */
} H5FD_ndgm_fapl_t;

/* Allocate memory in multiples of this size by default */
#define H5FD_NDGM_INCREMENT    1000000

/*
 * These macros check for overflow of various quantities.  These macros
 * assume that file_offset_t is signed and haddr_t and size_t are unsigned.
 * 
 * ADDR_OVERFLOW:  Checks whether a file address of type `haddr_t'
 *      is too large to be represented by the second argument
 *      of the file seek function.
 *
 * SIZE_OVERFLOW:  Checks whether a buffer size of type `hsize_t' is too
 *      large to be represented by the `size_t' type.
 *
 * REGION_OVERFLOW:  Checks whether an address and size pair describe data
 *      which can be addressed entirely in memory.
 */
#define MAXADDR     ((haddr_t)~(size_t)0 - 1)
#define ADDR_OVERFLOW(A)  (HADDR_UNDEF==(A) ||            \
         ((A) & ~(haddr_t)MAXADDR))
#define SIZE_OVERFLOW(Z)  ((Z) & ~(hsize_t)MAXADDR)
#define REGION_OVERFLOW(A,Z)  (ADDR_OVERFLOW(A) || SIZE_OVERFLOW(Z) ||      \
                                 HADDR_UNDEF==(A)+(Z) ||          \
         (size_t)((A)+(Z))<(size_t)(A))

/* Prototypes */
static void *H5FD_ndgm_fapl_get(H5FD_t *_file);
static H5FD_t *H5FD_ndgm_open(const char *name, unsigned flags, hid_t fapl_id,
            haddr_t maxaddr);
static herr_t H5FD_ndgm_close(H5FD_t *_file);
#ifdef XDMF_NOT_USED
static herr_t H5FD_ndgm_flush(H5FD_t *_file);
#endif
static int H5FD_ndgm_cmp(const H5FD_t *_f1, const H5FD_t *_f2);
static haddr_t H5FD_ndgm_get_eoa(H5FD_t *_file);
static herr_t H5FD_ndgm_set_eoa(H5FD_t *_file, haddr_t addr);
static haddr_t H5FD_ndgm_get_eof(H5FD_t *_file);
static herr_t H5FD_ndgm_read(H5FD_t *_file, H5FD_mem_t type, hid_t fapl_id, haddr_t addr,
           hsize_t size, void *buf);
static herr_t H5FD_ndgm_write(H5FD_t *_file, H5FD_mem_t type, hid_t fapl_id, haddr_t addr,
            hsize_t size, const void *buf);

static const H5FD_class_t H5FD_ndgm_g = {
    "ndgm",          /*name      */
    MAXADDR,          /*maxaddr    */
    NULL,          /*sb_size    */
    NULL,          /*sb_encode    */
    NULL,          /*sb_decode    */
    sizeof(H5FD_ndgm_fapl_t),      /*fapl_size    */
    H5FD_ndgm_fapl_get,        /*fapl_get    */
    NULL,          /*fapl_copy    */
    NULL,           /*fapl_free    */
    0,            /*dxpl_size    */
    NULL,          /*dxpl_copy    */
    NULL,          /*dxpl_free    */
    H5FD_ndgm_open,        /*open      */
    H5FD_ndgm_close,        /*close      */
    H5FD_ndgm_cmp,        /*cmp      */
    NULL,          /* query    */
    NULL,          /*alloc      */
    NULL,          /*free      */
    H5FD_ndgm_get_eoa,        /*get_eoa    */
    H5FD_ndgm_set_eoa,         /*set_eoa    */
    H5FD_ndgm_get_eof,        /*get_eof    */
    H5FD_ndgm_read,        /*read      */
    H5FD_ndgm_write,        /*write      */
    NULL,          /*flush      */
    H5FD_FLMAP_SINGLE,        /*fl_map    */
};



/****  NDGM Entries ********/
/**************************************
NDGM_ENTRY  Name  Start  End
**************************************/


#define WORD_CMP(a, b)    ((b) == NULL ? 1 : strncmp((a), (b), strlen(b)))

int
H5FD_ndgm_UpdateEntry( H5FD_ndgm_t *file ) {

int status;

file->end = MAX(((NDGM_ADDR)(file->start + file->eof)), file->end);
file->eof = file->end - file->start;

sprintf(file->entry, HDF_NDGM_ENTRY_FORMAT,
                        HDF_NDGM_MARKER,
                        file->name,
                        file->start,
                        file->end);
#ifdef HDF_IO_DEBUG
printf("HDF::H5FD_ndgm_UpdateEntry Extending Entry <%s> at %ld\n", file->entry, file->entry_addr);
#endif
status = ndgm_put(file->entry_addr, file->entry, HDF_NDGM_ENTRY_LENGTH);
if( status <= 0 ) return -1;
return( status );
}


char  *
H5FD_next_ndgm_entry(const char *last_entry)
{
char      *entry;
NDGM_DEFAULT_INT  status;
NDGM_LENGTH    chunk_size;
static char    entry_list[HDF_NDGM_ENTRY_CHUNK][HDF_NDGM_ENTRY_LENGTH];
static char    *elp;
static NDGM_ADDR  last, addr;

if(last_entry == NULL) {
  /* Cold Start */
  chunk_size = (sizeof(char) * 
      HDF_NDGM_ENTRY_LENGTH * HDF_NDGM_ENTRY_CHUNK);
  last = NDGM_LAST_ADDRESS();
  addr = last - chunk_size;
  status = ndgm_get(addr, entry_list, chunk_size);
  if(status <= 0){
    fprintf(stderr, "H5FD_next_ndgm_entry : ndgm_get() failed at %ld\n", (long)addr);
    return(NULL);
    }
  elp = entry_list[0];
  entry = entry_list[HDF_NDGM_ENTRY_CHUNK - 1];
} else {
  entry = (char *)(last_entry - HDF_NDGM_ENTRY_LENGTH);
  if(entry < elp){
    /* Get another chunk */
    addr -= chunk_size;
    status = ndgm_get(addr, entry_list, chunk_size);
    if(status <= 0){
      fprintf(stderr,"H5FD_next_ndgm_entry : ndgm_get() failed at %ld\n", (long)addr);
      return(NULL);
      }
    entry = entry_list[HDF_NDGM_ENTRY_CHUNK - 1];
  }
}

return(entry);
}

NDGM_ADDR
H5FD_find_ndgm_entry(const char *entry_name, H5FD_ndgm_t *nfp)
{
char    *entry = NULL;
char    name[HDF_NDGM_ENTRY_LENGTH], marker[HDF_NDGM_ENTRY_LENGTH];
NDGM_ADDR  entry_addr;

#ifdef HDF_IO_DEBUG
printf("Looking for %s\n", entry_name);
#endif
entry_addr = NDGM_LAST_ADDRESS();
do { 
#ifdef HDF_IO_DEBUG
printf("Look at Entry at %ld\n", entry_addr );
#endif
  entry = H5FD_next_ndgm_entry(entry);
  entry_addr -= HDF_NDGM_ENTRY_LENGTH;
  if(entry != NULL){
#ifdef HDF_IO_DEBUG
printf("Entry at %ld is %s\n", entry_addr, entry);
#endif
    if(WORD_CMP(entry, HDF_NDGM_MARKER) == 0){
      /* Valid Entry */
      sscanf(entry, "%s %s", marker, name);
      if(WORD_CMP(name, entry_name) == 0){
        /* This is it */
        if(nfp != NULL){
          nfp->entry_addr = entry_addr;
          strcpy(nfp->entry, entry);
/*
          sscanf(entry, "%s %s %ld %ld",
*/
          sscanf(entry, HDF_NDGM_ENTRY_FORMAT,
            marker,
            nfp->entry_name,
            &nfp->start,
            &nfp->end);
#ifdef HDF_IO_DEBUG
printf("Found it <%s> at %ld\n", entry, nfp->entry_addr);
printf("Start = %d End = %d\n", nfp->start, nfp->end );
printf("With HDF_NDGM_ENTRY_FORMAT = %s\n", HDF_NDGM_ENTRY_FORMAT );
// exit(1);
#endif
          return(entry_addr);
        }
      }
    } else {
      /* break; */
      entry = NULL;
    }
  }
} while(entry != NULL);
/*
printf("Entry not found\n");
*/
return(FAIL);
}

NDGM_DEFAULT_INT
H5FD_add_ndgm_entry(const char *entry_name, H5FD_ndgm_t *nfp)
{
char    *entry = NULL;
char    name[HDF_NDGM_ENTRY_LENGTH], marker[HDF_NDGM_ENTRY_LENGTH];
NDGM_DEFAULT_INT status;
NDGM_ADDR  entry_addr, start = 0, end = 0;
NDGM_ADDR  new_start = 0, new_end = 0;
H5FD_ndgm_t  fake_fp;

if( nfp == NULL ){
  nfp = &fake_fp;  
  }
entry_addr = NDGM_LAST_ADDRESS();
do { 
  entry = H5FD_next_ndgm_entry(entry);
  entry_addr -= HDF_NDGM_ENTRY_LENGTH;
  if(entry != NULL){
    if(WORD_CMP(entry, HDF_NDGM_MARKER) == 0){
      /* Valid Entry */
      sscanf(entry, HDF_NDGM_ENTRY_FORMAT,
        marker, name, &start, &end);
      new_end = MAX(new_end, end);
      new_start = MAX(new_start, start);
    } else {
      break;
    }
  }
} while(entry != NULL);

new_start = new_end;
if(new_start > 0){
  /* Align on a Double, After last end */
  new_start = ((new_start / sizeof(double)) + 2) * sizeof(double);
  new_end = new_start;
}
nfp->start = new_start;
nfp->end = new_end;
nfp->entry_addr = entry_addr;

sprintf(nfp->entry, HDF_NDGM_ENTRY_FORMAT,
    HDF_NDGM_MARKER,
    entry_name,
    new_start,
    new_end);  
#ifdef HDF_IO_DEBUG
printf("Adding Entry <%s> at %ld\n", nfp->entry, entry_addr );
#endif
status = ndgm_put(entry_addr, nfp->entry, HDF_NDGM_ENTRY_LENGTH);
return(status);
}

/* Make Entries Available Scripting */
/* Only HDF5 Files are put in NDGM */
NDGM_ADDR XdmfAddNdgmEntry( char *Name, NDGM_ADDR Length ){

H5FD_ndgm_t    fp;
NDGM_DEFAULT_INT  status;

status =  H5FD_add_ndgm_entry( Name, &fp);
if( status ) {
  sprintf(fp.entry, HDF_NDGM_ENTRY_FORMAT,
                        HDF_NDGM_MARKER,
                        Name,
                        fp.start,
                        fp.start + Length);
  status = ndgm_put(fp.entry_addr, fp.entry, HDF_NDGM_ENTRY_LENGTH);
  return( fp.start );
}
return(FAIL);
}

/* All or Nothing ... don't leave holes */
void XdmfDeleteAllNdgmEntries( void ) {

char    entry_list[HDF_NDGM_ENTRY_CHUNK][HDF_NDGM_ENTRY_LENGTH];

/* Cheat and just delete the last entry list */
memset( entry_list, 0, HDF_NDGM_ENTRY_CHUNK * HDF_NDGM_ENTRY_LENGTH );
ndgm_put( NDGM_LAST_ADDRESS() - ( HDF_NDGM_ENTRY_CHUNK * HDF_NDGM_ENTRY_LENGTH ),
     entry_list, HDF_NDGM_ENTRY_CHUNK * HDF_NDGM_ENTRY_LENGTH );
}

char *XdmfGetNdgmEntries( void ){
  char  *ReturnList = calloc( 1, 1 );
  char  *LastEntry;
  int  i, StingLength, ListSize = 0;

  LastEntry = H5FD_next_ndgm_entry( NULL );
  while( LastEntry != NULL ){
    if(WORD_CMP(LastEntry, HDF_NDGM_MARKER) != 0){
      break;
    }
    StingLength = strlen( LastEntry );
    ReturnList = realloc( ReturnList, ListSize + StingLength + 2 );
    for( i = 0 ; i < StingLength ; i++ ){
      ReturnList[ ListSize + i ] = LastEntry[ i ];
      }
    ReturnList[ ListSize + StingLength ] = ' ';
    ReturnList[ ListSize + StingLength  + 1 ] = '\0';
    ListSize += StingLength + 1;
    LastEntry = H5FD_next_ndgm_entry( LastEntry );
    }
  return( ReturnList );
  }

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_init
 *
 * Purpose:  Initialize this driver by registering the driver with the
 *    library.
 *
 * Return:  Success:  The driver ID for the ndgm driver.
 *
 *    Failure:  Negative.
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
hid_t
H5FD_ndgm_init(void)
{
    if (H5I_VFL!=H5Iget_type(H5FD_NDGM_g)) {
  H5FD_NDGM_g = H5FDregister(&H5FD_ndgm_g);
    }
    return H5FD_NDGM_g;
}

/*-------------------------------------------------------------------------
 * Function:  H5Pset_fapl_ndgm
 *
 * Purpose:  Modify the file access property list to use the H5FD_NDGM
 *    driver defined in this source file.  The INCREMENT specifies
 *    how much to grow the memory each time we need more.
 *    
 * Return:  Non-negative on success/Negative on failure
 *
 * Programmer:  Jerry Clarke
 *    Thursday, February 19, 1998
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Pset_fapl_ndgm(hid_t fapl_id, size_t increment, const char *host )
{
    H5FD_ndgm_fapl_t  fa;
    
#ifdef HDF_IO_DEBUG
printf("Setting fapl %d %s\n", increment, host );
#endif
    /*NO TRACE*/
    if (H5P_FILE_ACCESS!=H5Pget_class(fapl_id)) return -1;
    fa.increment = increment;
    fa.host = NULL;
    if( host != NULL ){
    fa.host = strdup( host );
    }
    return H5Pset_driver(fapl_id, H5FD_NDGM, &fa);
}

/*-------------------------------------------------------------------------
 * Function:  H5Pget_fapl_ndgm
 *
 * Purpose:  Queries properties set by the H5Pset_fapl_ndgm() function.
 *
 * Return:  Success:  Non-negative
 *
 *    Failure:  Negative
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, August 10, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Pget_fapl_ndgm(hid_t fapl_id, size_t *increment/*out*/, char **host /* out */ )
{
    H5FD_ndgm_fapl_t  *fa;

#ifdef HDF_IO_DEBUG
printf("Getting fapl\n");
#endif
    /*NO TRACE*/
    if (H5P_FILE_ACCESS!=H5Pget_class(fapl_id)) return -1;
    if (H5FD_NDGM!=H5Pget_driver(fapl_id)) return -1;
    if (NULL==(fa=H5Pget_driver_info(fapl_id))) return -1;
    if (increment) *increment = fa->increment;
    if( host ) *host = fa->host;
    return 0;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_fapl_get
 *
 * Purpose:  Returns a copy of the file access properties.
 *
 * Return:  Success:  Ptr to new file access properties.
 *
 *    Failure:  NULL
 *
 * Programmer:  Jerry Clarke
 *              Friday, August 13, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static void *
H5FD_ndgm_fapl_get(H5FD_t *_file)
{
    H5FD_ndgm_t    *file = (H5FD_ndgm_t*)_file;
    H5FD_ndgm_fapl_t  *fa = calloc(1, sizeof(H5FD_ndgm_fapl_t));

#ifdef HDF_IO_DEBUG
printf("Fapl Get\n");
#endif
    fa->increment = file->increment;
    return fa;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_open
 *
 * Purpose:  Create memory as an HDF5 file.
 *
 * Return:  Success:  A pointer to a new file data structure. The
 *        public fields will be initialized by the
 *        caller, which is always H5FD_open().
 *
 *    Failure:  NULL
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static H5FD_t *
H5FD_ndgm_open(const char *name, unsigned flags/*unused*/, hid_t fapl_id,
         haddr_t maxaddr)
{
    H5FD_ndgm_t    *file=NULL;
    H5FD_ndgm_fapl_t  *fa=NULL;
    NDGM_ADDR    addr;
    
#ifdef HDF_IO_DEBUG
printf("Opening %s ... Checking Args\n", name);
#endif
    /* Check arguments */
    if (0==maxaddr || HADDR_UNDEF==maxaddr) return NULL;
#ifdef HDF_IO_DEBUG
printf("Opening %s ... Checking maxaddr \n", name);
#endif
    if (ADDR_OVERFLOW(maxaddr)) return NULL;
#ifdef HDF_IO_DEBUG
printf("Opening %s .... Setting Driver\n", name);
#endif
    if (H5P_DEFAULT!=fapl_id) fa = H5Pget_driver_info(fapl_id);

#ifdef HDF_IO_DEBUG
printf("Opening %s\n", name);
#endif
  /* Initialize NDGM */
  if(!ndgm_inited){
    if( fa && fa->host ) {
      int H5FD_ndgm_argc = 3;
      char *H5FD_ndgm_argv[4];

      H5FD_ndgm_argv[0] = "Hdf5";
      H5FD_ndgm_argv[1] = "-host";
      H5FD_ndgm_argv[2] = fa->host;
      H5FD_ndgm_argv[3] = NULL;
      if(ndgm_init(&H5FD_ndgm_argc, H5FD_ndgm_argv ) == NDGM_FAIL){
        return( NULL );
      }
    } else {
      if(ndgm_init(NULL, NULL) == NDGM_FAIL){
        return( NULL );
      }
    }
  }

    /* Create the new file struct */
    file = calloc(1, sizeof(H5FD_ndgm_t));
    if (name && *name) {
  file->name = malloc(strlen(name)+1);
  strcpy(file->name, name);
  strcpy(file->entry_name, name);
    }

    /* See if it exists */
  addr = H5FD_find_ndgm_entry(name, file);
#ifdef HDF_IO_DEBUG
if( addr != FAIL ){
  printf("HDF::Open Start %ld End %ld\n", file->start, file->end );
}
#endif
  if(H5F_ACC_CREAT & flags){
    if (addr == FAIL){
#ifdef HDF_IO_DEBUG
printf("HDF::Creating\n");
#endif
      H5FD_add_ndgm_entry(name, file);
      addr = file->entry_addr;
    }
    file->eof = file->end - file->start;
  } else {
    /* Must Already Exist */
    if(addr == FAIL){
      free( file );
      return( NULL );
    }
    if(H5F_ACC_RDWR & flags) {
      /* Read and Write */
      file->eof = file->end - file->start;
    } else {
      /* Read Only */
      file->eof = file->end - file->start;
    }
  }

    /*
     * The increment comes from either the file access property list or the
     * default value. But if the file access property list was zero then use
     * the default value instead.
     */
    file->increment = (fa && fa->increment>0) ?
          fa->increment : H5FD_NDGM_INCREMENT;

#ifdef HDF_IO_DEBUG
printf("Open of %s was successful, entry at %ld\n", name, file->entry_addr);
printf("File Start %ld End %ld\n", file->start, file->end );
printf("HDF::Open eoa = %ld eof = %ld\n", file->eoa, file->eof);
#endif
  file->buffer_begin_addr = file->buffer_next_addr = 0;
  file->buffer_bytes_left = NDGM_MAX_PACKET_SIZE;
  file->data_buffer_ptr = file->data_buffer;
  file->dirty = 0;
    return (H5FD_t*)file;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_flush
 *
 * Purpose:  Flush the file.
 *
 * Return:  Success:  0
 *
 *    Failure:  -1
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
#ifdef XDMF_NOT_USED
static herr_t
H5FD_ndgm_flush(H5FD_t *_file)
{
    H5FD_ndgm_t  *file = (H5FD_ndgm_t*)_file;
    NDGM_LENGTH size;
    int status;

  size = file->buffer_next_addr - file->buffer_begin_addr;

#ifdef HDF_IO_DEBUG
  printf("Flushing Start = %ld Length = %ld\n",
      file->buffer_begin_addr, size );
#endif

  if( file->dirty && ( size > 0 ) ){
#ifdef HDF_IO_DEBUG
printf("....Really Flushing\n");
#endif
    status = ndgm_put(file->buffer_begin_addr, file->data_buffer, size);
#ifdef HDF_IO_DEBUG
printf("Status = %d\n", status );
#endif
    }
  file->buffer_begin_addr = file->buffer_next_addr = 0;
  file->buffer_bytes_left = NDGM_MAX_PACKET_SIZE;
  file->data_buffer_ptr = file->data_buffer;
  file->dirty = 0;
return( 0 );
}
#endif
/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_close
 *
 * Purpose:  Closes the file.
 *
 * Return:  Success:  0
 *
 *    Failure:  -1
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5FD_ndgm_close(H5FD_t *_file)
{
    H5FD_ndgm_t  *file = (H5FD_ndgm_t*)_file;
    int status;

#ifdef HDF_IO_DEBUG
  printf("Closing Entry <%s> at %ld, %ld - %ld \n",
    file->entry, file->entry_addr,
    file->start, file->end);
#endif
        status = H5FD_ndgm_UpdateEntry( file );
  if( status <= 0 ) return -1;
    if (file->name) free(file->name);
    memset(file, 0, sizeof(H5FD_ndgm_t));
    free(file);
    return 0;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_cmp
 *
 * Purpose:  Compares two files belonging to this driver by name. If one
 *    file doesn't have a name then it is less than the other file.
 *    If neither file has a name then the comparison is by file
 *    address.
 *
 * Return:  Success:  A value like strcmp()
 *
 *    Failure:  never fails (arguments were checked by the
 *        caller).
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static int
H5FD_ndgm_cmp(const H5FD_t *_f1, const H5FD_t *_f2)
{
    const H5FD_ndgm_t  *f1 = (const H5FD_ndgm_t*)_f1;
    const H5FD_ndgm_t  *f2 = (const H5FD_ndgm_t*)_f2;

    if (NULL==f1->name && NULL==f2->name) {
  if (f1<f2) return -1;
  if (f1>f2) return 1;
  return 0;
    }
    
    if (NULL==f1->name) return -1;
    if (NULL==f2->name) return 1;

    return strcmp(f1->name, f2->name);
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_get_eoa
 *
 * Purpose:  Gets the end-of-address marker for the file. The EOA marker
 *    is the first address past the last byte allocated in the
 *    format address space.
 *
 * Return:  Success:  The end-of-address marker.
 *
 *    Failure:  HADDR_UNDEF
 *
 * Programmer:  Jerry Clarke
 *              Monday, August  2, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static haddr_t
H5FD_ndgm_get_eoa(H5FD_t *_file)
{
    H5FD_ndgm_t  *file = (H5FD_ndgm_t*)_file;

#ifdef HDF_IO_DEBUG
printf("H5FD_ndgm_get_eoa Called\n");
#endif
    return file->eoa;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_set_eoa
 *
 * Purpose:  Set the end-of-address marker for the file. This function is
 *    called shortly after an existing HDF5 file is opened in order
 *    to tell the driver where the end of the HDF5 data is located.
 *
 * Return:  Success:  0
 *
 *    Failure:  -1
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5FD_ndgm_set_eoa(H5FD_t *_file, haddr_t addr)
{
    H5FD_ndgm_t  *file = (H5FD_ndgm_t*)_file;


#ifdef HDF_IO_DEBUG
printf("H5FD_ndgm_set_eoa Called %ld \n", addr);
#endif
    if (ADDR_OVERFLOW(addr)) return -1;
    file->eof = file->eoa = addr;
  H5FD_ndgm_UpdateEntry( file );
    return 0;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_get_eof
 *
 * Purpose:  Returns the end-of-file marker, which is the greater of
 *    either the size of the underlying memory or the HDF5
 *    end-of-address markers.
 *
 * Return:  Success:  End of file address, the first address past
 *        the end of the "file", either the memory
 *        or the HDF5 file.
 *
 *    Failure:  HADDR_UNDEF
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static haddr_t
H5FD_ndgm_get_eof(H5FD_t *_file)
{
    H5FD_ndgm_t  *file = (H5FD_ndgm_t*)_file;

#ifdef HDF_IO_DEBUG
printf("H5FD_ndgm_get_eoa Called %ld \n", MAX(file->eof, file->eoa) );
#endif
    return MAX(file->eof, file->eoa);
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_read
 *
 * Purpose:  Reads SIZE bytes of data from FILE beginning at address ADDR
 *    into buffer BUF according to data transfer properties in
 *    DXPL_ID.
 *
 * Return:  Success:  Zero. Result is stored in caller-supplied
 *        buffer BUF.
 *
 *    Failure:  -1, Contents of buffer BUF are undefined.
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5FD_ndgm_read(H5FD_t *_file, H5FD_mem_t type, hid_t dxpl_id/*unused*/, haddr_t addr,
         hsize_t size, void *buf/*out*/)
{
    H5FD_ndgm_t    *file = (H5FD_ndgm_t*)_file;
    ssize_t    nbytes;
    herr_t    status;

(void)type;
(void)dxpl_id;
    
#ifdef HDF_IO_DEBUG
printf("Called H5FD_ndgm_read at %ld size %ld\n", addr, size);
#endif
    assert(file && file->pub.cls);
    assert(buf);

    /* Check for overflow conditions */
    if (HADDR_UNDEF==addr) return -1;
    if (REGION_OVERFLOW(addr, size)) return -1;
    if (addr+size>file->eoa) return -1;

    /* Read the part which is before the EOF marker */
#ifdef HDF_IO_DEBUG
/*
printf("check addr ( %ld )  < file->eof ( %ld )\n", addr, file->eof);
*/
#endif
    if (addr<file->eof) {
  nbytes = MIN(size, file->eof-addr);
  status = ndgm_get(file->start + addr, buf, nbytes);
  if( status <= 0 ) return -1;
  size -= nbytes;
  addr += nbytes;
  buf = (char*)buf + nbytes;
    }

    /* Read zeros for the part which is after the EOF markers */
    if (size>0) {
  memset(buf, 0, size);
    }
    return 0;
}

/*-------------------------------------------------------------------------
 * Function:  H5FD_ndgm_write
 *
 * Purpose:  Writes SIZE bytes of data to FILE beginning at address ADDR
 *    from buffer BUF according to data transfer properties in
 *    DXPL_ID.
 *
 * Return:  Success:  Zero
 *
 *    Failure:  -1
 *
 * Programmer:  Jerry Clarke
 *              Tuesday, Sept. 28, 1999
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5FD_ndgm_write(H5FD_t *_file, H5FD_mem_t type, hid_t dxpl_id/*unused*/, haddr_t addr,
    hsize_t size, const void *buf)
{
    H5FD_ndgm_t    *file = (H5FD_ndgm_t*)_file;
    herr_t    status;

    const char    *bufp = buf;
    
    (void)type;
    (void)dxpl_id;
    
    assert(file && file->pub.cls);
    assert(buf);

#ifdef HDF_IO_DEBUG
printf("HDF::Write Start %ld End %ld addr %ld size %ld eoa %ld eof %ld\n",
    file->start, file->end, addr, size, file->eoa, file->eof );
#endif
    /* Check for overflow conditions */
    if (REGION_OVERFLOW(addr, size)) return -1;
    if (addr+size>file->eoa) return -1;

    if (addr+size>file->eof) {
  size_t new_eof = file->increment * ((addr+size)/file->increment);
  if ((addr+size) % file->increment) new_eof += file->increment;
#ifdef HDF_IO_DEBUG
printf("HDF::Write New eof %ld\n", new_eof);
#endif
  /* Blindly Grab more NDGM for now */
  file->end = file->start + new_eof;
  file->eof = new_eof;
  /* Write it out to NDGM */
  status = H5FD_ndgm_UpdateEntry( file );
  if( status <= 0 ) return -1;

    }

    /* Write from BUF to NDGM */
  status = ndgm_put(file->start + addr, (char*)buf, size);
  if( status <= 0 ) return -1;
  /* Flush old buffer if non-contiguous */

/*** As of Hdf 1.3 Buffering seems unnecessary *****/
/*
  if( file->buffer_next_addr != (file->start + addr) ){
#ifdef HDF_IO_DEBUG
printf("0....Calling Flush due to non-contiguous Data\n");
printf("buffer_next_addr %d != %d + %d\n", file->buffer_next_addr, file->start, addr );
#endif
    H5FD_ndgm_flush( _file );
    file->buffer_begin_addr = file->start + addr;
    file->buffer_next_addr = file->start + addr;
    }
  file->dirty = 1;
  while( size > 0 ){
  if( file->buffer_bytes_left <= size  ) {
    memcpy( file->data_buffer_ptr, bufp, file->buffer_bytes_left );
    file->data_buffer_ptr += file->buffer_bytes_left;
    file->buffer_next_addr += file->buffer_bytes_left;
    bufp += file->buffer_bytes_left;
    file->buffer_begin_addr += file->buffer_bytes_left;;
    size -= file->buffer_bytes_left;
#ifdef HDF_IO_DEBUG
printf("1....Calling Flush due to non-contiguous Data\n");
#endif
    H5FD_ndgm_flush( _file );
  } else {
    memcpy( file->data_buffer_ptr, bufp, size );
    file->data_buffer_ptr += size;
    file->buffer_next_addr += size;
    size = 0;
    }
  
  }
*/
   return 0;
}
