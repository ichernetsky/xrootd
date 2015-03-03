
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

set_target_properties(
  XrdFixed
  PROPERTIES
  VERSION   ${XRD_FIXED_VERSION}
  SOVERSION ${XRD_FIXED_SOVERSION}
  INTERFACE_LINK_LIBRARIES ""
  LINK_INTERFACE_LIBRARIES "" )

target_link_libraries(
  XrdFixed
  XrdCl
  ${OPENSSL_LIBRARIES} )

add_library(
  XrdFixedDataNode
  SHARED
  XrdFixed/XrdFixedDefines.hh
  XrdFixed/XrdFixedReplicator.cc XrdFixed/XrdFixedReplicator.hh
  XrdFixed/XrdFixedRedirector.cc XrdFixed/XrdFixedRedirector.hh
  XrdFixed/XrdFixedDataNode.cc XrdFixed/XrdFixedDataNode.hh )

set_target_properties(
  XrdFixedDataNode
  PROPERTIES
  VERSION   ${XRD_FIXED_VERSION}
  SOVERSION ${XRD_FIXED_SOVERSION}
  INTERFACE_LINK_LIBRARIES ""
  LINK_INTERFACE_LIBRARIES "" )

target_link_libraries(
  XrdFixedDataNode
  XrdCl
  ${OPENSSL_LIBRARIES} )

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------
install(
  TARGETS XrdFixed XrdFixedDataNode
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )
