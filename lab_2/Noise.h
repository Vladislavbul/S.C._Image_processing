#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/hal/interface.h"

using namespace cv;

void salt_pepper_noise(Mat& image, int min, int max);