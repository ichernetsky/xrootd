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
  //FixedEroute.Say("(c) 2015 Qualys Inc. XrdFixedDataNode version " XrdVSTRING);

  if (nativeFS == NULL) {
    FixedEroute.Emsg("XrdFixedDataNode ", "native file system object can not be null");
    return NULL;
  }

  XrdFixedDataNodeFS.setNativeFS(nativeFS);
  XrdFixedRedirector* writeRedirector = new XrdFixedRedirector(configFN,
                                        FixedEroute, FixedTrace);
  if (writeRedirector->getNodeCount() < 1) {
      FixedEroute.Emsg("XrdFixedDataNode", "data node configuration error");
      return NULL;
  }
  XrdFixedDataNodeFS.setWriteRedirector(writeRedirector);

  XrdFixedReplicator* replicator = new XrdFixedReplicator(configFN,
                                       FixedEroute, FixedTrace);
  XrdFixedDataNodeFS.setReplicator(replicator);

  return &XrdFixedDataNodeFS;
}
} // extern "C"

/*****************************************************************************/
/*       X r d F i x e d D a t a N o d e   c o n s t r u c t o r             */
/*****************************************************************************/
XrdFixedDataNode::XrdFixedDataNode() :
    m_nativeFS(NULL),
    m_replicator(NULL){}

/*****************************************************************************/
/*       X r d F i x e d D a t a N o d e   d e s t r u c t o r               */
/*****************************************************************************/
XrdFixedDataNode::~XrdFixedDataNode() {
    delete m_nativeFS;
    delete m_writeRedirector;
    delete m_replicator;
}

/*****************************************************************************/
/*                                                                           */
/*           F i l e   S y s t e m   O b j e c t  I n t e r f a c e          */
/*                                                                           */
/*****************************************************************************/
XrdSfsDirectory *XrdFixedDataNode::newDir(char *user, int monid) {
  FixedEroute.Say("XrdFixedDataNode ","newDir");
  return new XrdFixedDataNodeDirectory(user, monid);
}

XrdSfsFile *XrdFixedDataNode::newFile(char *user, int monid) {
  FixedEroute.Say("XrdFixedDataNode ","newFile");
  return new XrdFixedDataNodeFile(user, monid, getReplicator());
}

/* Change file mod settings */
int XrdFixedDataNode::chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","chmod ", path);
  return m_nativeFS->chmod(path, mode, eInfo, client, opaque);
}

/* File system control operation */
int XrdFixedDataNode::fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client) {
  FixedEroute.Say("XrdFixedDataNode ","fsctl");
  return m_nativeFS->fsctl(cmd, args, eInfo, client);
}

/* Return statistical information */
int XrdFixedDataNode::getStats(char *buff, int blen) {
  FixedEroute.Say("XrdFixedDataNode ","getStats");
  return m_nativeFS->getStats(buff, blen);
}

/* Get version string */
const char *XrdFixedDataNode::getVersion() {
  FixedEroute.Say("XrdFixedDataNode ","getVersion");
  return m_nativeFS->getVersion();
}

/* Return directory/file existense information (short list)  */
int XrdFixedDataNode::exists(const char *path, XrdSfsFileExistence &eFlag,
                             XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                             const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","exists ", path);
  return m_nativeFS->exists(path, eFlag, eInfo, client, opaque);
}

/* Create directory */
int XrdFixedDataNode::mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
                            const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","mkdir ", path);
  return m_nativeFS->mkdir(path, mode, eInfo, client, opaque);
}

/* Prepare a file for future processing */
int XrdFixedDataNode::prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo,
                              const XrdSecEntity *client) {
  FixedEroute.Say("XrdFixedDataNode ","prepare ");
  return m_nativeFS->prepare(pargs, eInfo, client);
}

/* Remove a file */
int XrdFixedDataNode::rem(const char *path, XrdOucErrInfo &eInfo,
                          const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","rem ", path);
  return m_nativeFS->rem(path, eInfo, client, opaque);
}

/* Remove directory */
int XrdFixedDataNode::remdir(const char *path, XrdOucErrInfo &eInfo,
                             const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","remdir ", path);
  return m_nativeFS->remdir(path, eInfo, client, opaque);
}

