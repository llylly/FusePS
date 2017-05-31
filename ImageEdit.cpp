#include "ImageEdit.h"
#include "Image.h"

#define EPS 1e-6

ImageEdit::ImageEdit()
{

}

Image *ImageEdit::brightAdjust(Image *img, int delta) {
    return ImageEdit::brightAdjust(img, delta, delta, delta);
}

Image *ImageEdit::brightAdjust(Image *img, int deltaR, int deltaG, int deltaB) {
    if ((img == NULL) || (img->height * img->width <= 0))
        return NULL;
    Image *ans = new Image();
    int h, w;
    h = ans->height = img->height;
    w = ans->width = img->width;
    ans->R = new int[h*w];
    ans->G = new int[h*w];
    ans->B = new int[h*w];
    for (int i=0; i<h*w; ++i) {
        ans->R[i] = img->R[i] + deltaR;
        ans->G[i] = img->G[i] + deltaG;
        ans->B[i] = img->B[i] + deltaB;
        if (ans->R[i] > 255) ans->R[i] = 255;
        if (ans->R[i] < 0) ans->R[i] = 0;
        if (ans->G[i] > 255) ans->G[i] = 255;
        if (ans->G[i] < 0) ans->G[i] = 0;
        if (ans->B[i] > 255) ans->B[i] = 255;
        if (ans->B[i] < 0) ans->B[i] = 0;
    }
    return ans;
}

Image *ImageEdit::contrastAdjust(Image *img, int minPoint, int maxPoint) {
    if ((img == NULL) || (img->height * img->width <= 0))
        return NULL;

    int *lookUpTable = new int[256];
    if (minPoint < 0) minPoint = 0;
    if (minPoint > 255) minPoint = 255;
    if (maxPoint < 0) maxPoint = 0;
    if (maxPoint > 255) maxPoint = 255;
    if (minPoint <= maxPoint) {
        for (int i=0; i<minPoint; ++i) lookUpTable[i] = 0;
        for (int i=maxPoint; i<256; ++i) lookUpTable[i] = 255;
        for (int i=minPoint; i<maxPoint; ++i)
            lookUpTable[i] = 255*(i-minPoint) / (maxPoint-minPoint);
    } else {
        for (int i=0; i<maxPoint; ++i) lookUpTable[i] = 255;
        for (int i=minPoint; i<256; ++i) lookUpTable[i] = 0;
        for (int i=maxPoint; i<minPoint; ++i)
            lookUpTable[i] = 255*(minPoint-i) / (minPoint-maxPoint);
    }

    Image *ans = new Image();
    int h, w;
    h = ans->height = img->height;
    w = ans->width = img->width;
    ans->R = new int[h*w];
    ans->G = new int[h*w];
    ans->B = new int[h*w];
    for (int i=0; i<h*w; ++i) {
        ans->R[i] = lookUpTable[img->R[i]];
        ans->G[i] = lookUpTable[img->G[i]];
        ans->B[i] = lookUpTable[img->B[i]];
    }

    delete[] lookUpTable;

    return ans;
}

Image *ImageEdit::gammaAdjust(Image *img, double gamma) {
    if ((img == NULL) || (img->height * img->width <= 0))
        return NULL;
    if (gamma < 0.0f) return NULL;

    int *lookUpTable = new int[256];
    for (int i=0; i<256; ++i) {
        lookUpTable[i] = (int)((double)255.0f * pow((double)(i) / 255.0f, 1.0f / gamma) + 0.5f);
        if (lookUpTable[i] > 255) lookUpTable[i] = 255;
        if (lookUpTable[i] < 0) lookUpTable[i] = 0;
    }

    Image *ans = new Image();
    int h, w;
    h = ans->height = img->height;
    w = ans->width = img->width;
    ans->R = new int[h*w];
    ans->G = new int[h*w];
    ans->B = new int[h*w];
    for (int i=0; i<h*w; ++i) {
        ans->R[i] = lookUpTable[img->R[i]];
        ans->G[i] = lookUpTable[img->G[i]];
        ans->B[i] = lookUpTable[img->B[i]];
    }

    delete[] lookUpTable;

    return ans;
}

