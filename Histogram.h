#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#define SLOTS 256

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <QObject>

class Histogram
{
public:
    static const int R, G, B, sumR, sumG, sumB, maxV;

    Histogram();
    double arrR[SLOTS], arrG[SLOTS], arrB[SLOTS];
    double sumArrR[SLOTS], sumArrG[SLOTS], sumArrB[SLOTS];

    double getMax(int type);
    double getValue(int type, double ind);
    void divide(int type, int dividend);
};

#endif // HISTOGRAM_H