/* Rename a file or directory */
int XrdFixedDataNode::rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo,
                             const XrdSecEntity *client, const char *opaqueO,
                             const char *opaqueN) {
  FixedEroute.Say("XrdFixedDataNode ","rename ", oPath, nPath);
  /* use native when sourece and destintation belong to the same node */
  XrdFixedNode srcNode = (XrdFixedDataNodeFS.getWriteRedirector()->getNode(oPath));
  XrdFixedNode tgtNode = (XrdFixedDataNodeFS.getWriteRedirector()->getNode(nPath));
  const char *srcHost = srcNode.getHostname();
  const char *tgtHost = tgtNode.getHostname();
  const char* srcPort = srcNode.getPort();
  const char* tgtPort = tgtNode.getPort();

  if (strncmp(srcNode.getHostname(), tgtNode.getHostname(),
              XRD_FIXED_MAX_HOSTNAME_LEN + 1) == 0 &&
      srcNode.getNumericPort() == tgtNode.getNumericPort())
    return m_nativeFS->rename(oPath, nPath, eInfo, client, opaqueO, opaqueN);

  char srcUrl[XRD_FIXED_MAX_URL_LEN] = {0};
  char tgtUrl[XRD_FIXED_MAX_URL_LEN] = {0};
  char srcNodePort[XRD_FIXED_MAX_URL_LEN] = {0};

  if (snprintf(srcUrl, XRD_FIXED_MAX_URL_LEN, "root://%s:%s/%s", srcHost, srcPort, oPath) >= XRD_FIXED_MAX_URL_LEN) {
      const char* err = "Error: source url length exceeds the limit";
      FixedEroute.Say(err);
      eInfo.setErrInfo(-1, err);
      return SFS_ERROR;
  }
  if (snprintf(tgtUrl, XRD_FIXED_MAX_URL_LEN, "root://%s:%s/%s", tgtHost, tgtPort, nPath) >= XRD_FIXED_MAX_URL_LEN) {
      const char* err = "Error: target url length exceeds the limit";
      FixedEroute.Say(err);
      eInfo.setErrInfo(-1, err);
      return SFS_ERROR;
  }

  if (snprintf(srcNodePort, XRD_FIXED_MAX_URL_LEN, "root://%s:%s", srcHost, srcPort) >= XRD_FIXED_MAX_URL_LEN) {
      // Should never get here
      const char* err = "Error: target url length exceeds the limit";
      FixedEroute.Say(err);
      eInfo.setErrInfo(-1, err);
      return SFS_ERROR;
  }

  XrdCl::CopyProcess cp;
  XrdCl::PropertyList properties, results;
  properties.Set("source", srcUrl);
  properties.Set("target", tgtUrl);
  properties.Set("makeDir", true);
  properties.Set("force", true);

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
  std::string strSrcNode(srcNodePort);
  XrdCl::FileSystem fs(strSrcNode);
  st = fs.Rm(oPath);

  if (!st.IsOK()) {
      FixedEroute.Say("Warning: Could not remove original file ", st.ToString().c_str());
      //eInfo.setErrInfo(st.errNo, st.ToString().c_str());
      //return SFS_ERROR;
  }

  return SFS_OK;
}

/* Return state information on file or a directory */
int XrdFixedDataNode::stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo,
                           const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","stat 5 ", name);
  return m_nativeFS->stat(name, buf, eInfo, client, opaque);
}
int XrdFixedDataNode::stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo,
                           const XrdSecEntity *client, const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","stat 5.1 ", name);
  return m_nativeFS->stat(name, mod, eInfo, client, opaque);
}

/* Truncate a file */
int XrdFixedDataNode::truncate(const char *path, XrdSfsFileOffset fsize,
                               XrdOucErrInfo &eInfo, const XrdSecEntity *client,
                               const char *opaque) {
  FixedEroute.Say("XrdFixedDataNode ","truncate ", path);
  return m_nativeFS->truncate(path, fsize, eInfo, client);
}

/*****************************************************************************/
/*           X r d F i x e d   i n t e r n a l  f u n c t i o n s            */
/*****************************************************************************/
void XrdFixedDataNode::setNativeFS(XrdSfsFileSystem *native)
     { m_nativeFS = native; }
void XrdFixedDataNode::setWriteRedirector(XrdFixedRedirector* redirector)
    { m_writeRedirector = redirector; }
void XrdFixedDataNode::setReplicator(XrdFixedReplicator* replicator)
    { m_replicator = replicator; }

XrdFixedRedirector* XrdFixedDataNode::getWriteRedirector()
    { return m_writeRedirector; }
XrdFixedReplicator* XrdFixedDataNode::getReplicator()
    { return m_replicator; }

