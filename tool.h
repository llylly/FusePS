#ifndef TOOL_H
#define TOOL_H

#define FUSE_USE_VERSION 26

#include "fuse.h"
#include "sys/stat.h"
#include <cstdlib>

#include "GlobalInfo.h"
#include "PSStatus.h"

PSStatus *getPSStatus();

struct stat *genPSFolderStat();

struct stat *genPSConfigFileStat();

bool legalImgName(const char *fileName);

#endif // TOOL_H
