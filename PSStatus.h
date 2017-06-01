#ifndef PSSTATUS_H
#define PSSTATUS_H

#include <string>
#include <sys/stat.h>
#include <map>

#include <sys/stat.h>
#include <unistd.h>

#include <QImage>

#include "Image.h"

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
    BasePSConfig **configs;
    map<string, string> *fileMap;
    map<string, bool> *baked;
    int stamp;

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

    int work(string path, int typeNo);

    char *bufferFolder;
};

#endif // PSSTATUS_H
