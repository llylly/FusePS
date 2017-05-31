#include "PSStatus.h"

PSStatus::PSStatus() {
    foldersN = 6;
    foldersPath = new char*[foldersN]
        {(char*)"/PS/Graying", (char*)"/PS/Scaling", (char*)"/PS/Blur",
         (char*)"/PS/Mosaic", (char*)"/PS/Brightness", (char*)"/PS/Contrast"};
    foldersName = new char*[foldersN]
        {"Graying", "Scaling", "Blur", "Mosaic", "Brightness", "Contrast"};
    foldersConfig = new int[foldersN]
        {-1, 0, 1, 2, 3, 4};
    configsN = 5;
    configsPath = new char*[configsN]
        {"/PS/Scaling/config.txt", "/PS/Blur/config.txt", "/PS/Mosaic/config.txt",
        "/PS/Brightness/config.txt", "/PS/Contrast/config.txt"};
    configs = new BasePSConfig*[configsN] {
        new ScalingConfig(), new BlurConfig(), new MosaicConfig(),
        new BrightnessConfig(), new ContrastConfig()
        };
    stamp = 0;
    fileMap = new map<string, string>[foldersN];
    for (int i=0; i<foldersN; ++i)
        fileMap[i].clear();
    baked = new map<string, bool>[foldersN];
    for (int i=0; i<foldersN; ++i)
        baked[i].clear();
}

int PSStatus::initBuffer(int *argc, char **argv[]) {
    bufferFolder = "PSBuf";
    bool find = false;
    int place = -1;
    for (int i=0; i<(*argc) - 1; ++i) {
        if (strcmp((*argv)[i], "-PSBuf") == 0) {
            place = i;
            find = true;
            break;
        }
    }
    if (find) {
        bufferFolder = new char[strlen((*argv)[place + 1]) + 1];
        strcpy(bufferFolder, (*argv)[place + 1]);
    }

    mkdir(bufferFolder, S_IRWXU | S_IRWXG | S_IRWXO);

    {
        if (find) {
            char **newArgv = new char*[*argc - 2];
            for (int i=0; i < place; ++i) {
                newArgv[i] = new char[strlen((*argv)[i]) + 1];
                strcpy(newArgv[i], (*argv)[i]);
            }
            for (int i=place; i < (*argc) - 2; ++i) {
                newArgv[i] = new char[strlen((*argv)[i + 2]) + 1];
                strcpy(newArgv[i], (*argv)[i + 2]);
            }
            *argc -= 2;
            argv = &newArgv;
        }
        return 0;
    }
}

int PSStatus::work(string path, int typeNo) {
    QImage *qimg = new QImage();
    if (qimg->load(QString::fromStdString(path))) {
        Image *img = Image::fromQImage(qimg);
        Image *ans = NULL;
        if (typeNo == 0) {
            // Graying
            ans = ImageEdit::graying(img);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
        if (typeNo == 1) {
            // Scaling
            ScalingConfig *conf = (ScalingConfig*)configs[foldersConfig[typeNo]];
            int w, h;
            if (conf->type == ABSOLUTE)
                w = conf->absoluteW, h = conf->absoluteH;
            else
                w = conf->ratioW * img->width, h = conf->ratioH * img->height;
            cerr << "```ScalingWH" << " " << w << " " << h << endl;
            if ((w * h <= 0) || (w >= 5000) || (h >= 5000)) return -1;
            ans = ImageEdit::bilinear(img, w, h);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
        if (typeNo == 2) {
            // Blur
            BlurConfig *conf = (BlurConfig*)configs[foldersConfig[typeNo]];
            ans = ImageEdit::gaussianBlur(img, conf->radius);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
        if (typeNo == 3) {
            // Mosaic
            MosaicConfig *conf = (MosaicConfig*)configs[foldersConfig[typeNo]];
            ans = ImageEdit::mosaicBlur(img, conf->radius);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
        if (typeNo == 4) {
            // Brightness
            BrightnessConfig *conf = (BrightnessConfig*)configs[foldersConfig[typeNo]];
            ans = ImageEdit::gammaAdjust(img, conf->gamma);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
        if (typeNo == 5) {
            //Contrast
            ContrastConfig *conf = (ContrastConfig*)configs[foldersConfig[typeNo]];
            ans = ImageEdit::contrastAdjust(img, conf->L, conf->R);
            if (ans) {
                ans->save(path);
                return 0;
            } else
                return -1;
        }
    } else
        return -1;
}
