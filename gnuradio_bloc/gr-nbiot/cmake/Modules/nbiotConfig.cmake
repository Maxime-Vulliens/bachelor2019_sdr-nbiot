INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_NBIOT nbiot)

FIND_PATH(
    NBIOT_INCLUDE_DIRS
    NAMES nbiot/api.h
    HINTS $ENV{NBIOT_DIR}/include
        ${PC_NBIOT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    NBIOT_LIBRARIES
    NAMES gnuradio-nbiot
    HINTS $ENV{NBIOT_DIR}/lib
        ${PC_NBIOT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NBIOT DEFAULT_MSG NBIOT_LIBRARIES NBIOT_INCLUDE_DIRS)
MARK_AS_ADVANCED(NBIOT_LIBRARIES NBIOT_INCLUDE_DIRS)

