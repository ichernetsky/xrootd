#ifndef __XRD_FIXED_REDIRECTOR_H__
#define __XRD_FIXED_REDIRECTOR_H__
/******************************************************************************/
/*                                                                            */
/*               X r d F i x e d R e d i r e c t o r. h h                     */
/*                                                                            */
/* (c) 2014 Qualys. Inc.                                                      */
/* Author: Artem Harutyunyan (hartem_at_qualys_dot_com)                       */
/*                                                                            */
/* This file is part of the XRootD software suite.                            */
/*                                                                            */
/* XRootD is free software: you can redistribute it and/or modify it under    */
/* the terms of the GNU Lesser General Public License as published by the     */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* XRootD is distributed in the hope that it will be useful, but WITHOUT      */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public       */
/* License for more details.                                                  */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public License   */
/* along with XRootD in a file called COPYING.LESSER (LGPL license) and file  */
/* COPYING (GPL license).  If not, see <http://www.gnu.org/licenses/>.        */
/*                                                                            */
/* The copyright holder's institutional names and contributor's names may not */
/* be used to endorse or promote products derived from this software without  */
/* specific prior written permission of the institution or contributor.       */
/******************************************************************************/

/*****************************************************************************/
/*                  C l a s s    X r d F i x e d R e d i r e c t o r         */
/*****************************************************************************/

#include "XrdSys/XrdSysError.hh"
#include "XrdOuc/XrdOucTrace.hh"

#include "XrdFixed/XrdFixedDefines.hh"

class XrdFixedNode {
public:
  const char* getHostname() const;
  const char* getPort() const;
  int getNumericPort() const;

  void setPort(const char* PortStr, int Port);
  bool isPortSet() const;

  /* Constructor and Destructor */
  XrdFixedNode();
  XrdFixedNode(const char* Hostname);
  XrdFixedNode(const char* Hostname, const char* PortStr, int Port);
  virtual ~XrdFixedNode();

private:
  char hostname[XRD_FIXED_MAX_HOSTNAME_LEN + 1];
  char portStr[XRD_FIXED_MAX_PORT_LEN + 1];
  int port;
  bool isPortSet_;
}; // class XrdFixedNode

class XrdFixedRedirector {
public:

  /* Get node name for a given file name */
  const XrdFixedNode getNode(const char* path);

  /* Return number of nodes */
  int getNodeCount();

  /* Constructor and Destructor */
  XrdFixedRedirector(const char* configFN, XrdSysError& FixedEroute, XrdOucTrace& FixedTrace);
  virtual ~XrdFixedRedirector();

private:
  void normalizePath(const char* path, char* normalizedPath, unsigned int* size);

  XrdFixedNode nodes[XRD_FIXED_MAX_CLUSTER_SIZE];
  int nodeCount;

  char defaultPortStr[XRD_FIXED_MAX_PORT_LEN + 1];
  int defaultPort;

  XrdSysError& Eroute;
  XrdOucTrace& Trace;
}; // class XrdFixedRedirector

#endif // __XRD_FIXED_REDIRECTOR_H__
