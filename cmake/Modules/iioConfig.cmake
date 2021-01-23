include(FindPkgConfig)
PKG_CHECK_MODULES(PC_IIO iio)

find_path(
    IIO_INCLUDE_DIRS
    NAMES iio/api.h
    HINTS $ENV{IIO_DIR}/include
        ${PC_IIO_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

find_library(
    IIO_LIBRARIES
    NAMES gnuradio-iio
    HINTS $ENV{IIO_DIR}/lib
        ${PC_IIO_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(IIO DEFAULT_MSG IIO_LIBRARIES IIO_INCLUDE_DIRS)

mark_as_advanced(
    IIO_INCLUDE_DIRS
    IIO_LIBRARIES
)
