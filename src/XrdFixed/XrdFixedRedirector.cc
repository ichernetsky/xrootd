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
#include <assert.h>
#include <cstdio>

#include "XrdOuc/XrdOucEnv.hh"
#include "XrdOuc/XrdOucStream.hh"
#include "XrdFixed/XrdFixedRedirector.hh"


XrdFixedNode::XrdFixedNode()
   : port(0), isPortSet_(false)
{
    memset(hostname, '\0', sizeof(hostname));
    memset(portStr, '\0', sizeof(portStr));
}
XrdFixedNode::XrdFixedNode(const char* Hostname)
    : port(0), isPortSet_(false)
{
    strncpy(hostname, Hostname, XRD_FIXED_MAX_HOSTNAME_LEN + 1);
    hostname[XRD_FIXED_MAX_HOSTNAME_LEN] = '\0';
    memset(portStr, '\0', sizeof(portStr));
}

XrdFixedNode::XrdFixedNode(const char* Hostname, const char* PortStr, int Port)
    : port(Port), isPortSet_(true)
{
    strncpy(hostname, Hostname, XRD_FIXED_MAX_HOSTNAME_LEN + 1);
    hostname[XRD_FIXED_MAX_HOSTNAME_LEN] = '\0';
    strncpy(portStr, PortStr, XRD_FIXED_MAX_PORT_LEN + 1);
    portStr[XRD_FIXED_MAX_PORT_LEN] = '\0';
}

const char* XrdFixedNode::getHostname() const {
    return hostname;
}

const char* XrdFixedNode::getPort() const {
    if (isPortSet_) return portStr;
    return XRD_FIXED_DEFAULT_PORT;
}

int XrdFixedNode::getNumericPort() const
{
    if (isPortSet_) return port;
    return XRD_FIXED_DEFAULT_N_PORT;
}

void XrdFixedNode::setPort(const char* PortStr, int Port)
{
    strncpy(portStr, PortStr, XRD_FIXED_MAX_PORT_LEN + 1);
    portStr[XRD_FIXED_MAX_PORT_LEN] = '\0';
    port = Port;
    isPortSet_ = true;
}

bool XrdFixedNode::isPortSet() const
{
    return isPortSet_;
}

XrdFixedNode::~XrdFixedNode() {
}