Image *ImageEdit::histogramEqualization(Image *img) {
    if ((img == NULL) || (img->height * img->width <= 0))
        return NULL;
    img->calcHistogram();

    srand(time(0));

    int *lookR = new int[256];
    int *lookG = new int[256];
    int *lookB = new int[256];
    for (int i=0; i<256; ++i) {
        lookR[i] = int(img->histogram->sumArrR[i] * 255.0f + 0.5f);
        lookG[i] = int(img->histogram->sumArrG[i] * 255.0f + 0.5f);
        lookB[i] = int(img->histogram->sumArrB[i] * 255.0f + 0.5f);
        if (lookR[i] > 255) lookR[i] = 255;
        if (lookG[i] > 255) lookG[i] = 255;
        if (lookB[i] > 255) lookB[i] = 255;
    }

    Image *ans = new Image();
    int h, w;
    h = ans->height = img->height;
    w = ans->width = img->width;
    ans->R = new int[h*w];
    ans->G = new int[h*w];
    ans->B = new int[h*w];
    int tmp, tmpr;
    for (int i=0; i<h*w; ++i) {
        if (img->R[i] == 0) tmp = 0; else tmp = lookR[img->R[i]-1] + 1;
        tmpr = lookR[img->R[i]];
        if (tmp >= tmpr) ans->R[i] = tmpr; else
            ans->R[i] = tmp + rand() % (tmpr - tmp + 1);

        if (img->G[i] == 0) tmp = 0; else tmp = lookG[img->G[i]-1] + 1;
        tmpr = lookG[img->G[i]];
        if (tmp >= tmpr) ans->G[i] = tmpr; else
            ans->G[i] = tmp + rand() % (tmpr - tmp + 1);

        if (img->B[i] == 0) tmp = 0; else tmp = lookB[img->B[i]-1] + 1;
        tmpr = lookB[img->B[i]];
        if (tmp >= tmpr) ans->B[i] = tmpr; else
            ans->B[i] = tmp + rand() % (tmpr - tmp + 1);
    }

    delete[] lookR;
    delete[] lookG;
    delete[] lookB;

    return ans;
}

Image *ImageEdit::histogramStylize(Image *img, Image *refer) {
    if ((img == NULL) || (img->height * img->width <= 0))
        return NULL;
    if ((refer == NULL) || (refer->height * refer->width <= 0))
        return NULL;

    img->calcHistogram();
    refer->calcHistogram();

    srand(time(0));

    int *lookR = new int[256];
    int *lookG = new int[256];
    int *lookB = new int[256];

    int p = 0;
    double mer = 0.0f;
    for (int i=0; i<256; ++i) {
        mer = img->histogram->sumArrR[i];
        while ((p < 255) && (refer->histogram->sumArrR[p+1] <= mer + EPS)) ++p;
        lookR[i] = p;
    }
    p=0;
    for (int i=0; i<256; ++i) {
        mer = img->histogram->sumArrG[i];
        while ((p < 255) && (refer->histogram->sumArrG[p+1] <= mer + EPS)) ++p;
        lookG[i] = p;
    }
    p=0;
    for (int i=0; i<256; ++i) {
        mer = img->histogram->sumArrB[i];
        while ((p < 255) && (refer->histogram->sumArrB[p+1] <= mer + EPS)) ++p;
        lookB[i] = p;
    }

    Image *ans = new Image();
    int h, w;
    h = ans->height = img->height;
    w = ans->width = img->width;
    ans->R = new int[h*w];
    ans->G = new int[h*w];
    ans->B = new int[h*w];

    int tmp, tmpr;
    for (int i=0; i<h*w; ++i) {
        if (img->R[i] == 0) tmp = 0; else tmp = lookR[img->R[i]-1] + 1;
        tmpr = lookR[img->R[i]];
        if (tmp >= tmpr) ans->R[i] = tmpr; else
            ans->R[i] = tmp + rand() % (tmpr - tmp + 1);

        if (img->G[i] == 0) tmp = 0; else tmp = lookG[img->G[i]-1] + 1;
        tmpr = lookG[img->G[i]];
        if (tmp >= tmpr) ans->G[i] = tmpr; else
            ans->G[i] = tmp + rand() % (tmpr - tmp + 1);

        if (img->B[i] == 0) tmp = 0; else tmp = lookB[img->B[i]-1] + 1;
        tmpr = lookB[img->B[i]];
        if (tmp >= tmpr) ans->B[i] = tmpr; else
            ans->B[i] = tmp + rand() % (tmpr - tmp + 1);
    }

    delete[] lookR;
    delete[] lookG;
    delete[] lookB;

    return ans;
}

