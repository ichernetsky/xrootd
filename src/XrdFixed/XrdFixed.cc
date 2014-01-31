/******************************************************************************/
/*                                                                            */
/*                             X r d F i x e d . h h                          */
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

#include "XrdFixed/XrdFixed.hh"

#include "XrdVersion.hh"
#include "XrdSys/XrdSysError.hh"
#include "XrdOuc/XrdOucTrace.hh"

/*****************************************************************************/
/*                  X r d F i x e d   E r r o r  r o u t i n g               */
/*****************************************************************************/
XrdSysError FixedEroute(0);
XrdOucTrace FixedTrace(&FixedEroute);

/*****************************************************************************/
/*           X r d F i x e d   F i l e  S y s t e m  O b j e c t             */
/*****************************************************************************/
XrdFixed XrdFixedFS;

/*****************************************************************************/
/*                       G e t F i l e S y s t e m                           */
/*****************************************************************************/
extern "C" {
XrdSfsFileSystem *XrdSfsGetFileSystem(XrdSfsFileSystem *nativeFS, XrdSysLogger *logger, const char *configFN) {
    FixedEroute.SetPrefix("fixed_");
    FixedEroute.logger(logger);
    FixedEroute.Say("(c) 2014 Qualys Inc. XrdFixed version " XrdVSTRING);

    if (nativeFS == NULL) {
        FixedEroute.Say("Error: native file system object can not be null");
        return NULL;
    }

    XrdFixedFS.setNativeFS(nativeFS);

    return &XrdFixedFS;
}
}

/*****************************************************************************/
/*                  X r d F i x e d   c o n s t r u c t o r                  */
/*****************************************************************************/
XrdFixed::XrdFixed() : nativeFS(NULL) {}

/*****************************************************************************/
/*                  X r d F i x e d   d e s t r u c t o r                    */
/*****************************************************************************/
XrdFixed::~XrdFixed() {}

/*****************************************************************************/
/*                                                                           */
/*           F i l e   S y s t e m   O b j e c t  I n t e r f a c e          */
/*                                                                           */
/*****************************************************************************/
XrdSfsDirectory *XrdFixed::newDir(char *user, int monid) { return 0; }

XrdSfsFile *XrdFixed::newFile(char *user, int monid) { return 0; }

/* Change file mod settings */
int XrdFixed::chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                    const char *opaque) {
    return 0;
}

/* File system control operation */
int XrdFixed::fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo, const XrdSecEntity *client) { return 0; }

/* Return statistical information */
int XrdFixed::getStats(char *buff, int blen) { return 0; }

/* Get version string */
const char *XrdFixed::getVersion() { return 0; }

/* Return directory/file existense information (short list)  */
int XrdFixed::exists(const char *path, XrdSfsFileExistence &eFlag, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                     const char *opaque) {
    return 0;
}

/* Create directory */
int XrdFixed::mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                    const char *opaque) {
    return 0;
}

/* Prepare a file for future processing */
int XrdFixed::prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo, const XrdSecEntity *cluent) { return 0; }

/* Remove a file */
int XrdFixed::rem(const char *path, XrdOucErrInfo &eInfo, const XrdSecEntity *client, const char *opaque) { return 0; }

/* Remove directory */
int XrdFixed::remdir(const char *path, XrdOucErrInfo &eInfo, const XrdSecEntity *client, const char *opaque) {
    return 0;
}

/* Rename a file or directory */
int XrdFixed::rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                     const char *opaqueO, const char *opaqueN) {
    return 0;
}

/* Return state information on file or a directory */
int XrdFixed::stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo, const XrdSecEntity *cilent,
                   const char *opaque) {
    return 0;
}
int XrdFixed::stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo, const XrdSecEntity *cilent,
                   const char *opaque) {
    return 0;
}

/* Truncate a file */
int truncate(const char *path, XrdSfsFileOffset fsize, XrdOucErrInfo &eInfo, const XrdSecEntity *client = 0,
             const char *opaque = 0) {
    return 0;
}

/*****************************************************************************/
/*                                                                           */
/*                  F i l e  O b j e c t  I n t e r f a c e                  */
/*                                                                           */
/*****************************************************************************/
/* Open a file */
int XrdFixedFile::open(const char *fileName, XrdSfsFileOpenMode openMode, mode_t createMode, const XrdSecEntity *client,
                       const char *opaque) {
    return 0;
}

/* Close a file */
int XrdFixedFile::close() { return 0; }

/* Execute a special operation on the file */
int XrdFixedFile::fctnl(const int cmd, const char *args, XrdOucErrInfo *eInfo) { return 0; }

/* Get File Path */
const char *XrdFixedFile::FName() { return 0; }

/* Get file's memoery mapping if one exists (memory mapped files only) */
int XrdFixedFile::getMmap(void **Addr, off_t &Size) { return 0; }

/* Read file bytes into a buffer */
XrdSfsXferSize XrdFixedFile::read(XrdSfsFileOffset offset, char *buffer, XrdSfsXferSize size) { return 0; }

/* Read file bytes using asychronous I/O */
XrdSfsXferSize XrdFixedFile::read(XrdSfsAio *aioparam) { return 0; }

/* Write file bytes from a buffer */
XrdSfsXferSize XrdFixedFile::write(XrdSfsFileOffset offset, const char *buffer, XrdSfsXferSize size) { return 0; }

/* Write file bytes using asynchronous I/O */
int XrdFixedFile::write(XrdSfsAio *aioparm) { return 0; }

/* Return state information on the file */
int XrdFixedFile::stat(struct stat *buf) { return 0; }

/* Make sure all the outstanding data is actually written on the file (sync) */
int XrdFixedFile::sync() { return 0; }

/* Make sure all outstanding data is actually written to the file (async) */
int XrdFixedFile::sync(XrdSfsAio *aiop) { return 0; }

/* Truncate the file */
int XrdFixedFile::truncate(XrdSfsFileOffset fsize) { return 0; }

/* Get compression information on the file */
int XrdFixedFile::getCXinfo(char cxtype[4], int &cxrsz) { return 0; }

/* Destructor */
XrdFixedFile::~XrdFixedFile() {}

/*****************************************************************************/
/*           X r d F i x e d   i n t e r n a l  f u n c t i o n s            */
/*****************************************************************************/
void XrdFixed::setNativeFS(XrdSfsFileSystem *native) { nativeFS = native; }
