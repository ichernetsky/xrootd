#ifndef __XRD_FIXED_DATA_NODE_H__
#define __XRD_FIXED_DATA_NODE_H__
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

#include "XrdSfs/XrdSfsInterface.hh"
#include "XrdFixed/XrdFixedRedirector.hh"
#include "XrdFixed/XrdFixedReplicator.hh"

/*****************************************************************************/
/*                  C l a s s    X r d F i x e d D i r e c t o r y           */
/*****************************************************************************/
class XrdFixedDataNodeDirectory : public XrdSfsDirectory {
public:

  /* Open a directory. */
  int open(const char *path, const XrdSecEntity *client = 0,
           const char *opaque = 0);

  /* Get the next directory entry. */
  const char *nextEntry();

  /* Close the file. */
  int close();

  /* Get the directory path. */
  const char *FName();

  /* Constructor and Destructor */
  XrdFixedDataNodeDirectory(char *user, int MonID);
  virtual ~XrdFixedDataNodeDirectory();

private:
  XrdSfsDirectory *nativeDirectory;
}; // class XrdFixedDataNodeDirectory

/*****************************************************************************/
/*                  C l a s s    X r d F i x e d F i l e                     */
/*****************************************************************************/
class XrdFixedDataNodeFile : public XrdSfsFile {
public:

  /* Open a file */
  int open(const char *fileName, XrdSfsFileOpenMode openMode, mode_t createMode,
           const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Close a file */
  int close();

  /* Execute a special operation on the file */
  int fctl(const int cmd, const char *args, XrdOucErrInfo &eInfo);

  /* Get File Path */
  const char *FName();

  /* Get file's memory mapping if one exists (memory mapped files only) */
  int getMmap(void **Addr, off_t &Size);

  /* Preread file blocks into the file system cache */
  XrdSfsXferSize read(XrdSfsFileOffset offset, XrdSfsXferSize size);

  /* Read file bytes into a buffer */
  XrdSfsXferSize read(XrdSfsFileOffset offset, char *buffer,
                      XrdSfsXferSize size);

  /* Read file bytes using asychronous I/O */
  XrdSfsXferSize read(XrdSfsAio *aioparam);

  /* Write file bytes from a buffer */
  XrdSfsXferSize write(XrdSfsFileOffset offset, const char *buffer,
                       XrdSfsXferSize size);

  /* Write file bytes using asynchronous I/O */
  int write(XrdSfsAio *aioparam);

  /* Return state information on the file */
  int stat(struct stat *buf);

  /* Make sure all the outstanding data is actually written on the file (sync)
   */
  int sync();

  /* Make sure all outstanding data is actually written to the file (async) */
  int sync(XrdSfsAio *aiop);

  /* Truncate the file */
  int truncate(XrdSfsFileOffset fsize);

  /* Get compression information on the file */
  int getCXinfo(char cxtype[4], int &cxrsz);

  /* Constructor and Destructor */
  XrdFixedDataNodeFile(char *user, int MonID, const XrdFixedReplicator* replicator);
  ~XrdFixedDataNodeFile();

private:
  XrdSfsFile *m_nativeFile;
  XrdFixedReplicator* m_replicator;
  bool m_trackChanges;
}; // XrdFixedDataNodeFile

/*****************************************************************************/
/*                  C l a s s    X r d F i x e d                             */
/*****************************************************************************/
class XrdFixedDataNode : public XrdSfsFileSystem {
  friend class XrdFixedDataNodeFile;
  friend class XrdFixedDataNodeDirectory;

public:
  /* Object allocation */
  XrdSfsDirectory *newDir(char *user = 0, int monid = 0);
  XrdSfsFile *newFile(char *user = 0, int monid = 0);

  /* Change file mod settings */
  int chmod(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
            const XrdSecEntity *client = 0, const char *opaque = 0);

  /* File system control operation */
  int fsctl(const int cmd, const char *args, XrdOucErrInfo &eInfo,
            const XrdSecEntity *client = 0);

  /* Return statistical information */
  int getStats(char *buff, int blen);

  /* Get version string */
  const char *getVersion();

  /* Return directory/file existense information (short list)  */
  int exists(const char *path, XrdSfsFileExistence &eFlag, XrdOucErrInfo &eInfo,
             const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Create directory */
  int mkdir(const char *path, XrdSfsMode mode, XrdOucErrInfo &eInfo,
            const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Prepare a file for future processing */
  int prepare(XrdSfsPrep &pargs, XrdOucErrInfo &eInfo,
              const XrdSecEntity *client = 0);

  /* Remove a file */
  int rem(const char *path, XrdOucErrInfo &eInfo,
          const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Remove directory */
  int remdir(const char *path, XrdOucErrInfo &eInfo, const XrdSecEntity *client,
             const char *opaque);

  /* Remove directory */
  int rdir(const char *path, XrdOucErrInfo &eInfo,
           const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Rename a file or directory */
  int rename(const char *oPath, const char *nPath, XrdOucErrInfo &eInfo,
             const XrdSecEntity *client = 0, const char *opaqueO = 0,
             const char *opaqueN = 0);

  /* Return state information on file or a directory */
  int stat(const char *name, struct stat *buf, XrdOucErrInfo &eInfo,
           const XrdSecEntity *client = 0, const char *opaque = 0);
  int stat(const char *name, mode_t &mod, XrdOucErrInfo &eInfo,
           const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Truncate a file */
  int truncate(const char *path, XrdSfsFileOffset fsize, XrdOucErrInfo &eInfo,
               const XrdSecEntity *client = 0, const char *opaque = 0);

  /* Constructor and destructor */
  XrdFixedDataNode();
  virtual ~XrdFixedDataNode();

  /* Internal functions */
  void setNativeFS(XrdSfsFileSystem *nativeFS);
  void setWriteRedirector(XrdFixedRedirector *redirector);
  void setReplicator(XrdFixedReplicator *replicator);

  XrdFixedRedirector* getWriteRedirector();
  XrdFixedReplicator* getReplicator();

private:
  XrdSfsFileSystem *m_nativeFS;
  XrdFixedRedirector *m_writeRedirector;
  XrdFixedReplicator *m_replicator;
}; // XrdFixedDataNode

#endif
