#include "Image.h"

ImageEntry *Image::head = NULL;
ImageEntry *Image::tail = NULL;

const int Image::W = 2;
const double Image::threshold = 100.0;

void Image::addImage(Image* img) {
    if ((img == NULL) || (img->height * img->width <= 0)) return;
    if (Image::head == NULL) {
        Image::head = Image::tail = new ImageEntry();
        Image::tail->value = img;
    } else {
        ImageEntry *newEntry = new ImageEntry();
        newEntry->prev = Image::tail;
        Image::tail = newEntry;
        newEntry->value = img;
    }
}

Image *Image::getCurImage() {
    if (Image::tail != NULL) return Image::tail->value; else return NULL;
}

void Image::deleteEntry() {
    if (Image::tail != NULL) {
        if (Image::tail != Image::head) {
            ImageEntry *tmp = Image::tail->prev;
            delete Image::tail;
            Image::tail = tmp;
        } else {
            delete Image::tail;
            Image::head = Image::tail = NULL;
        }
    }
}

ImageEntry *Image::getCurEntry() {
    return Image::tail;
}

Image *Image::fromQImage(QImage *qimg) {
    Image *img = new Image();
    int h, w;
    img->height = h = qimg->height();
    img->width = w = qimg->width();
    img->R = new int[h * w];
    img->G = new int[h * w];
    img->B = new int[h * w];
    for (int i=0; i<img->height; ++i)
        for (int j=0; j<img->width; ++j) {
            img->R[I(i,j,w)] = qRed(qimg->pixel(j,i));
            img->G[I(i,j,w)] = qGreen(qimg->pixel(j,i));
            img->B[I(i,j,w)] = qBlue(qimg->pixel(j,i));
        }
    return img;
}

Image *Image::fromImage(Image* img) {
    Image *n = new Image();
    n->height = img->height;
    n->width = img->width;
    n->R = new int[n->height * n->width];
    n->G = new int[n->height * n->width];
    n->B = new int[n->height * n->width];
    for (int i=0; i<img->height; ++i)
        for (int j=0; j<img->width; ++j) {
            n->R[I(i,j,n->width)] = img->R[I(i,j,img->width)];
            n->G[I(i,j,n->width)] = img->G[I(i,j,img->width)];
            n->B[I(i,j,n->width)] = img->B[I(i,j,img->width)];
        }
    return n;
}

uchar *Image::toUcharArr() {
    if (height*width == 0) return NULL;
    QRgb *arr = new QRgb[height * width];
    for (int i=0; i<height*width; ++i) {
        arr[i] = qRgb(R[i], G[i], B[i]);
    }
    return (uchar*)arr;
}

QImage *Image::toQImage() {
    if (height*width == 0) return NULL;
    QImage *qimg = new QImage(width, height, QImage::Format_RGB32);
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            qimg->setPixelColor(j, i, QColor(R[I(i,j,width)], G[I(i,j,width)], B[I(i,j,width)]));
        }
    return qimg;
}

QPixmap *Image::toQPixmap() {
    if (height*width == 0) return NULL;
    QImage *qimg;
    QPixmap *qpixmap = new QPixmap(QPixmap::fromImage(*(qimg=toQImage())));
    delete qimg;
    return qpixmap;
}

void Image::save(string path) {
    QImage *image = this->toQImage();
    image->save(QString::fromStdString(path));
    delete image;
}

void Image::printImg() {
    QRgb *arr = (QRgb*)toUcharArr();
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            printf("(%d,%d,%d,%d,%d) ", i, j, qRed(arr[I(i,j,width)]), qGreen(arr[I(i,j,width)]), qBlue(arr[I(i,j,width)]));
        }
        printf("\n");
    }
    delete[] arr;
}

