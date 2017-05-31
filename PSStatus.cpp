#include "PSStatus.h"

PSStatus::PSStatus() {
    foldersN = 6;
    foldersPath = new char*[foldersN]
        {"/PS/Graying", "/PS/Scaling", "/PS/Blur", "/PS/Mosaic", "/PS/Brightness", "/PS/Contrast"};
    foldersName = new char*[foldersN]
        {"Graying", "Scaling", "Blur", "Mosaic", "Brightness", "Contrast"};
    foldersConfig = new int[foldersN]
        {-1, 0, 1, 2, 3, 4};
    configsN = 5;
    configsPath = new char*[configsN]
        {"/PS/Scaling/config.txt", "/PS/Blur/config.txt", "/PS/Mosaic/config.txt",
        "/PS/Brightness/config.txt", "/PS/Contrast/config.txt"};
    configs = new BasePSConfig*[configsN] {
        new ScalingConfig(), new BlurConfig(), new MosaicConfig(),
        new BrightnessConfig(), new ContrastConfig()
        };
    stamp = 0;
    fileMap = new map<string, string>[foldersN];
    for (int i=0; i<foldersN; ++i)
        fileMap[i].clear();
}

int PSStatus::initBuffer(int *argc, char **argv[]) {
    bufferFolder = "PSBuf";
    bool find = false;
    int place = -1;
    for (int i=0; i<(*argc) - 1; ++i) {
        if (strcmp((*argv)[i], "-PSBuf") == 0) {
            place = i;
            find = true;
            break;
        }
    }
    if (find) {
        bufferFolder = new char[strlen((*argv)[place + 1]) + 1];
        strcpy(bufferFolder, (*argv)[place + 1]);
    }

    mkdir(bufferFolder, S_IRWXU | S_IRWXG | S_IRWXO);

    {
        if (find) {
            char **newArgv = new char*[*argc - 2];
            for (int i=0; i < place; ++i) {
                newArgv[i] = new char[strlen((*argv)[i]) + 1];
                strcpy(newArgv[i], (*argv)[i]);
            }
            for (int i=place; i < (*argc) - 2; ++i) {
                newArgv[i] = new char[strlen((*argv)[i + 2]) + 1];
                strcpy(newArgv[i], (*argv)[i + 2]);
            }
            *argc -= 2;
            argv = &newArgv;
        }
        return 0;
    }
}