Image *ImageEdit::nearestNeighbor(Image *img, int width, int height) {
    if (width < 0 || width > Constants::ZOOM_LIMIT) return NULL;
    if (height < 0 || height > Constants::ZOOM_LIMIT) return NULL;
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;

    double rratio = (double)(img->height) / (double)(height + 1);
    double cratio = (double)(img->width) / (double)(width + 1);

    Image *ans = new Image();
    ans->height = height;
    ans->width = width;
    ans->R = new int[height * width];
    ans->G = new int[height * width];
    ans->B = new int[height * width];

    int rf, cf;
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            rf = (int)(rratio * (i+1));
            cf = (int)(cratio * (j+1));
            ans->R[I(i,j,width)] = img->R[I(rf,cf,img->width)];
            ans->G[I(i,j,width)] = img->G[I(rf,cf,img->width)];
            ans->B[I(i,j,width)] = img->B[I(rf,cf,img->width)];
        }

    return ans;
}

Image *ImageEdit::bilinear(Image *img, int width, int height) {
    if (width < 0 || width > Constants::ZOOM_LIMIT) return NULL;
    if (height < 0 || height > Constants::ZOOM_LIMIT) return NULL;
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;

    double rratio, cratio;

    if (height == 1) rratio = 0.0f; else
        rratio = (double)(img->height - 1) / (double)(height - 1);
    if (width == 1) cratio = 0.0f; else
        cratio = (double)(img->width - 1) / (double)(width - 1);

    Image *ans = new Image();
    ans->height = height;
    ans->width = width;
    ans->R = new int[height * width];
    ans->G = new int[height * width];
    ans->B = new int[height * width];

    double rf, cf;
    int ror, roc;
    int flr, flc;
    int cer, cec;
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            rf = rratio * i;
            cf = cratio * j;
            ror = int(rf + 0.5f), roc = int(cf + 0.5f);
            if (abs(rf-ror) < EPS) rf = ror-EPS;
            if (abs(cf-roc) < EPS) cf = roc-EPS;
            flr = int(rf), flc = int(cf);
            cer = flr + 1, cec = flc + 1;
            if (cer >= img->height) cer = img->height - 1;
            if (cec >= img->width) cec = img->width - 1;
            {
                double deltaR = rf-flr, deltaC = cf - flc;
                ans->R[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * img->R[I(flr, flc, img->width)] +
                              (1.0 - deltaR) * deltaC * img->R[I(flr, cec, img->width)] +
                              deltaR * (1.0 - deltaC) * img->R[I(cer, flc, img->width)] +
                              deltaR * deltaC * img->R[I(cer, cec, img->width)] +
                              0.5f);
                ans->G[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * img->G[I(flr, flc, img->width)] +
                              (1.0 - deltaR) * deltaC * img->G[I(flr, cec, img->width)] +
                              deltaR * (1.0 - deltaC) * img->G[I(cer, flc, img->width)] +
                              deltaR * deltaC * img->G[I(cer, cec, img->width)] +
                              0.5f);
                ans->B[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * img->B[I(flr, flc, img->width)] +
                              (1.0 - deltaR) * deltaC * img->B[I(flr, cec, img->width)] +
                              deltaR * (1.0 - deltaC) * img->B[I(cer, flc, img->width)] +
                              deltaR * deltaC * img->B[I(cer, cec, img->width)] +
                              0.5f);
                if (ans->R[I(i,j,width)] > 255) ans->R[I(i,j,width)] = 255;
                if (ans->G[I(i,j,width)] > 255) ans->G[I(i,j,width)] = 255;
                if (ans->B[I(i,j,width)] > 255) ans->B[I(i,j,width)] = 255;
            }
        }

    return ans;
}

