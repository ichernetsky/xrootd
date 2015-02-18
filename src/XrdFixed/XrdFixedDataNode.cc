/******************************************************************************/
/*                                                                            */
/*                 X r d F i x e d D a t a N o d e. h h                       */
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

#include <stdio.h>

#include "XrdFixed/XrdFixedDataNode.hh"

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
XrdVERSIONINFO(XrdSfsGetFileSystem, XrdFixed);
XrdFixedDataNode XrdFixedDataNodeFS;

/*****************************************************************************/
/*                       G e t F i l e S y s t e m                           */
/*****************************************************************************/
extern "C" {
XrdSfsFileSystem *XrdSfsGetFileSystem(XrdSfsFileSystem *nativeFS,
                                      XrdSysLogger *logger,
                                      const char *configFN) {
  FixedEroute.SetPrefix("fixedDataNode_");
  FixedEroute.logger(logger);
  FixedEroute.Say("(c) 2015 Qualys Inc. XrdFixedDataNode version " XrdVSTRING);

  if (nativeFS == NULL) {
    FixedEroute.Emsg("XrdFixedDataNode", "native file system object can not be null");
    return NULL;
  }

  XrdFixedDataNodeFS.setNativeFS(nativeFS);

  return &XrdFixedDataNodeFS;
}
}

/*****************************************************************************/
/*       X r d F i x e d D a t a N o d e   c o n s t r u c t o r             */
/*****************************************************************************/
XrdFixedDataNode::XrdFixedDataNode() : nativeFS(NULL) {}

/*****************************************************************************/
/*       X r d F i x e d D a t a N o d e   d e s t r u c t o r               */
/*****************************************************************************/
XrdFixedDataNode::~XrdFixedDataNode() {}

/*****************************************************************************/
/*                                                                           */
/*           F i l e   S y s t e m   O b j e c t  I n t e r f a c e          */
/*                                                                           */
/*****************************************************************************/
XrdSfsDirectory *XrdFixedDataNode::newDir(char *user, int monid) {
  FixedEroute.Say("XrdFixedDataNode::newDir");
  return new XrdFixedDataNodeDirectory(user, monid);
}

XrdSfsFile *XrdFixedDataNode::newFile(char *user, int monid) {
  FixedEroute.Say("XrdFixedDataNode::newFile");
  return new XrdFixedDataNodeFile(user, monid);
}

/* Change file mod settings */
int XrdFixedDataNode::chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::chmod");
  return nativeFS->chmod(path, mode, eInfo, client, opaque);
}

/* File system control operation */
int XrdFixedDataNode::fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client) {
  FixedEroute.Say("XrdFixedDataNode::fsctl");
  return nativeFS->fsctl(cmd, args, eInfo, client);
}

/* Return statistical information */
int XrdFixedDataNode::getStats(char *buff, int blen) {
  FixedEroute.Say("XrdFixedDataNode::getStats");
  return nativeFS->getStats(buff, blen);
}

/* Get version string */
const char *XrdFixedDataNode::getVersion() {
  FixedEroute.Say("XrdFixedDataNode::getVersion");
  return nativeFS->getVersion();
}

/* Return directory/file existense information (short list)  */
int XrdFixedDataNode::exists(const char *path, XrdSfsFileExistence &eFlag,
                             XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                             const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::exists");
  return nativeFS->exists(path, eFlag, eInfo, client, opaque);
}

/* Create directory */
int XrdFixedDataNode::mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::mkdir");
  return nativeFS->mkdir(path, mode, eInfo, client, opaque);
}

/* Prepare a file for future processing */
int XrdFixedDataNode::prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo,
                              const XrdSecEntity *client) {
  FixedEroute.Say("XrdFixedDataNode::prepare");
  return nativeFS->prepare(pargs, eInfo, client);
}

/* Remove a file */
int XrdFixedDataNode::rem(const char *path, XrdOucErrInfo &eInfo,
                          const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::rem");
  return nativeFS->rem(path, eInfo, client, opaque);
}

/* Remove directory */
int XrdFixedDataNode::remdir(const char *path, XrdOucErrInfo &eInfo,
                             const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::remdir");
  return nativeFS->remdir(path, eInfo, client, opaque);
}

/* Rename a file or directory */
int XrdFixedDataNode::rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo,
                             const XrdSecEntity *client, const char *opaqueO,
                             const char *opaqueN) {
  FixedEroute.Say("XrdFixedDataNode::rename");
  return nativeFS->rename(oPath, nPath, eInfo, client, opaqueO, opaqueN);
}

/* Return state information on file or a directory */
int XrdFixedDataNode::stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo,
                           const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::stat 5");
  return nativeFS->stat(name, buf, eInfo, client, opaque);
}
int XrdFixedDataNode::stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo,
                           const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::stat 5.1");
  return nativeFS->stat(name, mod, eInfo, client, opaque);
}

/* Truncate a file */
int XrdFixedDataNode::truncate(const char *path, XrdSfsFileOffset fsize,
                               XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                               const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode::truncate");
  return nativeFS->truncate(path, fsize, eInfo, client);
}

/*****************************************************************************/
/*           X r d F i x e d   i n t e r n a l  f u n c t i o n s            */
/*****************************************************************************/
void XrdFixedDataNode::setNativeFS(XrdSfsFileSystem *native) { nativeFS = native; }

