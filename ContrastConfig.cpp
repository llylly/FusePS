#include "ContrastConfig.h"

ContrastConfig::ContrastConfig() {
    L = 0;
    R = 255;
    str = genStr(L, R);
}

int ContrastConfig::read(char *buf, size_t size, off_t offset) {
    if (size + offset - 1 > str.length() + 1) {
        size = str.length() + 1 - offset + 1;
    }
    memcpy(buf, (void*)(str.c_str() + offset), size);
    return size;
}

int ContrastConfig::getSize() {
    return str.length();
}

int ContrastConfig::write(const char *buf, size_t size, off_t offset) {
    bool ok = false;
    int newSize = str.length() + 1;
    if (offset + size - 1 > newSize) newSize = offset + size - 1;
    char *newChar = new char[newSize];
    memcpy((void*)newChar, (void*)str.c_str(), str.length() + 1);
    memcpy((void*)(newChar + offset), (void*)buf, size);
    int newL, newR;
    if (sscanf(newChar, "Contrast\nL: %d\nR: %d\n", &newL, &newR) == 2) {
        if ((newL >= 0) && (newL <= 255) && (newR >= 0) && (newR <= 255)) {
            L = newL, R = newR;
            str = genStr(L, R);

            cerr << "```Contrast " << L << " " << R << endl;

            ok = true;
        }
    }
    delete[] newChar;
    if (ok) return size; else return -EIO;
}

string ContrastConfig::genStr(int l, int r) {
    string str;
    stringstream sin(str);
    sin.clear();
    sin << "Contrast\n";
    sin << "L: " << l << "\n";
    sin << "R: " << r << "\n";
    sin << "# Please modify L and R, we will map color of each pixel from L~R to 0~255";
    sin << endl;
    sin.flush();
    return sin.str();
}