Image *ImageEdit::bicubic(Image *img, int width, int height) {
    if (width < 0 || width > Constants::ZOOM_LIMIT) return NULL;
    if (height < 0 || height > Constants::ZOOM_LIMIT) return NULL;
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;

    double rratio, cratio;

    if (height == 1) rratio = 0.0f; else
        rratio = (double)(img->height - 1) / (double)(height - 1);
    if (width == 1) cratio = 0.0f; else
        cratio = (double)(img->width - 1) / (double)(width - 1);

    Image *ans = new Image();
    ans->height = height;
    ans->width = width;
    ans->R = new int[height * width];
    ans->G = new int[height * width];
    ans->B = new int[height * width];

    double rf, cf;
    int ror, roc;
    int flr, flc;
    int cer, cec;
    DoubleColor ap0, ap1, ap2, ap3;
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            rf = rratio * i;
            cf = cratio * j;
            ror = int(rf + 0.5f), roc = int(cf + 0.5f);
            if (abs(rf-ror) < EPS) rf = ror-EPS;
            if (abs(cf-roc) < EPS) cf = roc-EPS;
            flr = int(rf), flc = int(cf);
            cer = flr + 1, cec = flc + 1;
            if (cer >= img->height) cer = img->height - 1;
            if (cec >= img->width) cec = img->width - 1;
            {
                double deltaR = rf-flr, deltaC = cf - flc;
                ap0 = getApproxV(img, rf, cf, flr, flc),
                ap1 = getApproxV(img, rf, cf, flr, cec),
                ap2 = getApproxV(img, rf, cf, cer, flc),
                ap3 = getApproxV(img, rf, cf, cer, cec);
                ans->R[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * ap0.R +
                              (1.0 - deltaR) * deltaC * ap1.R +
                              deltaR * (1.0 - deltaC) * ap2.R +
                              deltaR * deltaC * ap3.R +
                              0.5f);
                ans->G[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * ap0.G +
                              (1.0 - deltaR) * deltaC * ap1.G +
                              deltaR * (1.0 - deltaC) * ap2.G +
                              deltaR * deltaC * ap3.G +
                              0.5f);
                ans->B[I(i,j,width)] =
                        (int)((1.0 - deltaR) * (1.0 - deltaC) * ap0.B +
                              (1.0 - deltaR) * deltaC * ap1.B +
                              deltaR * (1.0 - deltaC) * ap2.B +
                              deltaR * deltaC * ap3.B +
                              0.5f);
                if (ans->R[I(i,j,width)] > 255) ans->R[I(i,j,width)] = 255;
                if (ans->G[I(i,j,width)] > 255) ans->G[I(i,j,width)] = 255;
                if (ans->B[I(i,j,width)] > 255) ans->B[I(i,j,width)] = 255;
                if (ans->R[I(i,j,width)] < 0) ans->R[I(i,j,width)] = 0;
                if (ans->G[I(i,j,width)] < 0) ans->G[I(i,j,width)] = 0;
                if (ans->B[I(i,j,width)] < 0) ans->B[I(i,j,width)] = 0;
            }
        }

    return ans;
}

