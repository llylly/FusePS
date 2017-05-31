#ifndef IMAGEEDIT_H
#define IMAGEEDIT_H

#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Constants.h"

class Image;

struct DoubleColor {
    double R, G, B;
    DoubleColor() { }
    DoubleColor(double r, double g, double b): R(r), G(g), B(b) { }
};

struct PSNRData {
    bool avail;
    int width, height;
    double RMSE;
    double GMSE;
    double BMSE;
    double RPSNR;
    double GPSNR;
    double BPSNR;
    double PSNR;
};

class ImageEdit
{
public:
    ImageEdit();

    static Image *brightAdjust(Image *img, int delta);
    static Image *brightAdjust(Image *img, int deltaR, int deltaG, int deltaB);
    static Image *contrastAdjust(Image *img, int minPoint, int maxPoint);
    static Image *gammaAdjust(Image *img, double gamma);
    static Image *histogramEqualization(Image *img);
    static Image *histogramStylize(Image *img, Image *refer);
    static Image *nearestNeighbor(Image *img, int width, int height);
    static Image *bilinear(Image *img, int width, int height);
    static Image *bicubic(Image *img, int width, int height);
    static PSNRData calcPSNR(Image *a, Image *b);
    static Image* uniformBlur(Image *img, int radius);
    static Image* gaussianBlur(Image *img, int radius);
    static Image* mosaicBlur(Image *img, int radius);

    static Image* graying(Image *img);

private:
    static DoubleColor getApproxV(Image *img, double r, double c, int refR, int refC);
    static int adjustR(Image *img, int r);
    static int adjustC(Image *img, int c);
};

#endif // IMAGEEDIT_H