void Image::calcHistogram(bool recalc) {
    if (recalc == true) {
        if (histogramAvailable) {
            histogramAvailable = false;
            delete histogram;
        }
    }
    if (this->histogramAvailable == false) {
        if (height * width > 0) {
            histogram = new Histogram();
            for (int i=0; i<height*width; ++i) {
                histogram->arrR[R[i]]+=1.0f;
                histogram->arrG[G[i]]+=1.0f;
                histogram->arrB[B[i]]+=1.0f;
            }
            histogram->sumArrR[0] = histogram->arrR[0];
            histogram->sumArrG[0] = histogram->arrG[0];
            histogram->sumArrB[0] = histogram->arrB[0];
            for (int i=0; i<Histogram::maxV; ++i) {
                histogram->sumArrR[i+1] = histogram->sumArrR[i] + histogram->arrR[i+1];
                histogram->sumArrG[i+1] = histogram->sumArrG[i] + histogram->arrG[i+1];
                histogram->sumArrB[i+1] = histogram->sumArrB[i] + histogram->arrB[i+1];
            }
            histogram->divide(Histogram::R, height*width);
            histogram->divide(Histogram::G, height*width);
            histogram->divide(Histogram::B, height*width);
            histogram->divide(Histogram::sumR, height*width);
            histogram->divide(Histogram::sumG, height*width);
            histogram->divide(Histogram::sumB, height*width);
            this->histogramAvailable = true;
        }
    }
}

