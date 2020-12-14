#include "Noise.h"

void salt_pepper_noise(Mat& image, int min, int max)
{
    Mat noise(image.size(), image.type());
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            noise.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
        }
    }
    RNG rng;
    int r;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (rng.uniform(0, 100) >= 50) {
                for (int ch = 0; ch < 3; ++ch) {
                    r = rng.uniform(0, 256);
                    if (r < min) noise.at<Vec3b>(i, j)[ch] = min;
                    if (r > max) noise.at<Vec3b>(i, j)[ch] = max;
                }
            }
        }
    }
    image += noise;
}