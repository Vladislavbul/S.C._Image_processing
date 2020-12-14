#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/hal/interface.h"
#include "iostream"
#include <vector>
#include <iostream>
#include <math.h>
#include <ctime>
#include <iostream>"

using namespace cv;

Mat geometric_mean(Mat image);

Mat median_filter(Mat& image, int rad);
int clamp(int value, int max, int min);