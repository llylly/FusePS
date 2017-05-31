#ifndef BRIGHTNESSCONFIG_H
#define BRIGHTNESSCONFIG_H

#include "BasePSConfig.h"
#include <sstream>

class BrightnessConfig : public BasePSConfig
{
public:
    BrightnessConfig();

    int read(char *buf, size_t size, off_t offset);
    int getSize();
    int write(const char *buf, size_t size, off_t offset);

private:
    string genStr(double gamma);

    double gamma;
};

#endif // BRIGHTNESSCONFIG_H
