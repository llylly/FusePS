#include "PSStatus.h"

PSStatus::PSStatus() {
    foldersN = 5;
    foldersPath = new char*[foldersN]
        {"/PS/Graying", "/PS/Scaling", "/PS/Blur", "/PS/Brightness", "/PS/Contrast"};
    foldersName = new char*[foldersN]
        {"Graying", "Scaling", "Blur", "Brightness", "Contrast"};
    configsN = 4;
    configsPath = new char*[configsN]
        {"/PS/Scaling/config.txt", "/PS/Blur/config.txt",
        "/PS/Brightness/config.txt", "/PS/Contrast/config.txt"};
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
    int ret;
    if (ret = mkdir(bufferFolder, S_IRWXU | S_IRWXG | S_IRWXO)) {
        return ret;
    } else {
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