PSNRData ImageEdit::calcPSNR(Image *a, Image *b) {
    PSNRData ans;
    ans.avail = false;
    if ((a == NULL) || (b == NULL) || (a->height * a->width <= 0) || (b->height * b->width <= 0))
        return ans;
    if ((a->height != b->height) || (a->width != b->width))
        return ans;
    ans.RMSE = ans.GMSE = ans.BMSE = 0.0f;
    for (int i=0; i < a->height*a->width; ++i) {
        ans.RMSE += (a->R[i] - b->R[i]) * (a->R[i] - b->R[i]);
        ans.GMSE += (a->G[i] - b->G[i]) * (a->G[i] - b->G[i]);
        ans.BMSE += (a->B[i] - b->B[i]) * (a->B[i] - b->B[i]);
    }
    ans.RMSE = ans.RMSE / (double)(a->height * a->width);
    ans.GMSE = ans.GMSE / (double)(a->height * a->width);
    ans.BMSE = ans.BMSE / (double)(a->height * a->width);

    if (abs(ans.RMSE) < EPS)
        ans.RPSNR = INFINITY;
    else
        ans.RPSNR = 20.0f * (log(255.0f / sqrt(ans.RMSE)) / log(10.0f));
    if (abs(ans.GMSE) < EPS)
        ans.GPSNR = INFINITY;
    else
        ans.GPSNR = 20.0f * (log(255.0f / sqrt(ans.GMSE)) / log(10.0f));
    if (abs(ans.BMSE) < EPS)
        ans.BPSNR = INFINITY;
    else
        ans.BPSNR = 20.0f * (log(255.0f / sqrt(ans.BMSE)) / log(10.0f));
    ans.PSNR = (ans.RPSNR + ans.GPSNR + ans.BPSNR) / 3.0f;
    ans.avail = true;
    return ans;
}

Image* ImageEdit::uniformBlur(Image *img, int radius) {
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;
    if ((radius <= 0) || (radius > Constants::BLUR_LIMIT)) return NULL;

    --radius;

    Image *ans = new Image();
    int h, w;
    ans->height = h = img->height;
    ans->width = w = img->width;
    ans->R = new int[h * w];
    ans->G = new int[h * w];
    ans->B = new int[h * w];

    double ratio = 1.0f / (double)((radius * 2 + 1) * (radius * 2 + 1));
    int r, c;
    for (int i=0; i<h; ++i)
        for (int j=0; j<w; ++j) {
            double R=0.0f, G=0.0f, B=0.0f;
            for (int ii=i-radius; ii<=i+radius; ++ii)
                for (int jj=j-radius; jj<=j+radius; ++jj) {
                    r = adjustR(img, ii);
                    c = adjustC(img, jj);
                    R += (double)img->R[I(r,c,w)] * ratio;
                    G += (double)img->G[I(r,c,w)] * ratio;
                    B += (double)img->B[I(r,c,w)] * ratio;
                }
            ans->R[I(i,j,w)] = (int)(R + 0.5f);
            ans->G[I(i,j,w)] = (int)(G + 0.5f);
            ans->B[I(i,j,w)] = (int)(B + 0.5f);
        }

    return ans;
}

