#define SIMPLE_TEST(x) int main(){ x; return 0; }
#define CHECK_SIZE_OF_TYPE(x) int main(){ return sizeof(x); }
#define CHECK_FUNCTION_EXISTS(x) char x(); int main(){ x(); return 1; }


#ifdef TEST_TIME_IN_SYS_TIME

#  include <sys/time.h>
SIMPLE_TEST(struct tm *tp; tp->tm_sec);

#else /* TEST_TIME_IN_SYS_TIME */
#  ifdef TIME_IN_SYS_TIME
#    include <sys/time.h>
#  else /* TIME_IN_SYS_TIME */
#    include <time.h>
#  endif /* TIME_IN_SYS_TIME */
#endif /* TEST_TIME_IN_SYS_TIME */

#ifdef STDC_HEADERS

#include <ctype.h>
#define ISLOWER(c) ('a' <= (c) && (c) <= 'z')
#define TOUPPER(c) (ISLOWER(c) ? 'A' + ((c) - 'a') : (c))
#define XOR(e, f) (((e) && !(f)) || (!(e) && (f)))
int main () { int i; for (i = 0; i < 256; i++)
if (XOR (islower (i), ISLOWER (i)) || toupper (i) != TOUPPER (i)) exit(2);
exit (0); }

#endif /* STDC_HEADERS */

#ifdef HAVE_TM_ZONE

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(struct tm tm; tm.tm_zone);

#endif /* HAVE_TM_ZONE */

#ifdef HAVE_TZNAME

#include <time.h>
#ifndef tzname /* For SGI.  */
extern char *tzname[]; /* RS6000 and others reject char **tzname.  */
#endif

SIMPLE_TEST(atoi(*tzname));

#endif /* HAVE_TZNAME */

#ifdef HAVE_LARGE_HSIZET

#error "Not yet implemented"
SIMPLE_TEST(off64_t n = 0);

#endif /* HAVE_LARGE_HSIZET */

#ifdef PRINTF_LL_WIDTH


#define TO_STRING0(x) #x
#define TO_STRING(x) TO_STRING0(x)

#ifdef HAVE_LONG_LONG
#  define LL_TYPE long long
#else /* HAVE_LONG_LONG */
#  define LL_TYPE __int64
#endif /* HAVE_LONG_LONG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
	char *s = malloc(128);
	LL_TYPE x = (LL_TYPE)1048576 * (LL_TYPE)1048576;
	sprintf(s,"%" TO_STRING(PRINTF_LL_WIDTH) "d",x);
	exit(strcmp(s,"1099511627776"));
}

#endif /* PRINTF_LL_WIDTH */

#ifdef TEST_SEEK_64

#include <sys/types.h>
SIMPLE_TEST(off64_t n = 0);

#endif /* TEST_SEEK_64 */

#ifdef TIME_WITH_SYS_TIME

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(struct tm *tp);

#endif /* TIME_WITH_SYS_TIME */

#ifdef HAVE_TM_GMTOFF

#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(struct tm tm; tm.tm_gmtoff=0);

#endif /* HAVE_TM_GMTOFF */

#ifdef HAVE___TM_GMTOFF

#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(struct tm tm; tm.__tm_gmtoff=0);

#endif /* HAVE___TM_GMTOFF */

#ifdef HAVE_TIMEZONE

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(timezone=0);

#endif /* HAVE_TIMEZONE */

#ifdef HAVE_STRUCT_TIMEZONE

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
SIMPLE_TEST(struct timezone tz; tz.tz_minuteswest=0);

#endif /* HAVE_STRUCT_TIMEZONE */

#ifdef HAVE_STAT_ST_BLOCKS

#include <sys/stat.h>
SIMPLE_TEST(struct stat sb; sb.st_blocks=0);

#endif /* HAVE_STAT_ST_BLOCKS */

#ifdef HAVE_STRUCT_VIDEOCONFIG

SIMPLE_TEST(struct videoconfig w; w.numtextcols=0);

#endif /* HAVE_STRUCT_VIDEOCONFIG */

#ifdef HAVE_STRUCT_TEXT_INFO

SIMPLE_TEST(struct text_info w; w.screenwidth=0);

#endif /* HAVE_STRUCT_TEXT_INFO */

#ifdef HAVE_TIOCGWINSZ

#include <sys/ioctl.h>
SIMPLE_TEST(int w=TIOCGWINSZ);

#endif /* HAVE_TIOCGWINSZ */

#ifdef HAVE_TIOCGETD

#include <sys/ioctl.h>
SIMPLE_TEST(int w=TIOCGETD);

#endif /* HAVE_TIOCGETD */

#ifdef HAVE_ATTRIBUTE

SIMPLE_TEST(int __attribute__((unused)) x);

#endif /* HAVE_ATTRIBUTE */

#ifdef HAVE_FUNCTION

SIMPLE_TEST((void)__FUNCTION__);

#endif /* HAVE_FUNCTION */

#ifdef CHECK_SIZE_OF

#include <sys/types.h>
#include <stdint.h>
CHECK_SIZE_OF_TYPE(CHECK_SIZE_OF);

#endif /* CHECK_SIZE_OF */

#ifdef CHECK_FUNCTION

CHECK_FUNCTION_EXISTS(CHECK_FUNCTION);

#endif /* CHECK_FUNCTION */

/* Just for debugging  */
