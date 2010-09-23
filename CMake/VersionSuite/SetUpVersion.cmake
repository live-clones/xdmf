# Version Suite
# Author: Brian Panneton
# Descrition: 	This small suite allows you to add support
#		for versioning in your projects

# This allows you to turn on and off the auto 
# update of the (project name)Version.hpp file
SET(VERSION_CONTROL_AUTOUPDATE ON CACHE BOOL "Automaticaly Update The Version")
MARK_AS_ADVANCED(VERSION_CONTROL_AUTOUPDATE)

# We need to make sure we have the header file
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/CMake/VersionSuite)

# Default incase CalculateVerison is not called
SET(vMajor "0")
SET(vMinor "0")

# This Macro allows you to set the rewrite number
MACRO(VersionMajorSet versionMajor)
	SET(vMajor ${versionMajor})
ENDMACRO(VersionMajorSet)

# This Macro calculates the number of tags from your git repo
MACRO(VersionCalculate)
	EXEC_PROGRAM(git ${CMAKE_SOURCE_DIR} ARGS tag OUTPUT_VARIABLE return)
	STRING(REGEX REPLACE "\n" ";" return "${return}")
	SET(count 0)
	FOREACH(r ${return})
		MATH(EXPR count "${count} + 1")
	ENDFOREACH(r ${return})
	SET(vMinor ${count})
ENDMACRO(VersionCalculate)

# This Macro writes your hpp file
MACRO(VersionWrite versionProjectName)
	FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${versionProjectName}Version.hpp 
"/*Current Version of ${versionProjectName}*/
\#define VersionMajor ${vMajor}
\#define VersionMinor ${vMinor}
\#include \"Version.hpp\"\n"
	)
ENDMACRO(VersionWrite)