/*****************************************************************************/
/*                                                                           */
/*                  F i l e  O b j e c t  I n t e r f a c e                  */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*            X r d F i x e d F i l e   c o n s t r u c t o r                */
/*****************************************************************************/
XrdFixedDataNodeFile::XrdFixedDataNodeFile(char *user, int MonID,
                                           const XrdFixedReplicator* replicator)
    : m_trackChanges(false) {
  /* Initialize the natvie file object */
  FixedEroute.Say("XrdFixedDataNodeFile ","ctor");
  m_nativeFile = XrdFixedDataNodeFS.m_nativeFS->newFile(user, MonID);
  m_replicator = new XrdFixedReplicator(*replicator);
}

/*****************************************************************************/
/*            X r d F i x e d F i l e   d e s t r u c t o r                  */
/*****************************************************************************/
XrdFixedDataNodeFile::~XrdFixedDataNodeFile() {
    delete m_nativeFile;
    delete m_replicator;
}

/* Open a file */
int XrdFixedDataNodeFile::open(const char *fileName, XrdSfsFileOpenMode openMode,
                               mode_t createMode, const XrdSecEntity *client,
                               const char *opaque) {
  int ret;
  const int msg_len = 2048;
  char msg[msg_len] = {0};
  snprintf(msg, msg_len, " fileName: %s, openMode: %d, createmode: %d, opaque: %s",
          fileName, openMode, createMode, opaque);

  FixedEroute.Say("XrdFixedDataNodeFile ","open ", msg);

  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->open(fileName, openMode, createMode, client, opaque)) != SFS_OK)
      this->error = m_nativeFile->error;

  /* Check if the replicator if enabled; enable tracking for this file if necessary */
  if (m_replicator->getReplication()) {
      int compareIndex = strlen(fileName) - strlen(XRD_FIXED_TRACK_FNAME);
      if ((compareIndex > 0) && (strncmp(&fileName[compareIndex], XRD_FIXED_TRACK_FNAME,
                                        strlen(XRD_FIXED_TRACK_FNAME)) == 0))
      m_trackChanges = true;
  }

  return ret;
}

/* Close a file */
int XrdFixedDataNodeFile::close() {
  FixedEroute.Say("XrdFixedDataNodeFile ","close ", m_nativeFile->FName());
  if (m_trackChanges)
      m_replicator->close();
  return m_nativeFile->close();
}

/* Execute a special operation on the file */
int XrdFixedDataNodeFile::fctl(const int cmd, const char *args, XrdOucErrInfo &eInfo) {

  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile ","fctnl ", m_nativeFile->FName());

  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->fctl(cmd, args, eInfo)) != SFS_OK)
      this->error = m_nativeFile->error;

  return ret;
}

/* Get File Path */
const char *XrdFixedDataNodeFile::FName() {
  FixedEroute.Say("XrdFixedDataNodeFile ","FName ", m_nativeFile->FName());
  return m_nativeFile->FName();
}

/* Get file's memory mapping if one exists (memory mapped files only) */
int XrdFixedDataNodeFile::getMmap(void **Addr, off_t &Size) {
  FixedEroute.Say("XrdFixedDataNodeFile ","getMmap ", m_nativeFile->FName());
  return m_nativeFile->getMmap(Addr, Size);
}

/* Preread file blocks into the file system cache */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsFileOffset offset,
                                          XrdSfsXferSize size) {
  int ret;

  const int msg_len = 2048;
  char msg[msg_len] = {0};
  snprintf(msg, msg_len, " offset: %lld, size: %d", offset, size);

  FixedEroute.Say("XrdFixedDataNodeFile ","read 1 ", m_nativeFile->FName(), msg);
  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->read(offset, size)) < 0)
      this->error = m_nativeFile->error;

  return ret;
}

/* Read file bytes into a buffer */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsFileOffset offset, char *buffer,
                                          XrdSfsXferSize size) {
  int ret;

  const int msg_len = 2048;
  char msg[msg_len] = {0};
  snprintf(msg, msg_len, " offset: %lld, size: %d", offset, size);

  FixedEroute.Say("XrdFixedDataNodeFile ","read 2 ", m_nativeFile->FName(), msg);

  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->read(offset, buffer, size)) < 0)
      this->error = m_nativeFile->error;

  return ret;
}

