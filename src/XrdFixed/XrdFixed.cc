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
XrdVERSIONINFO(XrdSfsGetFileSystem,XrdFixed);
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
XrdFixed::~XrdFixed() {
    delete nativeFS;
}

/*****************************************************************************/
/*                                                                           */
/*           F i l e   S y s t e m   O b j e c t  I n t e r f a c e          */
/*                                                                           */
/*****************************************************************************/
XrdSfsDirectory *XrdFixed::newDir(char *user, int monid) { 
    FixedEroute.Say("XrdFixed::newDir");
    return new XrdFixedDirectory(user, monid); 
}

XrdSfsFile *XrdFixed::newFile(char *user, int monid) { 
    FixedEroute.Say("XrdFixed::newFile");
    return new XrdFixedFile(user, monid); 
}

/* Change file mod settings */
int XrdFixed::chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                    const char *opaque) {
    FixedEroute.Say("XrdFixed::chmod");
    return nativeFS->chmod(path, mode, eInfo, client, opaque);
}

/* File system control operation */
int XrdFixed::fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo, const XrdSecEntity *client) { 
    FixedEroute.Say("XrdFixed::fsctl");
    return nativeFS->fsctl(cmd, args, eInfo, client);
}

/* Return statistical information */
int XrdFixed::getStats(char *buff, int blen) { 
    FixedEroute.Say("XrdFixed::getStats");
    return nativeFS->getStats(buff, blen);
}

/* Get version string */
const char *XrdFixed::getVersion() { 
    FixedEroute.Say("XrdFixed::getVersion");
    return nativeFS->getVersion(); 
}

/* Return directory/file existense information (short list)  */
int XrdFixed::exists(const char *path, XrdSfsFileExistence &eFlag, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                     const char *opaque) {
    FixedEroute.Say("XrdFixed::exists");
    return nativeFS->exists(path, eFlag, eInfo, client, opaque);
}

/* Create directory */
int XrdFixed::mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                    const char *opaque) {
    FixedEroute.Say("XrdFixed::mkdir");
    return nativeFS->mkdir(path, mode, eInfo, client, opaque);
}

/* Prepare a file for future processing */
int XrdFixed::prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo, const XrdSecEntity *client) { 
    FixedEroute.Say("XrdFixed::prepare");
    return nativeFS->prepare(pargs, eInfo, client); 
}

/* Remove a file */
int XrdFixed::rem(const char *path, XrdOucErrInfo &eInfo, const XrdSecEntity *client, const char *opaque) { 
    FixedEroute.Say("XrdFixed::rem");
    return nativeFS->rem(path, eInfo, client, opaque); 
}

/* Remove directory */
int XrdFixed::remdir(const char *path, XrdOucErrInfo &eInfo, const XrdSecEntity *client, const char *opaque) {
    FixedEroute.Say("XrdFixed::remdir");
    return nativeFS->remdir(path, eInfo, client, opaque);
}

/* Rename a file or directory */
int XrdFixed::rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                     const char *opaqueO, const char *opaqueN) {
    FixedEroute.Say("XrdFixed::rename");
    return nativeFS->rename(oPath, nPath, eInfo, client, opaqueO, opaqueN);
}

/* Return state information on file or a directory */
int XrdFixed::stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                   const char *opaque) {
    FixedEroute.Say("XrdFixed::stat");
    return nativeFS->stat(name, buf, eInfo, client, opaque);
}
int XrdFixed::stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                   const char *opaque) {
    FixedEroute.Say("XrdFixed::stat");
    return nativeFS->stat(name, mod, eInfo, client, opaque);
}

/* Truncate a file */
int XrdFixed::truncate(const char *path, XrdSfsFileOffset fsize, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                       const char *opaque) {
    FixedEroute.Say("XrdFixed::truncate");
    return nativeFS->truncate(path, fsize, eInfo, client);
}

/*****************************************************************************/
/*           X r d F i x e d   i n t e r n a l  f u n c t i o n s            */
/*****************************************************************************/
void XrdFixed::setNativeFS(XrdSfsFileSystem *native) { 
    nativeFS = native; 
}

/*****************************************************************************/
/*                                                                           */
/*                  F i l e  O b j e c t  I n t e r f a c e                  */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*            X r d F i x e d F i l e   c o n s t r u c t o r                */
/*****************************************************************************/
XrdFixedFile::XrdFixedFile(char* user, int MonID) {
    /* Initialize the natvie file object */
    FixedEroute.Say("XrdFixedFile::ctor");
    nativeFile = XrdFixedFS.nativeFS->newFile(user, MonID);
}

/*****************************************************************************/
/*            X r d F i x e d F i l e   d e s t r u c t o r                  */
/*****************************************************************************/
XrdFixedFile::~XrdFixedFile() {
    delete nativeFile;
}

/* Open a file */
int XrdFixedFile::open(const char *fileName, XrdSfsFileOpenMode openMode, mode_t createMode, const XrdSecEntity *client,
                       const char *opaque) {
    FixedEroute.Say("XrdFixedFile::open");
    return nativeFile->open(fileName, openMode, createMode, client, opaque);
}

