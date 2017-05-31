#ifndef SCALINGCONFIG_H
#define SCALINGCONFIG_H

#include "BasePSConfig.h"
#include <sstream>
#include <string>

using namespace std;

enum SCALE_TYPE {
    RATIO, ABSOLUTE
};

class ScalingConfig : public BasePSConfig
{
public:
    ScalingConfig();

    int read(char *buf, size_t size, off_t offset);
    int getSize();
    int write(const char *buf, size_t size, off_t offset);

    SCALE_TYPE type;
    int absoluteW, absoluteH;
    double ratioW, ratioH;

private:
    string genStr(SCALE_TYPE type, int absoluteW, int absoluteH,
                  double ratioW, double ratioH);
};

#endif // SCALINGCONFIG_H
