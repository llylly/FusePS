#include "BrightnessConfig.h"

BrightnessConfig::BrightnessConfig() {
    gamma = 1.0;
    str = genStr(gamma);
}

int BrightnessConfig::read(char *buf, size_t size, off_t offset) {
    if (size + offset - 1 > str.length() + 1) {
        size = str.length() + 1 - offset + 1;
    }
    memcpy(buf, (void*)(str.c_str() + offset), size);
    return size;
}

int BrightnessConfig::getSize() {
    return str.length();
}

int BrightnessConfig::write(const char *buf, size_t size, off_t offset) {
    bool ok = false;
    int newSize = str.length() + 1;
    if (offset + size - 1 > newSize) newSize = offset + size - 1;
    char *newChar = new char[newSize];
    memcpy((void*)newChar, (void*)str.c_str(), str.length() + 1);
    memcpy((void*)(newChar + offset), (void*)buf, size);
    double newGamma;
    if (sscanf(newChar, "Brightness\nGamma: %lf", &newGamma) == 1) {
        if (newGamma > 0.0) {
            gamma = newGamma;
            str = genStr(gamma);

            cerr << "```Brightness " << gamma << endl;

            ok = true;
        }
    }
    delete[] newChar;
    if (ok) return size; else return -EIO;
}

string BrightnessConfig::genStr(double gamma) {
    string str;
    stringstream sin(str);
    sin.clear();
    sin << "Brightness\n";
    sin << "Gamma: " << gamma << "\n";
    sin << "# Please modify Gamma\n";
    sin << "# Gamma is a positive real number\n";
    sin << "# When larger than 1, image becomes brighter\n";
    sin << "# When smaller than 1, image becomes darker\n";
    sin << endl;
    sin.flush();
    return sin.str();
}
