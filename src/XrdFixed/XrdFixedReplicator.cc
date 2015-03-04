/******************************************************************************/
/*                                                                            */
/*                    X r d F i x e d R e p o r t e r. c c                    */
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

#include <fcntl.h>
#include <sstream>

#include "XrdOuc/XrdOucEnv.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdFixed/XrdFixedReplicator.hh"

/*****************************************************************************/
/*          X r d F i x e d R e p l i c a t o r   c t o r                    */
/*****************************************************************************/
XrdFixedReplicator::XrdFixedReplicator(const char* ConfigFN, XrdSysError& FixedEroute,
                                   XrdOucTrace& FixedTrace) :
    Eroute(FixedEroute),
    Trace(FixedTrace) {

    XrdOucEnv myEnv;
    XrdOucStream Config(NULL, getenv("XRDINSTANCE"), &myEnv, "=====> ");
    int cfgFD;
    setReplication(false);

    if (!ConfigFN || !*ConfigFN)
        FixedEroute.Emsg("Config", "Configuration file not specified");
    else {
        /* Try to open a config file */
        if ((cfgFD = open(ConfigFN, O_RDONLY, 0)) <= 0) {
            FixedEroute.Emsg("Config", errno, "open config file ", ConfigFN);
            return;
        }
        Config.Attach(cfgFD);

        /* Iterate over config file */
        char* var;
        while ((var = Config.GetMyFirstWord())) {
            if (strcmp(XRD_FIXED_CONF_OPTION_REPLICATION, var) == 0) {
                if (!(var = Config.GetWord())) {
                    FixedEroute.Emsg("Config", "Replication option argument missing");
                    setReplication(false);
                }

                if(strcmp(XRD_FIXED_CONF_OPTION_REPLICATION_ENABLED_VALUE, var) == 0) {
                    FixedEroute.Emsg("Config", "enabling file replication for ", XRD_FIXED_TRACK_FNAME, " files");                   
                    setReplication(true);
                }
                else if (strcmp(XRD_FIXED_CONF_OPTION_REPLICATION_DISABLED_VALUE, var) == 0)
                    setReplication(false);
                else {
                    FixedEroute.Emsg("Config", "Invalid replication option argument: ", var);
                    return;
                }
            }
        }
    }
}

XrdFixedReplicator::XrdFixedReplicator(const XrdFixedReplicator& other) :
 Eroute(other.Eroute),
 Trace(other.Trace),
 m_replicationEnabled(other.m_replicationEnabled) {};

/*****************************************************************************/
/*          X r d F i x e d R e p l i c a t o r   m e t h o d s              */
/*****************************************************************************/

/* Updates the entry for a given offset in an internal map */
void XrdFixedReplicator::write(XrdSfsFileOffset offset, XrdSfsXferSize size) {
    if (m_dirtyMap[offset] < size)
        m_dirtyMap[offset] = size;
}

/* Flushes out the internal map of updates */
void XrdFixedReplicator::close() {
    Eroute.Emsg("Replicator", "close");
    for (std::map<XrdSfsFileOffset, XrdSfsXferSize>::iterator it = m_dirtyMap.begin();
            it != m_dirtyMap.end(); ++it) {
        std::ostringstream convert;
        convert << it->first << " " << it->second;
        Eroute.Emsg("Replicator", convert.str().c_str());
    }
}

/* Getters and setters */
void XrdFixedReplicator::setReplication(bool b)
    { m_replicationEnabled = b;  }
bool XrdFixedReplicator::getReplication()
    { return m_replicationEnabled; }

