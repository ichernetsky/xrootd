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

#include <stdio.h>

#include "XrdFixed/XrdFixed.hh"

#include "XrdVersion.hh"
#include "XrdSys/XrdSysError.hh"
#include "XrdOuc/XrdOucTrace.hh"

#include "XrdCl/XrdClCopyProcess.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClURL.hh"

/*****************************************************************************/
/*                  X r d F i x e d   E r r o r  r o u t i n g               */
/*****************************************************************************/
XrdSysError FixedEroute(0);
XrdOucTrace FixedTrace(&FixedEroute);

/*****************************************************************************/
/*           X r d F i x e d   F i l e  S y s t e m  O b j e c t             */
/*****************************************************************************/
XrdVERSIONINFO(XrdSfsGetFileSystem, XrdFixed);
XrdFixed XrdFixedFS;

/*****************************************************************************/
/*                       G e t F i l e S y s t e m                           */
/*****************************************************************************/
extern "C" {
XrdSfsFileSystem *XrdSfsGetFileSystem(XrdSfsFileSystem *nativeFS,
                                      XrdSysLogger *logger,
                                      const char *configFN) {
  FixedEroute.SetPrefix("fixed_");
  FixedEroute.logger(logger);
  FixedEroute.Say("(c) 2014 Qualys Inc. XrdFixed version " XrdVSTRING);

  if (nativeFS == NULL) {
    FixedEroute.Emsg("XrdFixed", "native file system object can not be null");
    return NULL;
  }
  
  XrdFixedFS.setNativeFS(nativeFS);
  XrdFixedRedirector* writeRedirector = new XrdFixedRedirector(configFN, FixedEroute, FixedTrace);

  if (writeRedirector->getnNodes() < 1) {
    FixedEroute.Emsg("XrdFixed", "data node configuration error");
    return NULL;
  }

  XrdFixedFS.setWriteRedirector(writeRedirector);

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
    if (writeRedirector) delete writeRedirector;
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
int XrdFixed::chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                    const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::chmod");
  return nativeFS->chmod(path, mode, eInfo, client, opaque);
}

/* File system control operation */
int XrdFixed::fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo,
                    const XrdSecEntity *client) {
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
int XrdFixed::exists(const char *path, XrdSfsFileExistence &eFlag,
                     XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                     const char *opaque) {
  FixedEroute.Say("XrdFixed::exists");
  return nativeFS->exists(path, eFlag, eInfo, client, opaque);
}

/* Create directory */
int XrdFixed::mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                    const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::mkdir");
  return redirect(path, eInfo);
  //return nativeFS->mkdir(path, mode, eInfo, client, opaque);
}

/* Prepare a file for future processing */
int XrdFixed::prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo,
                      const XrdSecEntity *client) {
  FixedEroute.Say("XrdFixed::prepare");
  return nativeFS->prepare(pargs, eInfo, client);
}

/* Remove a file */
int XrdFixed::rem(const char *path, XrdOucErrInfo &eInfo,
                  const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::rem");
  return redirect(path, eInfo);
  //return nativeFS->rem(path, eInfo, client, opaque);
}

/* Remove directory */
int XrdFixed::remdir(const char *path, XrdOucErrInfo &eInfo,
                     const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::remdir");
  return nativeFS->remdir(path, eInfo, client, opaque);
}