void Image::calcKeyPoint(bool reCalc) {
    if (this->height * this->width <= 0) return;
    if ((reCalc) || (!keypointAvailable)) {
        pointSet.clear();

        vector<Image*> *tower = new vector<Image*>();
        vector<double*> *points = new vector<double*>();
        tower->push_back(this);
        points->push_back(new double[this->width * this->height]);

        Image *p = this;
        while ((p->height >= 2) && (p->width >= 2)) {
            p = ImageEdit::bilinear(p, p->width >> 1, p->height >> 1);
            tower->push_back(p);
            points->push_back(new double[p->width * p->height]);
        }

        int cnt = 0;
        for (int i=0; i<tower->size(); ++i) {
            Image *now = (*tower)[i];
            Image *blur = ImageEdit::gaussianBlur(now, 8);
            for (int j=0; j<now->height * now->width; ++j)
                (*points)[i][j] = abs(blur->R[j] - now->R[j]) +
                    abs(blur->G[j] - now->G[j]) +
                    abs(blur->B[j] - now->B[j]);
            delete blur;
        }

        for (int k=tower->size() - 2; k>=1; --k) {
            if (k > 4) continue;
            Image *now = (*tower)[k];
            Image *pre = (*tower)[k-1];
            Image *nex = (*tower)[k+1];
            double *arr = (*points)[k];
            double *parr = (*points)[k-1];
            double *narr = (*points)[k+1];
            for (int i=7; i<now->height-8; ++i)
                for (int j=7; j<now->width-8; ++j) {
                    int pi = i << 1, pj = j << 1;
                    int ni = i >> 1, nj = j >> 1;

                    if ((pi > 0) && (pj > 0) && (ni > 0) && (nj > 0) &&
                            (pi < pre->height-1) && (pj < pre->width-1) &&
                            (ni < nex->height-1) && (nj < nex->width-1) &&
                        (arr[I(i,j,now->width)] > arr[I(i-1,j,now->width)] + EPS) &&
                        (arr[I(i,j,now->width)] > arr[I(i+1,j,now->width)] + EPS) &&
                        (arr[I(i,j,now->width)] > arr[I(i+1,j,now->width)] + EPS) &&
                        (arr[I(i,j,now->width)] > arr[I(i+1,j,now->width)] + EPS) &&
                        //(arr[I(i,j,now->width)] > arr[I(i+1,j+1,now->width)] + EPS) &&
                        //(arr[I(i,j,now->width)] > arr[I(i+1,j-1,now->width)] + EPS) &&
                        //(arr[I(i,j,now->width)] > arr[I(i-1,j+1,now->width)] + EPS) &&
                        //(arr[I(i,j,now->width)] > arr[I(i-1,j-1,now->width)] + EPS) &&

                                                    (arr[I(i,j,now->width)] > parr[I(pi,pj,pre->width)] + EPS) &&
                                                    (arr[I(i,j,now->width)] > parr[I(pi-1,pj,pre->width)] + EPS) &&
                                                    (arr[I(i,j,now->width)] > parr[I(pi+1,pj,pre->width)] + EPS) &&
                                                    (arr[I(i,j,now->width)] > parr[I(pi+1,pj,pre->width)] + EPS) &&
                                                    (arr[I(i,j,now->width)] > parr[I(pi+1,pj,pre->width)] + EPS) &&
                                                    //(arr[I(i,j,now->width)] > parr[I(pi+1,pj+1,pre->width)] + EPS) &&
                                                    //(arr[I(i,j,now->width)] > parr[I(pi+1,pj-1,pre->width)] + EPS) &&
                                                    //(arr[I(i,j,now->width)] > parr[I(pi-1,pj+1,pre->width)] + EPS) &&
                                                    //(arr[I(i,j,now->width)] > parr[I(pi-1,pj-1,pre->width)] + EPS) &&
                            (arr[I(i,j,now->width)] > narr[I(ni,nj,nex->width)] + EPS) &&
                            (arr[I(i,j,now->width)] > narr[I(ni-1,nj,nex->width)] + EPS) &&
                            (arr[I(i,j,now->width)] > narr[I(ni+1,nj,nex->width)] + EPS) &&
                            (arr[I(i,j,now->width)] > narr[I(ni+1,nj,nex->width)] + EPS) &&
                            (arr[I(i,j,now->width)] > narr[I(ni+1,nj,nex->width)] + EPS) &&
                            //(arr[I(i,j,now->width)] > narr[I(ni+1,nj+1,nex->width)] + EPS) &&
                            //(arr[I(i,j,now->width)] > narr[I(ni+1,nj-1,nex->width)] + EPS) &&
                            //(arr[I(i,j,now->width)] > narr[I(ni-1,nj+1,nex->width)] + EPS) &&
                            //(arr[I(i,j,now->width)] > narr[I(ni-1,nj-1,nex->width)] + EPS) &&
                        (arr[I(i,j,now->width)] > Image::threshold)) {
                        KeyPoint kp;
                        if (k == 0) kp.r = i; else kp.r = (i << k) + (1 << (k-1));
                        if (k == 0) kp.c = j; else kp.c = (j << k) + (1 << (k-1));
                        kp.scale = 1 << k;
                        kp.value = arr[I(i,j,now->width)];
                        this->pointSet.push_back(kp);
                    }
                }
        }

        for (int i=1; i<tower->size(); ++i)
            delete (*tower)[i];
        delete tower;
        for (int i=0; i<points->size(); ++i)
            delete (*points)[i];
        delete points;

        keypointAvailable = true;
    }
}

