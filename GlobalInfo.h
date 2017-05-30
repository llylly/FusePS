#ifndef GLOBALINFO_H
#define GLOBALINFO_H

#include "PSStatus.h"

/**
 * @brief The GlobalInfo class
 * A class storing the global status when executing
 */
class GlobalInfo
{
public:
    GlobalInfo();
    PSStatus *psStatus;
};

#endif // GLOBALINFO_H
