#ifndef CONTRASTCONFIG_H
#define CONTRASTCONFIG_H

#include "BasePSConfig.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class ContrastConfig : public BasePSConfig
{
public:
    ContrastConfig();

    int read(char *buf, size_t size, off_t offset);
    int getSize();
    int write(const char *buf, size_t size, off_t offset);

    int L, R;

private:
    string genStr(int l, int r);

};

#endif // CONTRASTCONFIG_H
