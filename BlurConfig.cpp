#include "BlurConfig.h"


BlurConfig::BlurConfig() {
   radius = 5;
   str = genStr(radius);
}

int BlurConfig::read(char *buf, size_t size, off_t offset) {
    if (size + offset - 1 > str.length() + 1) {
        size = str.length() + 1 - offset + 1;
    }
    memcpy(buf, (void*)(str.c_str() + offset), size);
    return size;
}

int BlurConfig::getSize() {
    return str.length();
}

int BlurConfig::write(const char *buf, size_t size, off_t offset) {
    bool ok = false;
    int newSize = str.length() + 1;
    if (offset + size - 1 > newSize) newSize = offset + size - 1;
    char *newChar = new char[newSize];
    memcpy((void*)newChar, (void*)str.c_str(), str.length() + 1);
    memcpy((void*)(newChar + offset), (void*)buf, size);
    int newRadius;
    if (sscanf(newChar, "Blur\nRadius: %d", &newRadius) == 1) {
        if ((newRadius > 0) && (newRadius < 51)) {
            radius = newRadius;
            str = genStr(radius);

            cerr << "```Blur " << radius << endl;

            ok = true;
        }
    }
    delete[] newChar;
    if (ok) return size; else return -EIO;
}

string BlurConfig::genStr(int radius) {
    string str;
    stringstream sin(str);
    sin.clear();
    sin << "Blur\n";
    sin << "Radius: " << radius << "\n";
    sin << "# Gaussian Blur\n";
    sin << "# Please modify radius\n";
    sin << "# Radius should be a positive integer and not larger than 50\n";
    sin << endl;
    sin.flush();
    return sin.str();
}