void Image::calcFeatureVec(bool reCalc) {
    if (this->height * this->width <= 0) return;
    if (!keypointAvailable) return;
    if ((reCalc) || (!featureVecAvailble)) {
        vector<Image*> *tower = new vector<Image*>();
        tower->push_back(this);
        Image *p = this;
        while ((p->height >= 2) && (p->width >= 2)) {
            p = ImageEdit::bilinear(p, p->width >> 1, p->height >> 1);
            tower->push_back(p);
        }

        for (int i=0; i < pointSet.size(); ++i) {
            KeyPoint *point = &(pointSet[i]);

            int tNo = 0, tmpInt = 1;
            while (tmpInt < point->scale) tmpInt <<= 1, ++tNo;

            int r = point->r / point->scale, c = point->c / point->scale;
            Image *img = (*tower)[tNo];

            double *direcs = new double[36];
            memset(direcs, 0, sizeof(double) * 36);
            for (int ii = -7; ii <= 8; ++ii)
                for (int jj = -7; jj <= 8; ++jj) {
                    vec2 t0 = getDelta(img, 0, r + ii, c + jj), t1 = getDelta(img, 1, r + ii, c + jj), t2 = getDelta(img, 2, r + ii, c + jj);
                    double ang = getAngle(vec2(t0.x + t1.x + t2.x, t0.y + t1.y + t2.y));
                    direcs[((int)(ang + 0.5f) / 10) % 36] += exp(-(double)(ii*ii + jj*jj) / (double)(2*8.0f));
                }
            int direc = 0, compdirec = -1; double tmp = 0.0f, tmp2 = 0.0f;
            for (int ii = 0; ii < 36; ++ii)
                if (direcs[ii] > tmp)
                    tmp = direcs[ii], direc = ii*10;
            for (int ii = 0; ii < 36; ++ii)
                if ((ii != (direc / 10)) && (direcs[ii] > tmp2))
                    tmp2 = direcs[ii], compdirec = ii;
            if (tmp2 < 0.8f * tmp) compdirec = -1;
            delete[] direcs;

            point->orient = direc;

            point->feature = new double[128];
            memset(point->feature, 0, sizeof(double) * 128);
            if (compdirec != -1) {
                point->feature2 = new double[128];
                memset(point->feature2, 0, sizeof(double) * 128);
                //cerr << "compdirec" << endl;
            }

            for (int ii = 0; ii < 4; ++ii)
                for (int jj = 0; jj < 4; ++jj) {
                    for (int di = -7 + 4*ii; di < -3 + 4*ii; ++di)
                        for (int dj = -7 + 4*jj; dj < -3 + 4*jj; ++dj) {
                            vec2 t0 = getDelta(img, 0, r + di, c + dj), t1 = getDelta(img, 1, r + di, c + dj), t2 = getDelta(img, 2, r + di, c + dj);
                            double ang = getAngle(vec2(t0.x + t1.x + t2.x, t0.y + t1.y + t2.y));
                            double weigh = exp(-(double)(di*di + dj*dj) / (double)(2*8.0f));
                            point->feature[((ii * 4 + jj) << 3) + (int)((ang - direc + 22.5f + 360.0f) / 45.0f) % 8] += weigh;
                            if (compdirec != -1)
                                point->feature2[((ii * 4 + jj) << 3) + (int)((ang - compdirec + 22.5f + 360.0f) / 45.0f) % 8] += weigh;
                        }
                }

            //cerr << "Point " << i << ": " << point->r << " " << point->c << " " << point->orient << endl;
        }

        for (int i=1; i<tower->size(); ++i)
            delete (*tower)[i];
        delete tower;
        featureVecAvailble = true;
    }
}

void Image::featureMatch(Image *merit) {
    if ((merit == NULL) || (merit->width * merit->height <= 0) || (!merit->featureVecAvailble))
        return;
    for (int i=0; i<merit->pointSet.size(); ++i) {
        double minV = 1e+100; int p = -1;
        double *here = ((merit->pointSet)[i]).feature;
        for (int j=0; j<this->pointSet.size(); ++j) {
            double *there = ((this->pointSet)[j]).feature;
            double now;

            now = 0.0f;
            for (int k=0; k<128; ++k)
                now += (there[k] - here[k]) * (there[k] - here[k]);
            if (now < minV)
                minV = now, p = j;
            there = ((this->pointSet)[j]).feature2;
            if (there != NULL) {
                now = 0.0f;
                for (int k=0; k<128; ++k)
                    now += (there[k] - here[k]) * (there[k] - here[k]);
                if (now < minV)
                    minV = now, p = j;
            }
        }

        if (((merit->pointSet)[i]).feature2 != NULL)
            here = ((merit->pointSet)[i]).feature2;
        if (here) {
            for (int j=0; j<this->pointSet.size(); ++j) {
                double *there = ((this->pointSet)[j]).feature;
                double now;

                now = 0.0f;
                for (int k=0; k<128; ++k)
                    now += (there[k] - here[k]) * (there[k] - here[k]);
                if (now < minV)
                    minV = now, p = j;
                there = ((this->pointSet)[j]).feature2;
                if (there != NULL) {
                    now = 0.0f;
                    for (int k=0; k<128; ++k)
                        now += (there[k] - here[k]) * (there[k] - here[k]);
                    if (now < minV)
                        minV = now, p = j;
                }
            }
        }

        cerr << "MATCH " << "this " << p << " that " << i << " with v=" << minV << endl;
        if (minV <= 25.0) {
            (this->pointSet)[p].matched = i;
            (merit->pointSet)[i].matched = p;
        }
    }
}

