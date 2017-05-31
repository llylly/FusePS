#ifndef BLURCONFIG_H
#define BLURCONFIG_H

#include "BasePSConfig.h"
#include <string>
#include <sstream>

using namespace std;

class BlurConfig : public BasePSConfig
{
public:
    BlurConfig();

    int read(char *buf, size_t size, off_t offset);
    int getSize();
    int write(const char *buf, size_t size, off_t offset);

    int radius;

private:
    string genStr(int radius);
};


#endif // BLURCONFIG_H
