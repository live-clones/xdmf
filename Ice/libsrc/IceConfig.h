/* IceConfig.h.  Generated automatically by configure.  */
/********************************************************/
/*							*/
/* 	ICE Host Dependent Configuration File		*/
/*							*/
/********************************************************/

#ifdef _WIN32
#  include "windows.h"
#endif

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef ICE_WORDS_BIGENDIAN */

/* The number of bytes in a double.  */
#define ICE_SIZEOF_DOUBLE 8

/* The number of bytes in a float.  */
#define ICE_SIZEOF_FLOAT 4

/* The number of bytes in a int.  */
#define ICE_SIZEOF_INT 4

/* The number of bytes in a unsigned long long.  */
#define ICE_SIZEOF_UNSIGNED_LONG_LONG 8

/* Define if you have the GL library (-lGL).  */
#define ICE_HAVE_LIBGL 1

/* Define if you have the X11 library (-lX11).  */
/* #undef ICE_HAVE_LIBX11 */

/* Define if you have the Xmu library (-lXmu).  */
/* #undef ICE_HAVE_LIBXMU */

/* Define if you have the curses library (-lcurses).  */
#define ICE_HAVE_LIBCURSES 1

/* Define if you have the dl library (-ldl).  */
#define ICE_HAVE_LIBDL 1

/* Define if you have the m library (-lm).  */
#define ICE_HAVE_LIBM 1

/* Define if you have the mpi library (-lmpi).  */
#define ICE_HAVE_LIBMPI 1

/* Define if you have the sun library (-lsun).  */
/* #undef ICE_HAVE_LIBSUN */

#define ICE_SYSTEM "Linux-2.4.9-34smp"
#define ICE_HOST Linux
#define ICE_BYTE_ORDER_LITTLE

#define ICE_HAVE_FCNTL

#ifndef _WIN32
#  define ICE_HAVE_NETINET
#  define ICE_HAVE_MMAN
#  define ICE_have_64_bit_int
#endif

/* Size of Atomic Datum */
typedef		void 	ICE_VOID;
typedef		void *	ICE_PTR;
typedef		char 	ICE_CHAR;
typedef 	unsigned char ICE_8_INT;
typedef 	int ICE_32_INT;
#ifdef _WIN32
typedef 	unsigned long ICE_64_INT;
#else
typedef 	unsigned long long ICE_64_INT;
#endif
typedef 	float ICE_FLOAT;
typedef 	double ICE_DOUBLE;


/********************************************************/