Image* ImageEdit::gaussianBlur(Image *img, int radius) {
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;
    if ((radius <= 0) || (radius > Constants::BLUR_LIMIT)) return NULL;

    --radius;

    Image *ans = new Image();
    int h, w;
    ans->height = h = img->height;
    ans->width = w = img->width;
    ans->R = new int[h * w];
    ans->G = new int[h * w];
    ans->B = new int[h * w];

    int n = 1 + (radius << 1);
    double *weigh = new double[n * n];
    double normal = 0.0f;
    if (radius)
        for (int i=0; i<n; ++i)
            for (int j=0; j<n; ++j) {
                weigh[I(i,j,n)] = exp(-((i-radius) * (i-radius) + (j-radius) * (j-radius)) / (2.0f * radius * radius));
                normal += weigh[I(i,j,n)];
            }
    else
        weigh[0] = normal = 1.0f;
    for (int i=0; i<n; ++i)
        for (int j=0; j<n; ++j)
            weigh[I(i,j,n)] /= normal;

    int r, c;
    for (int i=0; i<h; ++i)
        for (int j=0; j<w; ++j) {
            double R=0.0f, G=0.0f, B=0.0f;
            for (int ii=0; ii<n; ++ii)
                for (int jj=0; jj<n; ++jj) {
                    r = adjustR(img, i + ii - radius);
                    c = adjustC(img, j + jj - radius);
                    R += (double)img->R[I(r,c,w)] * weigh[I(ii,jj,n)];
                    G += (double)img->G[I(r,c,w)] * weigh[I(ii,jj,n)];
                    B += (double)img->B[I(r,c,w)] * weigh[I(ii,jj,n)];
                }
            ans->R[I(i,j,w)] = (int)(R + 0.5f);
            ans->G[I(i,j,w)] = (int)(G + 0.5f);
            ans->B[I(i,j,w)] = (int)(B + 0.5f);
            if (ans->R[I(i,j,w)] > 255) ans->R[I(i,j,w)] = 255;
            if (ans->G[I(i,j,w)] > 255) ans->G[I(i,j,w)] = 255;
            if (ans->B[I(i,j,w)] > 255) ans->B[I(i,j,w)] = 255;
        }

    delete[] weigh;

    return ans;
}

Image* ImageEdit::mosaicBlur(Image *img, int radius) {
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;
    if ((radius <= 0) || (radius > Constants::BLUR_LIMIT)) return NULL;

    Image *ans = new Image();
    int h, w;
    ans->height = h = img->height;
    ans->width = w = img->width;
    ans->R = new int[h * w];
    ans->G = new int[h * w];
    ans->B = new int[h * w];

    for (int i=0; i<(h+radius-1)/radius; ++i)
        for (int j=0; j<(w+radius-1)/radius; ++j) {
            double R=0.0f, G=0.0f, B=0.0f;
            int cnt=0;
            for (int ii=i*radius; ii<(i+1)*radius; ++ii)
                for (int jj=j*radius; jj<(j+1)*radius; ++jj)
                    if ((ii < img->height) && (jj < img->width)) {
                        R += img->R[I(ii,jj,w)];
                        G += img->G[I(ii,jj,w)];
                        B += img->B[I(ii,jj,w)];
                        ++cnt;
                    }
            int iR = int(R/cnt + 0.5f), iG = int(G/cnt + 0.5f), iB = int(B/cnt + 0.5f);
            for (int ii=i*radius; ii<(i+1)*radius; ++ii)
                for (int jj=j*radius; jj<(j+1)*radius; ++jj)
                    if ((ii < img->height) && (jj < img->width))
                        ans->R[I(ii,jj,w)] = iR,
                        ans->G[I(ii,jj,w)] = iG,
                        ans->B[I(ii,jj,w)] = iB;
        }

    return ans;
}

Image* ImageEdit::graying(Image *img) {
    if ((img == NULL) || (img->height * img->width <= 0)) return NULL;

    Image *ans = new Image();
    int h, w;
    ans->height = h = img->height;
    ans->width = w = img->width;
    ans->R = new int[h * w];
    ans->G = new int[h * w];
    ans->B = new int[h * w];

    for (int i=0; i<h*w; ++i) {
        int v = (img->R[i] + img->G[i] + img->B[i]) / 3;
        ans->R[i] = v;
        ans->G[i] = v;
        ans->B[i] = v;
    }

    return ans;
}

