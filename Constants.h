#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Constants
{
public:
    static string PROJ_NAME;
    static int DEFAULT_WIDTH;
    static int DEFAULT_HEIGHT;
    static string IO_GROUP_TITLE;
    static string HISTOGRAM_GROUP_TITLE;
    static string RESAMPLING_GROUP_TITLE;
    static string ROTATE_GROUP_TITLE;
    static string FORGREY_GROUP_TITLE;
    static string ABOUT_GROUP_TITLE;
    static string OPEN_IMAGE_CAPTION;
    static string SAVE_IMAGE_CAPTION;
    static string UNDO_CAPTION;
    static string SHOW_HISTOGRAM_CAPTION;
    static string BRIGHT_CAPTION;
    static string CONTRAST_CAPTION;
    static string GAMMA_CAPTION;
    static string HISTOGRAM_EQUALIZATION_CAPTION;
    static string HISTOGRAM_MATCH_CAPTION;
    static string BLUR_CAPTION;
    static string RESAMPLING_CAPTION;
    static string PSNR_CAPTION;
    static string ROTATE_CAPTION;
    static string SPHERE_CAPTION;
    static string POINT_DETECT_CAPTION;
    static string DIFFERENCE_CAPTION;
    static string ABOUT_CAPTION;
    static string OPEN_IMAGE_DIALOG;
    static string OPEN_IMAGE_FILTER;
    static string SAVE_IMAGE_DIALOG;
    static string SAVE_IMAGE_FILTER;
    static string SAVE_IMAGE_INFO;
    static string DEFAULT_STATUS_BAR;
    static string EMPTY_IMAGE_STATUS_BAR;
    static string IMAGE_STATUS_BAR(int h, int w);
    static string HISTOGRAM_DIALOG;
    static string RED;
    static string GREEN;
    static string BLUE;
    static string OK;
    static string ERROR;
    static string HISTOGRAM_ERROR;
    static string BRIGHT_UNIFORM;
    static string BRIGHT_SEPARATE;
    static string BRIGHT_DELTA;
    static string BRIGHT_RDELTA;
    static string BRIGHT_GDELTA;
    static string BRIGHT_BDELTA;
    static string BRIGHT_DIALOG_CAPTION;
    static int BRIGHT_DIALOG_W;
    static int BRIGHT_DIALOG_H;
    static string ILLEGAL_VALUE;
    static string CONTRAST_MIN;
    static string CONTRAST_MAX;
    static string CONTRAST_DIALOG_CAPTION;
    static int CONTRAST_DIALOG_W;
    static int CONTRAST_DIALOG_H;
    static string GAMMA_DIALOG_CAPTION;
    static string STYLIZE_DIALOG;
    static string ZOOM_TITLE;
    static string ZOOM_NEAREST_NEIGHBOR;
    static string ZOOM_BILINEAR;
    static string ZOOM_BICUBIC;
    static string ZOOM_ORIGINAL_SIZE;
    static string ZOOM_BY_RATIO;
    static string ZOOM_BY_SIZE;
    static string ZOOM_RATIO;
    static string ZOOM_SIZE;
    static string ZOOM_WIDTH;
    static string ZOOM_HEIGHT;
    static string ZOOM_UNKNOWN;
    static string ZOOM_NO_CHOOSE;
    static int ZOOM_DIALOG_WIDTH;
    static int ZOOM_DIALOG_HEIGHT;
    static string ZOOM_CAUTION;
    static int ZOOM_LIMIT;
    static string PSNR_DIALOG;
    static string PSNR;
    static string PSNR_ILLEGAL_CAPTION(int ow, int oh, int rw, int rh);
    static int PSNR_DIALOG_W;
    static int PSNR_DIALOG_H;
    static string BLUR_UNIFORM;
    static string BLUR_GAUSSIAN;
    static string BLUR_MOSAIC;
    static string BLUR_RADIUS;
    static int BLUR_DIALOG_W;
    static int BLUR_DIALOG_H;
    static string BLUR_ILLEGAL_VALUE;
    static string BLUR_NO_CHOOSE;
    static int BLUR_LIMIT;
    static string DIFF_OPEN_DIALOG;
    static int ABOUT_DIALOG_WIDTH;
    static int ABOUT_DIALOG_HEIGHT;

    Constants();
};

#endif // CONSTANTS_H