KeyPoint *Image::calcPixel(Image *img, int r, int c) {
    KeyPoint *ans = new KeyPoint();

    if (img == NULL) return NULL;
    double a11 = 0.0f, a12 = 0.0f, a21 = 0.0f, a22 = 0.0f;
    double v = 0.0f;;
    for (int channel = 0; channel < 3; ++channel) {
        for (int i = r-W; i<=r+W; ++i)
            for (int j = c-W; j<=c+W; ++j) {
                vec2 d = Image::getDelta(img, channel, i, j);
                //cerr << d.x << " " << d.y << endl;
                a11 += d.x * d.x;
                a12 += d.x * d.y;
                a21 += d.y * d.x;
                a22 += d.y * d.y;
            }
        a11 /= ((W << 1) + 1) * ((W << 1) + 1);
        a12 /= ((W << 1) + 1) * ((W << 1) + 1);
        a21 /= ((W << 1) + 1) * ((W << 1) + 1);
        a22 /= ((W << 1) + 1) * ((W << 1) + 1);
        if (a11 + a22 != 0)
            v += (a11 * a22 - a12 * a21) / (a11 + a22);
    }

    ans->r = r;
    ans->c = c;
    ans->value = v / 3.0f;

    //printf("%d %d %lf\n", ans->r, ans->c, ans->value);
    return ans;
}

vec2 Image::getDelta(Image *img, int channel, int r, int c) {
    if ((img == NULL) || (img->height * img->width <= 0)
        || (r < 0) || (r >= img->height)
        || (c < 0) || (c >= img->width)) return vec2();

    int *arr = NULL;
    switch (channel) {
        case 0: arr = img->R; break;
        case 1: arr = img->G; break;
        case 2: arr = img->B; break;
    }
    if (arr == NULL) return vec2();

    vec2 ans;
    if ((r < 0) || (img->height == 1)) ans.x = 0.0f;
    else if (r == 0) ans.x = arr[I(r + 1, c, img->width)] - arr[I(r, c, img->width)];
    else if (r == (img->height - 1)) ans.x = arr[I(r, c, img->width)] - arr[I(r - 1, c, img->width)];
    else ans.x = (double)(arr[I(r+1, c, img->width)] - arr[I(r-1, c, img->width)]) / 2.0f;

    if ((c < 0) || (img->width == 1)) ans.y = 0.0f;
    else if (c == 0) ans.y = arr[I(r, c + 1, img->width)] - arr[I(r, c, img->width)];
    else if (c == (img->width - 1)) ans.y = arr[I(r, c, img->width)] - arr[I(r, c - 1, img->width)];
    else ans.y = (double)(arr[I(r, c+1, img->width)] - arr[I(r, c-1, img->width)]) / 2.0f;

    return ans;
}

double Image::getAngle(vec2 vec) {
    if (abs(vec.x) < REAL_EPS)
        if (vec.y >= 0) return 90.0f; else return 270.0f;
    double ang = atan2(vec.y, vec.x);
    if (ang < 0) ang = 2 * PI +ang;
    ang = ang / PI * 180;
    return ang;
}

Image::Image()
{
    height = width = 0;
    R = G = B = NULL;
    histogram = NULL;
    histogramAvailable = false;
    pointSet.clear();
    keypointAvailable = false;
    featureVecAvailble = false;
}

Image::~Image() {
    delete[] R;
    delete[] G;
    delete[] B;
}

