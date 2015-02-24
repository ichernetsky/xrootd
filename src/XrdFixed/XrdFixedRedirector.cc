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

#include <openssl/md5.h>
#include <string.h>
#include <fcntl.h>

#include "XrdOuc/XrdOucEnv.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdFixed/XrdFixedRedirector.hh"


/*****************************************************************************/
/*      X r d F i x e d R e d i r e c t o r  c o n s t r u c t o r           */
/*****************************************************************************/
XrdFixedRedirector::XrdFixedRedirector(const char* ConfigFN,
                                       XrdSysError& FixedEroute,
                                       XrdOucTrace& FixedTrace) :
    Eroute(FixedEroute),
    Trace(FixedTrace) {

    XrdOucEnv myEnv;
    XrdOucStream Config(NULL, getenv("XRDINSTANCE"), &myEnv, "=====> ");
    int cfgFD;
    char *var;
    n_port = 0;

    if (!ConfigFN || !*ConfigFN)
        FixedEroute.Emsg("Config", "Configuration file not specified");
    else {
        /* Try to open a config file */
        if ((cfgFD = open(ConfigFN, O_RDONLY, 0)) <= 0) {
            FixedEroute.Emsg("Config", errno, "open config file ", ConfigFN);

            return;
        }
        Config.Attach(cfgFD);

        /* Read the data server list */
        while ((var = Config.GetMyFirstWord())) {
            if (strncmp(var, "xrd.port", 8) == 0) {
                if (!(var = Config.GetWord())) {
                    FixedEroute.Emsg("Config", "xrd.port value missing");
                    str_port[0] = '\0';
                    n_port = 0;
                    return;
                }

                n_port = strtol(var, NULL, 10);
                if (n_port > 65535) {
                    FixedEroute.Emsg("Config", "Invalid xrd.port value");
                    str_port[0] = '\0';
                    n_port = 0;
                    return;
                }

                strncpy(str_port, var, 5);
            }
            if (strncmp(var, "fixed.hosts", 11) == 0) {

                if (!(var = Config.GetWord())) {
                    FixedEroute.Emsg("Config", "Number of servers not specified for fixed.hosts");
                    nNodes = 0;
                    return;
                }

                /* Get the number of servers */
                nNodes = strtol(var, NULL, 10);
                if (nNodes < 1 || nNodes > 63) {
                    FixedEroute.Emsg("Config", "number of servers for fixed.hosts should be between 1 - 63");
                    return;
                }

                long i = nNodes;
                /* Iterate over the list of nodes */
                while (i > 0) {
                    if (!(var = Config.GetWord()))
                        break;

                    unsigned int nHost = strlen(var);
                    if (nHost > XRD_FIXED_MAX_HOSTNAME_LEN) {
                        FixedEroute.Emsg("Config", "fixed.hosts hostname too long");
                        nNodes = 0;
                        return;
                    }
                    strncpy(nodes[nNodes - i], var, XRD_FIXED_MAX_HOSTNAME_LEN + 1);
                    FixedEroute.Emsg("Config", "Got", nodes[nNodes - i]);
                    --i;
                }

                if (i != 0) {
                    FixedEroute.Emsg("Config", "fixed.hosts server number mismatch");
                    nNodes = 0;
                    return;
                }
            }
        }

    }
    if (str_port == NULL) {
        strncpy(str_port, XRD_FIXED_DEFAULT_PORT, XRD_FIXED_DEFAULT_PORT_LEN);
        n_port = XRD_FIXED_DEFAULT_N_PORT;
    }
}

XrdFixedRedirector::~XrdFixedRedirector() {
}

/* Given a file name return a node name where the file should be created */
const char* XrdFixedRedirector::node(const char* path) {
    unsigned char result[MD5_DIGEST_LENGTH];

    unsigned int len = strlen(path);

    // Strip all '/' from the end
    while ((len > 1) && (path[len - 1] == '/'))
        --len;

    MD5((const unsigned char*) path, len, result);

    /* use the first byte of the checksum to see which node to choose */
    unsigned int nodeIndex = result[0] % nNodes;
    Eroute.Emsg("XrdFixedRedirector", path, nodes[nodeIndex]);
    return nodes[nodeIndex];
}

const char* XrdFixedRedirector::getPort() {
    return str_port;
}

unsigned int XrdFixedRedirector::getNumericPort() {
    return n_port;
}

/* Return a number of available nodes */
long XrdFixedRedirector::getnNodes() {
    return nNodes;
}