/* Close a file */
int XrdFixedFile::close() { 
    FixedEroute.Say("XrdFixedFile::close");
    return nativeFile->close(); 
}

/* Execute a special operation on the file */
int XrdFixedFile::fctl(const int cmd, const char *args, XrdOucErrInfo& eInfo) { 
    FixedEroute.Say("XrdFixedFile::fctnl");
    return nativeFile->fctl(cmd, args, eInfo); 
}

/* Get File Path */
const char *XrdFixedFile::FName() { 
    FixedEroute.Say("XrdFixedFile::FName");
    return nativeFile->FName(); 
}

/* Get file's memory mapping if one exists (memory mapped files only) */
int XrdFixedFile::getMmap(void **Addr, off_t &Size) { 
    FixedEroute.Say("XrdFixedFile::getMmap");
    return nativeFile->getMmap(Addr, Size); 
}

/* Preread file blocks into the file system cache */
XrdSfsXferSize XrdFixedFile::read(XrdSfsFileOffset offset, XrdSfsXferSize size) { 
    FixedEroute.Say("XrdFixedFile::read 1");
    return nativeFile->read(offset, size);
}

/* Read file bytes into a buffer */
XrdSfsXferSize XrdFixedFile::read(XrdSfsFileOffset offset, char *buffer, XrdSfsXferSize size) { 
    FixedEroute.Say("XrdFixedFile::read 1");
    return nativeFile->read(offset, buffer, size);
}

/* Read file bytes using asychronous I/O */
XrdSfsXferSize XrdFixedFile::read(XrdSfsAio *aioparam) { 
    FixedEroute.Say("XrdFixedFile::read 2");
    return nativeFile->read(aioparam); 
}

/* Write file bytes from a buffer */
XrdSfsXferSize XrdFixedFile::write(XrdSfsFileOffset offset, const char *buffer, XrdSfsXferSize size) { 
    FixedEroute.Say("XrdFixedFile::write 1");
    return nativeFile->write(offset, buffer, size);
}

/* Write file bytes using asynchronous I/O */
int XrdFixedFile::write(XrdSfsAio *aioparam) { 
    FixedEroute.Say("XrdFixedFile::write 2");
    return nativeFile->write(aioparam);
}

/* Return state information on the file */
int XrdFixedFile::stat(struct stat *buf) { 
    FixedEroute.Say("XrdFixedFile::stat");
    return nativeFile->stat(buf);
}

/* Make sure all the outstanding data is actually written on the file (sync) */
int XrdFixedFile::sync() { 
    FixedEroute.Say("XrdFixedFile::sync 1");
    return nativeFile->sync();
}

/* Make sure all outstanding data is actually written to the file (async) */
int XrdFixedFile::sync(XrdSfsAio *aiop) { 
    FixedEroute.Say("XrdFixedFile::sync 2");
    return nativeFile->sync(aiop);
}

/* Truncate the file */
int XrdFixedFile::truncate(XrdSfsFileOffset fsize) { 
    FixedEroute.Say("XrdFixedFile::truncate");
    return nativeFile->truncate(fsize);
}

/* Get compression information on the file */
int XrdFixedFile::getCXinfo(char cxtype[4], int &cxrsz) { 
    FixedEroute.Say("XrdFixedFile::getCXinfo");
    return nativeFile->getCXinfo(cxtype, cxrsz);
}

/*****************************************************************************/
/*                                                                           */
/*            D i r e c t o r y  O b j e c t  I n t e r f a c e              */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*      X r d F i x e d D i r e c t o r y  c o n s t r u c t o r             */
/*****************************************************************************/
XrdFixedDirectory::XrdFixedDirectory(char* user, int MonID) {
    /* Initialize the natvie directory object */
    FixedEroute.Say("XrdFixedDirectory::ctor");
    nativeDirectory = XrdFixedFS.nativeFS->newDir(user, MonID);
}

/*****************************************************************************/
/*        X r d F i x e d D i r e c t o r y  d e s t r u c t o r             */
/*****************************************************************************/
XrdFixedDirectory::~XrdFixedDirectory() {
    FixedEroute.Say("XrdFixedDirectory destructor");
    delete nativeDirectory;
}

/* Open a directory. */
int XrdFixedDirectory::open(const char *path, const XrdSecEntity *client, const char *opaque) {
    FixedEroute.Say("XrdFixedDirectory::open");
    return nativeDirectory->open(path, client, opaque);
}

/* Get the next directory entry. */
const char *XrdFixedDirectory::nextEntry() {
    FixedEroute.Say("XrdFixedDirectory::nextEntry");
    return nativeDirectory->nextEntry();
}

/* Close the file. */
int XrdFixedDirectory::close() {
    FixedEroute.Say("XrdFixedDirectory::close");
    return nativeDirectory->close();
}

/* Get the directory path. */
const char *XrdFixedDirectory::FName() {
    FixedEroute.Say("XrdFixedDirectory::FName");
    return nativeDirectory->FName();
}