/* Rename a file or directory */
int XrdFixed::rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo,
                     const XrdSecEntity *client, const char *opaqueO,
                     const char *opaqueN) {
  FixedEroute.Say("XrdFixed::rename");

  /* use native when sourece and destintation belong to the same node */
  const char *srcNode = (XrdFixedFS.getWriteRedirector()->node(oPath));
  const char *tgtNode = (XrdFixedFS.getWriteRedirector()->node(nPath));

  if (strncmp(srcNode, tgtNode, XRD_FIXED_MAX_HOSTNAME_LEN + 1) == 0)
      return nativeFS->rename(oPath, nPath, eInfo, client, opaqueO, opaqueN);

  char srcUrl[XRD_FIXED_MAX_URL_LEN] = {0};
  char tgtUrl[XRD_FIXED_MAX_URL_LEN] = {0};
  
  if (snprintf(srcUrl, XRD_FIXED_MAX_URL_LEN, "root://%s:1094/%s", srcNode, oPath) >= XRD_FIXED_MAX_URL_LEN) {
      const char* err = "Error: source url length exceeds the limit";
      FixedEroute.Say(err);
      eInfo.setErrInfo(-1, err);
      return SFS_ERROR;
  }
  if (snprintf(tgtUrl, XRD_FIXED_MAX_URL_LEN, "root://%s:1094/%s", tgtNode, nPath) >= XRD_FIXED_MAX_URL_LEN) {
      const char* err = "Error: target url length exceeds the limit";
      FixedEroute.Say(err);
      eInfo.setErrInfo(-1, err);
      return SFS_ERROR;
  }

  // Else copy the file and then rename 
  //return nativeFS->rename(oPath, nPath, eInfo, client, opaqueO, opaqueN);
  XrdCl::CopyProcess cp;
  XrdCl::PropertyList properties, results;
  properties.Set("source", srcUrl);
  properties.Set("target", tgtUrl);
  properties.Set("thirdParty", "first");
  properties.Set("makeDir", true);

  XrdCl::XRootDStatus st;
  st = cp.AddJob(properties, &results); 
  if (!st.IsOK()) {
      FixedEroute.Say("Error: adding copy job failed ", st.ToString().c_str());
      eInfo.setErrInfo(st.errNo, st.ToString().c_str());
      return SFS_ERROR; 
  }

  st = cp.Prepare(); 
  if (!st.IsOK()) {
      FixedEroute.Say("Error: prepare failed for copy job ", st.ToString().c_str());
      eInfo.setErrInfo(st.errNo, st.ToString().c_str());
      return SFS_ERROR;
  }
  st = cp.Run(0); 
  if (!st.IsOK()) {
      FixedEroute.Say("Error: run failed for copy job ", st.ToString().c_str());
      eInfo.setErrInfo(st.errNo, st.ToString().c_str());
      return SFS_ERROR;
  }

  // Remove original file 
  std::string strSrcNode(srcNode);
  XrdCl::FileSystem fs(strSrcNode);
  st = fs.Rm(oPath);
  if (!st.IsOK()) {
      FixedEroute.Say("Error: Could not remove original file ");
      eInfo.setErrInfo(st.errNo, st.ToString().c_str());
      return SFS_ERROR;    
  }

  return SFS_OK;
}

/* Return state information on file or a directory */
int XrdFixed::stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo,
                   const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::stat 5");
  return redirect(name, eInfo);

  //return nativeFS->stat(name, buf, eInfo, client, opaque);
}
int XrdFixed::stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo,
                   const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixed::stat 5.1");
  return redirect(name, eInfo);
  //return nativeFS->stat(name, mod, eInfo, client, opaque);
}

/* Truncate a file */
int XrdFixed::truncate(const char *path, XrdSfsFileOffset fsize,
                       XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                       const char *opaque) {
  FixedEroute.Say("XrdFixed::truncate");
  return nativeFS->truncate(path, fsize, eInfo, client);
}

/*****************************************************************************/
/*           X r d F i x e d   i n t e r n a l  f u n c t i o n s            */
/*****************************************************************************/
void XrdFixed::setNativeFS(XrdSfsFileSystem *native) { nativeFS = native; }
void XrdFixed::setWriteRedirector(XrdFixedRedirector* r) { writeRedirector = r; }

/* Redirect a request to the right node */
int XrdFixed::redirect(const char* path, XrdOucErrInfo &eInfo) {
  const char *dataNode = (writeRedirector->node(path));
  eInfo.Reset();
  eInfo.setErrInfo(1094, dataNode);
  return SFS_REDIRECT;
}

XrdFixedRedirector* XrdFixed::getWriteRedirector() { return writeRedirector; }


/*****************************************************************************/
/*                                                                           */
/*                  F i l e  O b j e c t  I n t e r f a c e                  */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*            X r d F i x e d F i l e   c o n s t r u c t o r                */
/*****************************************************************************/
XrdFixedFile::XrdFixedFile(char *user, int MonID) {
  /* Initialize the natvie file object */
  FixedEroute.Say("XrdFixedFile::ctor");
  nativeFile = XrdFixedFS.nativeFS->newFile(user, MonID);
}

/*****************************************************************************/
/*            X r d F i x e d F i l e   d e s t r u c t o r                  */
/*****************************************************************************/
XrdFixedFile::~XrdFixedFile() { delete nativeFile; }


