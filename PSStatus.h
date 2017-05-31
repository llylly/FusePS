#ifndef PSSTATUS_H
#define PSSTATUS_H

#include <string>
#include <sys/stat.h>
#include <map>

#include "BasePSConfig.h"
#include "ScalingConfig.h"
#include "BlurConfig.h"
#include "BrightnessConfig.h"
#include "ContrastConfig.h"
#include "MosaicConfig.h"

#define MAX_CONFIG_LEN 200

using namespace std;

class PSStatus
{
public:
    int foldersN;
    char **foldersPath;
    char **foldersName;
    int *foldersConfig;
    int configsN;
    char **configsPath;
    map<string, string> *fileMap;

    BasePSConfig **configs;

    PSStatus();

    /**
     * @brief initBuffer
     * @param argc
     * @param argv
     * Find "-PSBuf" to determine the buffer used by PS module
     * Default buffer is: "PSBuf" folder
     * It also refreshes argc & argv to delete "-PSBuf" argument
     */
    int initBuffer(int *argc, char **argv[]);

    char *bufferFolder;

private:
    int stamp;
};

#endif // PSSTATUS_H
