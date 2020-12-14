#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

Mat bgr_to_YCrCb(const Mat& image);
Mat YCrCb_to_bgr(const Mat& image);
Mat brightness_bgr(const Mat& image);
Mat brightness_YCrCb(const Mat& image);