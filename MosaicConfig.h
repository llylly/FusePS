#ifndef MOSAICCONFIG_H
#define MOSAICCONFIG_H

#include "BasePSConfig.h"
#include <sstream>

class MosaicConfig : public BasePSConfig
{
public:
    MosaicConfig();

    int read(char *buf, size_t size, off_t offset);
    int getSize();
    int write(const char *buf, size_t size, off_t offset);

private:
    string genStr(int radius);

    int radius;
};

#endif // MOSAICCONFIG_H
