#include "Constants.h"

string Constants::PROJ_NAME = "Kapu Image Editor";
int Constants::DEFAULT_WIDTH = 800;
int Constants::DEFAULT_HEIGHT = 600;
string Constants::IO_GROUP_TITLE = "File";
string Constants::HISTOGRAM_GROUP_TITLE = "Histogram";
string Constants::RESAMPLING_GROUP_TITLE = "Zoom";
string Constants::ROTATE_GROUP_TITLE = "Rotate";
string Constants::FORGREY_GROUP_TITLE = "Feature Detect";
string Constants::ABOUT_GROUP_TITLE = "About";

string Constants::OPEN_IMAGE_CAPTION = "Open Image";
string Constants::SAVE_IMAGE_CAPTION = "Save Image";
string Constants::UNDO_CAPTION = "Undo";
string Constants::SHOW_HISTOGRAM_CAPTION = "Show Histogram";
string Constants::BRIGHT_CAPTION = "Adjust Brightness";
string Constants::CONTRAST_CAPTION = "Adjust Contrast";
string Constants::GAMMA_CAPTION = "Adjust Gamma";
string Constants::HISTOGRAM_EQUALIZATION_CAPTION = "Equalize";
string Constants::HISTOGRAM_MATCH_CAPTION = "Stylize";
string Constants::BLUR_CAPTION = "Blur";
string Constants::PSNR_CAPTION = "Calculate PSNR";
string Constants::RESAMPLING_CAPTION = "Zoom";
string Constants::ROTATE_CAPTION = "Rotate";
string Constants::SPHERE_CAPTION = "Spherize";
string Constants::POINT_DETECT_CAPTION = "Key Point Detect";
string Constants::DIFFERENCE_CAPTION = "Difference Capture";
string Constants::ABOUT_CAPTION = "About";

string Constants::OPEN_IMAGE_DIALOG = "Choose an image";
string Constants::OPEN_IMAGE_FILTER = "Images (*.bmp *.png *.jpg *.jpeg *.tif *.GIF)";

string Constants::SAVE_IMAGE_DIALOG = "Save image to";
string Constants::SAVE_IMAGE_FILTER = "BMP Images (*.bmp);;JPG Images (*.jpg);;JPEG Images (*.jpeg);;PNG Images (*.png)";
string Constants::SAVE_IMAGE_INFO = "Please load an image first.";

string Constants::DEFAULT_STATUS_BAR = "Ready";
string Constants::EMPTY_IMAGE_STATUS_BAR = "Please open an image";
string Constants::IMAGE_STATUS_BAR(int h, int w) {
    string s = "";
    ostringstream sin(s);
    sin << "Height: " << h << " | " << "Width: " << w;
    sin.flush();
    return sin.str();
}

string Constants::HISTOGRAM_DIALOG = "Histogram";
string Constants::RED = "Red";
string Constants::GREEN = "Green";
string Constants::BLUE = "Blue";
string Constants::OK = "Ok";

string Constants::ERROR = "Error";
string Constants::HISTOGRAM_ERROR = "Sorry! Histogram calculation error.";

string Constants::BRIGHT_UNIFORM = "Uniform Adjust";
string Constants::BRIGHT_SEPARATE = "Separate Adjust";
string Constants::BRIGHT_DELTA = "Delta";
string Constants::BRIGHT_RDELTA = "R Channel Delta";
string Constants::BRIGHT_GDELTA = "G Channel Delta";
string Constants::BRIGHT_BDELTA = "B Channel Delta";
string Constants::BRIGHT_DIALOG_CAPTION = "Delta should be in -255 ~ 255.";

string Constants::ILLEGAL_VALUE = "Input value is illegal.";

int Constants::BRIGHT_DIALOG_W = 380;
int Constants::BRIGHT_DIALOG_H = 200;

string Constants::CONTRAST_MIN = "Min Value";
string Constants::CONTRAST_MAX = "Max Value";
string Constants::CONTRAST_DIALOG_CAPTION = \
        "The interval 'Min Value ~ Max Value' is projected to 0 ~ 255 in histogram.\n"
        "'Min Value' >= 'Max Value' is permitted.\n"
        "They should be in interval 0 ~ 255.";

int Constants::CONTRAST_DIALOG_W = 400;
int Constants::CONTRAST_DIALOG_H = 200;

string Constants::GAMMA_DIALOG_CAPTION =
        "Gamma factor is a positive real number.\n"
        "For factor > 1, image becomes lighter;\n"
        "For factor < 1, image becomes darker.\n"
        "Gamma Factor:";

string Constants::STYLIZE_DIALOG = "Choose Reference Image";

string Constants::ZOOM_TITLE = "Zoom";
string Constants::ZOOM_NEAREST_NEIGHBOR = "Nearest Neighbor";
string Constants::ZOOM_BILINEAR = "Bilinear";
string Constants::ZOOM_BICUBIC = "Bicubic";
string Constants::ZOOM_ORIGINAL_SIZE = "Original Size";
string Constants::ZOOM_BY_RATIO = "By Ratio";
string Constants::ZOOM_BY_SIZE = "By size";
string Constants::ZOOM_RATIO = "Ratio";
string Constants::ZOOM_SIZE = "Size";
string Constants::ZOOM_WIDTH = "Width";
string Constants::ZOOM_HEIGHT = "Height";
string Constants::ZOOM_UNKNOWN = "Unknown";
string Constants::ZOOM_NO_CHOOSE = "Please choose one zoom policy.";
int Constants::ZOOM_DIALOG_WIDTH = 600;
int Constants::ZOOM_DIALOG_HEIGHT = 300;
string Constants::ZOOM_CAUTION = \
        "Width and height should be in interval 1 ~ 5000 by pixel.";
int Constants::ZOOM_LIMIT = 5000;

string Constants::PSNR_DIALOG = "Choose Another Image for PSNR Compare";
string Constants::PSNR = "PSNR";

string Constants::PSNR_ILLEGAL_CAPTION(int ow, int oh, int rw, int rh) {
    string s = "";
    ostringstream sin(s);
    sin << "Size not match." << endl;
    sin << "Original Image | Height: " << oh << " | " << "Width: " << ow << endl;
    sin << "Refering Image | Height: " << rh << " | " << "Width: " << rw << endl;
    sin.flush();
    return sin.str();
}

int Constants::PSNR_DIALOG_H = 300;
int Constants::PSNR_DIALOG_W = 300;

string Constants::BLUR_UNIFORM = "Uniform Blur";
string Constants::BLUR_GAUSSIAN = "Gaussian Blur";
string Constants::BLUR_MOSAIC = "Mosaic";
string Constants::BLUR_RADIUS = "Radius";
int Constants::BLUR_DIALOG_W = 350;
int Constants::BLUR_DIALOG_H = 150;
string Constants::BLUR_ILLEGAL_VALUE = "Illegal Value. The radius should be a positive integer and smaller or equal than 50.";
string Constants::BLUR_NO_CHOOSE = "Please choose one blurring type.";
int Constants::BLUR_LIMIT = 50;

string Constants::DIFF_OPEN_DIALOG = "Choose image for comparison";

int Constants::ABOUT_DIALOG_WIDTH = 200;
int Constants::ABOUT_DIALOG_HEIGHT = 200;

Constants::Constants()
{

}
