#include "ScalingConfig.h"

#define ABS(x) (((x) < 0)? (-(x)) : (x))

ScalingConfig::ScalingConfig() {
   type = RATIO;
   absoluteW = 100;
   absoluteH = 100;
   ratioW = 1.0;
   ratioH = 1.0;
   str = genStr(type, absoluteW, absoluteH, ratioW, ratioH);
}

int ScalingConfig::read(char *buf, size_t size, off_t offset) {
    if (size + offset - 1 > str.length() + 1) {
        size = str.length() + 1 - offset + 1;
    }
    memcpy(buf, (void*)(str.c_str() + offset), size);
    return size;
}

int ScalingConfig::getSize() {
    return str.length();
}

int ScalingConfig::write(const char *buf, size_t size, off_t offset) {
    bool ok = false;
    int newSize = str.length() + 1;
    if (offset + size - 1 > newSize) newSize = offset + size - 1;
    char *newChar = new char[newSize];
    memcpy((void*)newChar, (void*)str.c_str(), str.length() + 1);
    memcpy((void*)(newChar + offset), (void*)buf, size);
    char typeStr[20];
    double w, h;
    if (sscanf(newChar, "Scaling\nType: %s\nWidth: %lf\nHeight: %lf\n", typeStr, &w, &h) == 3) {
        if (strcmp(typeStr, "Ratio") == 0) {
            if ((w > 0.0) && (h > 0.0)) {
                type = RATIO;
                ratioW = w;
                ratioH = h;

                cerr << "```Scaling Ratio " << w << " " << h << endl;
                ok = true;
            }
        }
        if (strcmp(typeStr, "Absolute") == 0) {
            int ww = (int)(w+0.5), hh = (int)(h+0.5);
            if ((ABS(w - (double)ww) < 1e-6) && (ABS(h - (double)hh) < 1e-6))
                if ((ww > 0) && (hh > 0) && (ww < 5000) & (hh < 5000)) {
                    type = ABSOLUTE;
                    absoluteW = w;
                    absoluteH = h;

                    cerr << "```Scaling Absolute " << w << " " << h << endl;
                    ok = true;
                }
        }
        if (ok) {
            str = genStr(type, absoluteW, absoluteH, ratioW, ratioH);
        }
    }
    delete[] newChar;
    if (ok) return size; else return -EIO;
}

string ScalingConfig::genStr(SCALE_TYPE type,
                             int absoluteW, int absoluteH,
                             double ratioW, double ratioH) {
    string str;
    stringstream sin(str);
    sin.clear();
    sin << "Scaling\n";
    sin << "Type: ";
        if (type == RATIO) sin << "Ratio";
        if (type == ABSOLUTE) sin << "Absolute";
    sin << "\n";
    sin << "Width: ";
        if (type == RATIO) sin << ratioW;
        if (type == ABSOLUTE) sin << absoluteW;
    sin << "\n";
    sin << "Height: ";
        if (type == RATIO) sin << ratioH;
        if (type == ABSOLUTE) sin << absoluteH;
    sin << "\n";
    sin << "# Type can be 'Ratio' or 'Absolute'\n";
    sin << "# The output size is determined by ratio or absolute pixels accordingly\n";
    sin << "# When type is 'Ratio', width and height is output/input ratio\n";
    sin << "    Width and height should be positive real numbers\n";
    sin << "# When type is 'Absolute', width and height is number of pixels\n";
    sin << "    Width and height should be positive integer and not larger than 5000";
    sin << endl;
    sin.flush();
    return sin.str();
}

