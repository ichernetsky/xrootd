/******************************************************************************/
/*                                                                            */
/*                             X r d F i x e d . c c                          */
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

#include "XrdFixed/XrdFixedRedirector.hh"


/*****************************************************************************/
/*      X r d F i x e d R e d i r e c t o r  c o n s t r u c t o r           */
/*****************************************************************************/
XrdFixedRedirector::XrdFixedRedirector(const char* configFN, 
                                        XrdSysError& FixedEroute, 
                                        XrdOucTrace& FixedTrace) {
}

XrdFixedRedirector::~XrdFixedRedirector() {
}

/* Given a file name return a node name where the file should be created */
const char* XrdFixedRedirector::node(const char* path) {
    return 0;
}

