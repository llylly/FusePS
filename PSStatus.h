#ifndef PSSTATUS_H
#define PSSTATUS_H

#include <string>
#include <sys/stat.h>

#define MAX_CONFIG_LEN 200

class PSStatus
{
public:
    int foldersN;
    char **foldersPath;
    char **foldersName;
    int configsN;
    char **configsPath;

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

};

#endif // PSSTATUS_H