/* Read file bytes using asychronous I/O */
XrdSfsXferSize XrdFixedDataNodeFile::read(XrdSfsAio *aioparam) {
  int ret;

  FixedEroute.Say("XrdFixedDataNodeFile ","read 3 ", m_nativeFile->FName());

  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->read(aioparam)) < 0)
      this->error = m_nativeFile->error;

  return ret;
}

/* Write file bytes from a buffer */
XrdSfsXferSize XrdFixedDataNodeFile::write(XrdSfsFileOffset offset, const char *buffer,
                                   XrdSfsXferSize size) {
  int ret;

  const int msg_len = 2048;
  char msg[msg_len] = {0};
  snprintf(msg, msg_len, " offset: %lld, size: %d",offset, size);

  if (m_trackChanges)
      m_replicator->write(offset, size);

  FixedEroute.Say("XrdFixedDataNodeFile ","write 1 ", m_nativeFile->FName(), msg);
  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->write(offset, buffer, size)) < 0)
      this->error = m_nativeFile->error;

  return ret;
}

/* Write file bytes using asynchronous I/O */
int XrdFixedDataNodeFile::write(XrdSfsAio *aioparam) {
  int ret;

  FixedEroute.Say("XrdFixedDataNodeFile ","write 2 ", m_nativeFile->FName());
  this->error.Reset();
  m_nativeFile->error = this->error;

  if ((ret = m_nativeFile->write(aioparam)) < 0)
      this->error = m_nativeFile->error;

  return ret;
}

/* Return state information on the file */
int XrdFixedDataNodeFile::stat(struct stat *buf) {
  FixedEroute.Say("XrdFixedDataNodeFile ","stat ", m_nativeFile->FName());
  return m_nativeFile->stat(buf);
}

/* Make sure all the outstanding data is actually written on the file (sync) */
int XrdFixedDataNodeFile::sync() {

  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile ","sync 1 ", m_nativeFile->FName());

  this->error.Reset();
  m_nativeFile->error = this->error;
  if ((ret = m_nativeFile->sync()) != SFS_OK)
      this->error = m_nativeFile->error;

  return ret;
}

/* Make sure all outstanding data is actually written to the file (async) */
int XrdFixedDataNodeFile::sync(XrdSfsAio *aiop) {
  int ret;
  FixedEroute.Say("XrdFixedDataNodeFile ","sync 2 ", m_nativeFile->FName());

  this->error.Reset();
  m_nativeFile->error = this->error;
  if ((ret = m_nativeFile->sync(aiop)) != SFS_OK)
      this->error = m_nativeFile->error;

  return ret;
}

/* Truncate the file */
int XrdFixedDataNodeFile::truncate(XrdSfsFileOffset fsize) {
 int ret = 0;

  FixedEroute.Say("XrdFixedDataNodeFile ","truncate ", m_nativeFile->FName());

  this->error.Reset();
  m_nativeFile->error = this->error;
  if ((ret = m_nativeFile->truncate(fsize)) != SFS_OK)
      this->error = m_nativeFile->error;

  return ret;
}

/* Get compression information on the file */
int XrdFixedDataNodeFile::getCXinfo(char cxtype[4], int &cxrsz) {
  FixedEroute.Say("XrdFixedDataNodeFile ","getCXinfo");
  return m_nativeFile->getCXinfo(cxtype, cxrsz);
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
  FixedEroute.Say("XrdFixedDataNodeDirectory ","ctor");
  nativeDirectory = XrdFixedDataNodeFS.m_nativeFS->newDir(user, MonID);
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
  FixedEroute.Say("XrdFixedDataNodeDirectory ","open ", path);

  this->error.Reset();
  nativeDirectory->error = this->error;

  if ((ret = nativeDirectory->open(path, client, opaque)) != SFS_OK)
    this->error = nativeDirectory->error;

  return ret;
}

/* Get the next directory entry. */
const char *XrdFixedDataNodeDirectory::nextEntry() {
  FixedEroute.Say("XrdFixedDataNodeDirectory ","nextEntry ", nativeDirectory->FName());
  return nativeDirectory->nextEntry();
}

/* Close the file. */
int XrdFixedDataNodeDirectory::close() {
  FixedEroute.Say("XrdFixedDataNodeDirectory ","close ", nativeDirectory->FName());
  return nativeDirectory->close();
}

/* Get the directory path. */
const char *XrdFixedDataNodeDirectory::FName() {
  FixedEroute.Say("XrdFixedDataNodeDirectory ","FName ", nativeDirectory->FName());
  return nativeDirectory->FName();
}
