# Dashboard is opened for submissions for a 24 hour period starting at
# the specified NIGHLY_START_TIME. Time is specified in 24 hour format.
SET (NIGHTLY_START_TIME "0:30:00 EDT")

# Dart server to submit results (used by client)
IF(DROP_METHOD MATCHES http)
  SET (DROP_SITE "public.kitware.com")
  SET (DROP_LOCATION "/cgi-bin/HTTPUploadDartFile.cgi")
ELSE(DROP_METHOD MATCHES http)
  IF(DROP_METHOD MATCHES scp)
    SET (DROP_SITE "public.kitware.com")
    SET (DROP_LOCATION "/ftp/incoming/Xdmf")
    SET (DROP_SITE_USER "www")
  ELSE(DROP_METHOD MATCHES scp)
    SET (DROP_SITE "public.kitware.com")
    SET (DROP_LOCATION "/incoming")
    SET (DROP_SITE_USER "ftpuser")
    SET (DROP_SITE_PASSWORD "public")
  ENDIF(DROP_METHOD MATCHES scp)
ENDIF(DROP_METHOD MATCHES http)
SET (DROP_METHOD "ftp" CACHE STRING "Select drop method for test submission")
SET (TRIGGER_SITE 
       "http://${DROP_SITE}/cgi-bin/Submit-Xdmf-TestingResults.pl")

# Project Home Page
SET (PROJECT_URL "http://public.kitware.com")

# Dart server configuration 
SET (ROLLUP_URL "http://${DROP_SITE}/cgi-bin/xdmf-rollup-dashboard.sh")
SET (CVS_WEB_URL "http://${DROP_SITE}/cgi-bin/viewcvs.cgi/")
SET (CVS_WEB_CVSROOT "Xdmf")
SET (USE_DOXYGEN "On")
SET (DOXYGEN_URL "http://www.arl.hpc.mil/ice/Manuals/Xdmf/index.html" )
SET (GNATS_WEB_URL "${PROJECT_URL}/Bug/index.php")
SET (USE_GNATS "On")

# copy over the testing logo
CONFIGURE_FILE(${Xdmf_SOURCE_DIR}/DiceLogo.gif ${Xdmf_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)

# Continuous email delivery variables
SET (CONTINUOUS_FROM "xdmf-dashboard@public.kitware.com")
SET (SMTP_MAILHOST "public.kitware.com")
SET (CONTINUOUS_MONITOR_LIST "xdmf-dashboard@public.kitware.com")
SET (CONTINUOUS_BASE_URL "${PROJECT_URL}/Testing")

SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_FAILURES ON)
SET (DELIVER_BROKEN_BUILD_EMAIL "Continuous Nightly")
SET (EMAIL_FROM "xdmf-dashboard@public.kitware.com")
SET (DARTBOARD_BASE_URL "${PROJECT_URL}/Testing")

SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_CONFIGURE_FAILURES 1)
SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_BUILD_ERRORS 1)
SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_BUILD_WARNINGS 1)
SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_NOT_RUNS 1)
SET (DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_FAILURES 1)