/*****************************************************************************/
/*      X r d F i x e d R e d i r e c t o r  c o n s t r u c t o r           */
/*****************************************************************************/
XrdFixedRedirector::XrdFixedRedirector(const char* ConfigFN,
                                       XrdSysError& FixedEroute,
                                       XrdOucTrace& FixedTrace) :
    Eroute(FixedEroute),
    Trace(FixedTrace) {

    char msg[2048] = {0};

    XrdOucEnv myEnv;
    XrdOucStream Config(NULL, getenv("XRDINSTANCE"), &myEnv, "=====> ");
    int cfgFD = 0;
    char *var = NULL;

    char hostname[XRD_FIXED_MAX_HOSTNAME_LEN + 1] = {0};
    const char *portPtr = NULL;
    int port = 0;

    nodeCount = 0;
    defaultPortStr[0] = '\0';
    defaultPort = 0;

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
                    return;
                }

                /* We are intersted only in the simplest form: xrd.port N  */
                if (Config.GetWord()) continue;

                defaultPort = atoi(var);
                if (defaultPort < 1 || defaultPort > 65535) return;

                strncpy(defaultPortStr, var, XRD_FIXED_MAX_PORT_LEN + 1);
                defaultPortStr[XRD_FIXED_MAX_PORT_LEN] = '\0';
            }

            if (strncmp(var, "fixed.hosts", 11) == 0) {

                if (!(var = Config.GetWord())) {
                    FixedEroute.Emsg("Config", "Number of servers not specified for fixed.hosts");
                    return;
                }

                /* Get the number of servers */
                nodeCount = atoi(var);
                if (nodeCount < 1 || nodeCount > XRD_FIXED_MAX_CLUSTER_SIZE) {
                    snprintf(msg, sizeof(msg), "number of servers for fixed.hosts should be between 1 - %d",
                             XRD_FIXED_MAX_CLUSTER_SIZE);
                    FixedEroute.Emsg("Config", msg);
                    return;
                }

                int i = nodeCount;
                /* Iterate over the list of nodes */
                while (i > 0) {
                    if (!(var = Config.GetWord()))
                        break;

                    unsigned int nHost = strlen(var);
                    portPtr = strrchr(var, ':');
                    if (portPtr != NULL) {
                        portPtr++;
                        nHost -= strlen(portPtr) + 1;

                        port = atoi(portPtr);
                        if (port < 1 || port > 65535) {
                            snprintf(msg, sizeof(msg), "Invalid xrd.port value: %s", portPtr);
                            FixedEroute.Emsg("Config", msg);
                            return;
                        }
                    }

                    if (nHost > XRD_FIXED_MAX_HOSTNAME_LEN) {
                        FixedEroute.Emsg("Config", "fixed.hosts hostname too long");
                        nodeCount = 0;
                        return;
                    }

                    memmove(hostname, var, nHost);
                    hostname[nHost] = '\0';

                    if (portPtr) {
                        snprintf(msg, sizeof(msg), "Adding node: %s:%d", hostname, port);
                        FixedEroute.Emsg("Config", msg);
                        nodes[nodeCount - i] = XrdFixedNode(hostname, portPtr, port);
                    }
                    else {
                        snprintf(msg, sizeof(msg), "Adding node: %s", hostname);
                        FixedEroute.Emsg("Config", msg);
                        nodes[nodeCount - i] = XrdFixedNode(hostname);
                    }

                    --i;
                }

                if (i != 0) {
                    FixedEroute.Emsg("Config", "fixed.hosts server number mismatch");
                    nodeCount = 0;
                    return;
                }
            }
        }

        if (defaultPort != 0) {
            snprintf(msg, sizeof(msg), "Default port: %d", defaultPort);
            FixedEroute.Emsg("Config", msg);

            for (int i = 0; i < nodeCount; i++) {
                if (!nodes[i].isPortSet()) {
                    snprintf(msg, sizeof(msg), "Setting default port for %s: %d",
                             nodes[i].getHostname(), defaultPort);
                    FixedEroute.Emsg("Config", msg);
                    nodes[i].setPort(defaultPortStr, defaultPort);
                }
            }
        }
    }
}

XrdFixedRedirector::~XrdFixedRedirector() {
}

/* Given a file name return a node name where the file should be created */
const XrdFixedNode XrdFixedRedirector::getNode(const char* path) {
    unsigned char result[MD5_DIGEST_LENGTH];
    char normPath[XRD_FIXED_MAX_URL_LEN] = {0};
    unsigned int normPathLen = XRD_FIXED_MAX_URL_LEN;

    normalizePath(path, normPath, &normPathLen);
    MD5((const unsigned char*) normPath, normPathLen, result);

    /* use the first byte of the checksum to see which node to choose */
    int nodeIndex = result[0] % nodeCount;
    Eroute.Emsg("XrdFixedRedirector", path, normPath,
                nodes[nodeIndex].getHostname());
    return nodes[nodeIndex];
}

void XrdFixedRedirector::normalizePath(const char* path, char* normalizedPath, unsigned int* size) {
    unsigned int len = strlen(path);
    assert(*size >= len);

    unsigned int i = 0;
    unsigned int j = 0;

    while (i < len) {
        normalizedPath[i] = path[j];
        ++j;

        /* advance i while removing duplicate occurences of '/' */
        if (path[i] != '/')
            i++;
        else {
            while (i < len && path[i] == '/')
                ++i;
        }
    }

    /* strip '/' at the end */
    while ((j > 1) && (normalizedPath[j] == '/'))
        --j;

    normalizedPath[j] = '\0';
    *size = j;
}

/* Return a number of available nodes */
int XrdFixedRedirector::getNodeCount() {
    return nodeCount;
}