/*****************************************************************************/
/*                                                                           */
/*                  F i l e  O b j e c t  I n t e r f a c e                  */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*            X r d F i x e d F i l e   c o n s t r u c t o r                */
/*****************************************************************************/
XrdFixedDataNodeFile::XrdFixedDataNodeFile(char *user, int MonID) {
  /* Initialize the natvie file object */
  FixedEroute.Say("XrdFixedDataNodeFile::ctor");
  nativeFile = XrdFixedDataNodeFS.nativeFS->newFile(user, MonID);
}

/*****************************************************************************/
/*            X r d F i x e d F i l e   d e s t r u c t o r                  */
/*****************************************************************************/
XrdFixedDataNodeFile::~XrdFixedDataNodeFile() { delete nativeFile; }

/* Open a file */
int XrdFixedDataNodeFile::open(const char *fileName, XrdSfsFileOpenMode openMode,
                               mode_t createMode, const XrdSecEntity *client,
                               const char *opaque) {
  return nativeFile->open(fileName, openMode, createMode, client, opaque);
}

/* Close a file */
int XrdFixedDataNodeFile::close() {
  FixedEroute.Say("XrdFixedDataNodeFile::close");
  return nativeFile->close();
}

/* Execute a special operation on the file */
int XrdFixedDataNodeFile::fctl(const int cmd, const char *args, XrdOucErrInfo &eInfo) {

  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::fctnl");

  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->fctl(cmd, args, eInfo)) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
}

/* Get File Path */
const char *XrdFixedDataNodeFile::FName() {
  FixedEroute.Say("XrdFixedDataNodeFile::FName");
  return nativeFile->FName();
}

/* Get file's memory mapping if one exists (memory mapped files only) */
int XrdFixedDataNodeFile::getMmap(void **Addr, off_t &Size) {
  FixedEroute.Say("XrdFixedDataNodeFile::getMmap");
  return nativeFile->getMmap(Addr, Size);
}

/* Preread file blocks into the file system cache */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsFileOffset offset,
                                          XrdSfsXferSize size) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::read 1");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(offset, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Read file bytes into a buffer */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsFileOffset offset, char *buffer,
                                          XrdSfsXferSize size) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::read 2");

  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(offset, buffer, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Read file bytes using asychronous I/O */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsAio *aioparam) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::read 3");

  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(aioparam)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Write file bytes from a buffer */
XrdSfsXferSize XrdFixedDataNodeFile::write(XrdSfsFileOffset offset, const char *buffer,
                                   XrdSfsXferSize size) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::write 1");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->write(offset, buffer, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Write file bytes using asynchronous I/O */
int XrdFixedDataNodeFile::write(XrdSfsAio *aioparam) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::write 2");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->write(aioparam)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Return state information on the file */
int XrdFixedDataNodeFile::stat(struct stat *buf) {
  FixedEroute.Say("XrdFixedDataNodeFile::stat");
  return nativeFile->stat(buf);
}

/* Make sure all the outstanding data is actually written on the file (sync) */
int XrdFixedDataNodeFile::sync() {

  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::sync 1");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->sync()) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
}

/* Make sure all outstanding data is actually written to the file (async) */
int XrdFixedDataNodeFile::sync(XrdSfsAio *aiop) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile::sync 2");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->sync(aiop)) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
}

/* Truncate the file */
int XrdFixedDataNodeFile::truncate(XrdSfsFileOffset fsize) {
 int ret = 0;

  FixedEroute.Say("XrdFixedDataNodeFile::truncate");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->truncate(fsize)) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
}

/* Get compression information on the file */
int XrdFixedDataNodeFile::getCXinfo(char cxtype[4], int &cxrsz) {
  FixedEroute.Say("XrdFixedDataNodeFile::getCXinfo");
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
XrdFixedDataNodeDirectory::XrdFixedDataNodeDirectory(char *user, int MonID) {
  /* Initialize the natvie directory object */
  FixedEroute.Say("XrdFixedDataNodeDirectory::ctor");
  nativeDirectory = XrdFixedDataNodeFS.nativeFS->newDir(user, MonID);
}

/*****************************************************************************/
/*        X r d F i x e d D i r e c t o r y  d e s t r u c t o r             */
/*****************************************************************************/
XrdFixedDataNodeDirectory::~XrdFixedDataNodeDirectory() {
  FixedEroute.Say("XrdFixedDataNodeDirectory destructor");
  delete nativeDirectory;
}

/* Open a directory. */
int XrdFixedDataNodeDirectory::open(const char *path, const XrdSecEntity *client,
                            const char *opaque) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeDirectory::open");

  this->error.Reset();
  nativeDirectory->error = this->error;

  if ((ret = nativeDirectory->open(path, client, opaque)) != SFS_OK)
    this->error = nativeDirectory->error;

  return ret;
}

/* Get the next directory entry. */
const char *XrdFixedDataNodeDirectory::nextEntry() {
  FixedEroute.Say("XrdFixedDataNodeDirectory::nextEntry");
  return nativeDirectory->nextEntry();
}

/* Close the file. */
int XrdFixedDataNodeDirectory::close() {
  FixedEroute.Say("XrdFixedDataNodeDirectory::close");
  return nativeDirectory->close();
}

/* Get the directory path. */
const char *XrdFixedDataNodeDirectory::FName() {
  FixedEroute.Say("XrdFixedDataNodeDirectory::FName");
  return nativeDirectory->FName();
}
