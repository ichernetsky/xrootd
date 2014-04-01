
include( XRootDCommon )

#-------------------------------------------------------------------------------
# Shared library version
#-------------------------------------------------------------------------------
set( XRD_FIXED_VERSION   1.0.0 )
set( XRD_FIXED_SOVERSION 1 )

#-------------------------------------------------------------------------------
# The XrdClient lib
#-------------------------------------------------------------------------------
include_directories( ${OPENSSL_INCLUDE_DIR} )

add_library(
  XrdFixed
  SHARED
  XrdFixed/XrdFixedDefines.hh
  XrdFixed/XrdFixed.cc           XrdFixed/XrdFixed.hh 
  XrdFixed/XrdFixedRedirector.cc XrdFixed/XrdFixedRedirector.hh )

target_link_libraries(
  XrdFixed 
  ${OPENSSL_LIBRARIES} )

set_target_properties(
  XrdFixed
  PROPERTIES
  VERSION   ${XRD_FIXED_VERSION}
  SOVERSION ${XRD_FIXED_SOVERSION}
  INTERFACE_LINK_LIBRARIES ""
  LINK_INTERFACE_LIBRARIES "" )

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------
install(
  TARGETS XrdFixed
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )