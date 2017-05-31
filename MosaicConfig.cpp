#include "MosaicConfig.h"

MosaicConfig::MosaicConfig() {
   radius = 5;
   str = genStr(radius);
}

int MosaicConfig::read(char *buf, size_t size, off_t offset) {
    if (size + offset - 1 > str.length() + 1) {
        size = str.length() + 1 - offset + 1;
    }
    memcpy(buf, (void*)(str.c_str() + offset), size);
    return size;
}

int MosaicConfig::getSize() {
    return str.length();
}

int MosaicConfig::write(const char *buf, size_t size, off_t offset) {
    bool ok = false;
    int newSize = str.length() + 1;
    if (offset + size - 1 > newSize) newSize = offset + size - 1;
    char *newChar = new char[newSize];
    memcpy((void*)newChar, (void*)str.c_str(), str.length() + 1);
    memcpy((void*)(newChar + offset), (void*)buf, size);
    int newRadius;
    if (sscanf(newChar, "Mosaic\nRadius: %d", &newRadius) == 1) {
        if (newRadius > 0) {
            radius = newRadius;
            str = genStr(radius);

            cerr << "```Mosaic " << radius << endl;

            ok = true;
        }
    }
    delete[] newChar;
    if (ok) return size; else return -EIO;
}

string MosaicConfig::genStr(int radius) {
    string str;
    stringstream sin(str);
    sin.clear();
    sin << "Mosaic\n";
    sin << "Radius: " << radius << "\n";
    sin << "# Please modify radius\n";
    sin << "# Radius should be a positive integer\n";
    sin << endl;
    sin.flush();
    return sin.str();
}
