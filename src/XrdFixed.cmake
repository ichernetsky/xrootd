
include( XRootDCommon )

#-------------------------------------------------------------------------------
# Shared library version
#-------------------------------------------------------------------------------
set( XRD_FIXED_VERSION   1.0.0 )
set( XRD_FIXED_SOVERSION 1 )

#-------------------------------------------------------------------------------
# The XrdClient lib
#-------------------------------------------------------------------------------
add_library(
  XrdFixed
  SHARED
  XrdFixed/XrdFixed.cc    XrdFixed/XrdFixed.hh )

target_link_libraries(
  XrdFixed )

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