/* Open a file */
int XrdFixedFile::open(const char *fileName, XrdSfsFileOpenMode openMode,
                       mode_t createMode, const XrdSecEntity *client,
                       const char *opaque) {
  int ret;
  const int msg_len = 2048;
  char msg[msg_len] = {0};

  const int open_flag_len = 1024;
  char open_flags[open_flag_len] = {0};
  //int flags_index = 0;

  snprintf(msg, 2048, "XrdFixedFile::open fileName: %s, openMode: %s, createmode: %d, opaque: %s", 
           fileName, open_flags, createMode, opaque);
  FixedEroute.Say(msg);
  
  /* if someone is not trying to modify the file pass the request through */
  if (!(openMode & SFS_O_WRONLY) && !(openMode & SFS_O_RDWR) && 
      !(openMode & SFS_O_CREAT) && !(openMode & SFS_O_TRUNC) && 
      !(openMode & SFS_O_MKPTH) && !(SFS_O_RESET)) {
      this->error.Reset();
      nativeFile->error = this->error;
 
      if ((ret = nativeFile->open(fileName, openMode, createMode, client, opaque)) != SFS_OK)
          this->error = nativeFile->error;
      return ret;      
  }

  /* otherwise, always redirect */
  const char *dataNode = (XrdFixedFS.getWriteRedirector()->node(fileName));
  FixedEroute.Say("Redirecting to ", dataNode);
  this->error.setErrInfo(1094, dataNode);
  return SFS_REDIRECT;
}

/* Close a file */
int XrdFixedFile::close() {
  FixedEroute.Say("XrdFixedFile::close");
  return nativeFile->close();
}

/* Execute a special operation on the file */
int XrdFixedFile::fctl(const int cmd, const char *args, XrdOucErrInfo &eInfo) {
  
  int ret;
  FixedEroute.Say("XrdFixedFile::fctnl");
  
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->fctl(cmd, args, eInfo)) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
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
XrdSfsXferSize XrdFixedFile::read(XrdSfsFileOffset offset,
                                  XrdSfsXferSize size) {
  int ret;    
  FixedEroute.Say("XrdFixedFile::read 1");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(offset, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Read file bytes into a buffer */
XrdSfsXferSize XrdFixedFile::read(XrdSfsFileOffset offset, char *buffer,
                                  XrdSfsXferSize size) {
  int ret;
  FixedEroute.Say("XrdFixedFile::read 2");

  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(offset, buffer, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Read file bytes using asychronous I/O */
XrdSfsXferSize XrdFixedFile::read(XrdSfsAio *aioparam) {
  int ret;
  FixedEroute.Say("XrdFixedFile::read 3");

  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->read(aioparam)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Write file bytes from a buffer */
XrdSfsXferSize XrdFixedFile::write(XrdSfsFileOffset offset, const char *buffer,
                                   XrdSfsXferSize size) {
  int ret;    
  FixedEroute.Say("XrdFixedFile::write 1");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->write(offset, buffer, size)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Write file bytes using asynchronous I/O */
int XrdFixedFile::write(XrdSfsAio *aioparam) {
  int ret;
  FixedEroute.Say("XrdFixedFile::write 2");
  this->error.Reset();
  nativeFile->error = this->error;

  if ((ret = nativeFile->write(aioparam)) < 0)
      this->error = nativeFile->error;

  return ret;
}

/* Return state information on the file */
int XrdFixedFile::stat(struct stat *buf) {
  FixedEroute.Say("XrdFixedFile::stat");
  return nativeFile->stat(buf);
}

/* Make sure all the outstanding data is actually written on the file (sync) */
int XrdFixedFile::sync() {
  
  int ret;    
  FixedEroute.Say("XrdFixedFile::sync 1");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->sync()) != SFS_OK)
      this->error = nativeFile->error;
  
  return ret;
}

/* Make sure all outstanding data is actually written to the file (async) */
int XrdFixedFile::sync(XrdSfsAio *aiop) {
  int ret;    
  FixedEroute.Say("XrdFixedFile::sync 2");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->sync(aiop)) != SFS_OK)
      this->error = nativeFile->error;
  
  return ret;
}

/* Truncate the file */
int XrdFixedFile::truncate(XrdSfsFileOffset fsize) {
 int ret = 0;

  FixedEroute.Say("XrdFixedFile::truncate");

  this->error.Reset();
  nativeFile->error = this->error;
  if ((ret = nativeFile->truncate(fsize)) != SFS_OK)
      this->error = nativeFile->error;

  return ret;
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
XrdFixedDirectory::XrdFixedDirectory(char *user, int MonID) {
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
int XrdFixedDirectory::open(const char *path, const XrdSecEntity *client,
                            const char *opaque) {
  int ret;    
  FixedEroute.Say("XrdFixedDirectory::open");

  this->error.Reset();
  nativeDirectory->error = this->error;
 
  if ((ret = nativeDirectory->open(path, client, opaque)) != SFS_OK)
    this->error = nativeDirectory->error;

  return ret;
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
