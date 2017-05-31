#include "Histogram.h"

const int Histogram::R = 1;
const int Histogram::G = 2;
const int Histogram::B = 3;
const int Histogram::sumR = 4;
const int Histogram::sumG = 5;
const int Histogram::sumB = 6;
const int Histogram::maxV = SLOTS - 1;

Histogram::Histogram()
{
    memset(arrR, 0, sizeof(arrR));
    memset(arrG, 0, sizeof(arrG));
    memset(arrB, 0, sizeof(arrB));
    memset(sumArrR, 0, sizeof(sumArrR));
    memset(sumArrG, 0, sizeof(sumArrG));
    memset(sumArrB, 0, sizeof(sumArrB));
}

double Histogram::getMax(int type) {
    const double *arr = NULL;
    switch (type) {
        case R: arr = arrR; break;
        case G: arr = arrG; break;
        case B: arr = arrB; break;
        case sumR: arr = sumArrR; break;
        case sumG: arr = sumArrG; break;
        case sumB: arr = sumArrB; break;
    }
    if (arr == NULL) return 0.0f;
    double ans = 0.0f;
    for (int i=0; i<SLOTS; ++i)
        if (arr[i] > ans) ans = arr[i];
    return ans;
}

double Histogram::getValue(int type, double ind) {
    const double *arr = NULL;
    switch (type) {
        case R: arr = arrR; break;
        case G: arr = arrG; break;
        case B: arr = arrB; break;
        case sumR: arr = sumArrR; break;
        case sumG: arr = sumArrG; break;
        case sumB: arr = sumArrB; break;
    }
    if (arr == NULL) return NAN;
    if (ind <= 0.0f) return arr[0];
    if (ind >= (double)(SLOTS-1)) return arr[SLOTS-1];
    int fInd = int(floor(ind));
    double w = (double)(fInd + 1) - ind;
    return w * arr[fInd] + (1 - w) * arr[fInd + 1];
}

void Histogram::divide(int type, int dividend) {
    double *arr = NULL;
    switch (type) {
        case R: arr = arrR; break;
        case G: arr = arrG; break;
        case B: arr = arrB; break;
        case sumR: arr = sumArrR; break;
        case sumG: arr = sumArrG; break;
        case sumB: arr = sumArrB; break;
    }
    if (arr == NULL) return;
    for (int i=0; i<SLOTS; ++i)
        arr[i] /= (double)dividend;
}
