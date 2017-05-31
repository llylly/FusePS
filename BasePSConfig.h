#ifndef BASEPSCONFIG_H
#define BASEPSCONFIG_H

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <string>
#include <iostream>

using namespace std;

class BasePSConfig {
public:
    BasePSConfig();
    virtual int read(char *buf, size_t size, off_t offset) = 0;
    virtual int getSize() = 0;
    virtual int write(const char *buf, size_t size, off_t offset) = 0;

protected:
    string str;
};

#endif // BASEPSCONFIG_H