DoubleColor ImageEdit::getApproxV(Image *img, double r, double c, int refR, int refC) {
    DoubleColor ans(0.0f, 0.0f, 0.0f);
    if ((img == NULL) || (img->height * img->width <= 0)) return ans;
    if ((refR < 0) || (refR >= img->height)) return ans;
    if ((refC < 0) || (refC >= img->width)) return ans;
    DoubleColor rVec(0.0f, 0.0f, 0.0f), cVec(0.0f, 0.0f, 0.0f);
    if ((refR > 0) && (refR < img->height - 1)) {
        double d;
        d = (img->R[I(refR + 1, refC, img->width)] - img->R[I(refR - 1, refC, img->width)]);
        rVec.R = d / 2.0f;
        d = (img->G[I(refR + 1, refC, img->width)] - img->G[I(refR - 1, refC, img->width)]);
        rVec.G = d / 2.0f;
        d = (img->B[I(refR + 1, refC, img->width)] - img->B[I(refR - 1, refC, img->width)]);
        rVec.B = d / 2.0f;
    } else
    if ((refR == 0) && (img->height > 0)) {
        double d;
        d = img->R[I(refR + 1, refC, img->width)] - img->R[I(refR, refC, img->width)];
        rVec.R = d;
        d = img->G[I(refR + 1, refC, img->width)] - img->G[I(refR, refC, img->width)];
        rVec.G = d;
        d = img->B[I(refR + 1, refC, img->width)] - img->B[I(refR, refC, img->width)];
        rVec.B = d;
    } else
    if ((refR == img->height - 1) && (img->height > 0)) {
        double d;
        d = img->R[I(refR, refC, img->width)] - img->R[I(refR - 1, refC, img->width)];
        rVec.R = d;
        d = img->G[I(refR, refC, img->width)] - img->G[I(refR - 1, refC, img->width)];
        rVec.G = d;
        d = img->B[I(refR, refC, img->width)] - img->B[I(refR - 1, refC, img->width)];
        rVec.B = d;
    }

    if ((refC > 0) && (refC < img->width - 1)) {
        double d;
        d = (img->R[I(refR, refC + 1, img->width)] - img->R[I(refR, refC - 1, img->width)]);
        cVec.R = d / 2.0f;
        d = (img->G[I(refR, refC + 1, img->width)] - img->G[I(refR, refC - 1, img->width)]);
        cVec.G = d / 2.0f;
        d = (img->B[I(refR, refC + 1, img->width)] - img->B[I(refR, refC - 1, img->width)]);
        cVec.B = d / 2.0f;
    } else
    if ((refC == 0) && (img->width > 0)) {
        double d;
        d = img->R[I(refR, refC + 1, img->width)] - img->R[I(refR, refC, img->width)];
        cVec.R = d;
        d = img->G[I(refR, refC + 1, img->width)] - img->G[I(refR, refC, img->width)];
        cVec.G = d;
        d = img->B[I(refR, refC + 1, img->width)] - img->B[I(refR, refC, img->width)];
        cVec.B = d;
    } else
    if ((refC == img->width - 1) && (img->width > 0)) {
        double d;
        d = img->R[I(refR, refC, img->width)] - img->R[I(refR, refC - 1, img->width)];
        cVec.R = d;
        d = img->G[I(refR, refC, img->width)] - img->G[I(refR, refC - 1, img->width)];
        cVec.G = d;
        d = img->B[I(refR, refC, img->width)] - img->B[I(refR, refC - 1, img->width)];
        cVec.B = d;
    }
    ans.R = img->R[I(refR, refC, img->width)];
    ans.G = img->G[I(refR, refC, img->width)];
    ans.B = img->B[I(refR, refC, img->width)];
    ans.R += (r - refR) * rVec.R + (c - refC) * cVec.R;
    ans.G += (r - refR) * rVec.G + (c - refC) * cVec.G;
    ans.B += (r - refR) * rVec.B + (c - refC) * cVec.B;
    return ans;
}

int ImageEdit::adjustR(Image *img, int r) {
    int ans;
    if (r < 0) ans = -r; else ans = r;
    return ans % img->height;
}

int ImageEdit::adjustC(Image *img, int c) {
    int ans;
    if (c < 0) ans = -c; else ans = c;
    return ans % img->width;
}
