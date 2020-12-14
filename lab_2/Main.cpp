#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <ctime>
#include <iostream>

#include "Filter_noise.h"
#include "Noise.h"

using namespace cv;
using namespace std;

float SSIM(const Mat& image1, const Mat& image2)
{
    Mat gray_image1;
    Mat gray_image2;

    cvtColor(image1, gray_image1, COLOR_BGR2GRAY);
    cvtColor(image2, gray_image2, COLOR_BGR2GRAY);

    int N = image1.rows * image1.cols;

    double ma = 0, mb = 0, sigma_a = 0, sigma_b = 0, cov = 0;

    for (int i = 0; i < gray_image1.rows; i++)
    {
        for (int j = 0; j < gray_image1.cols; j++)
        {
            ma += gray_image1.at<uchar>(i, j);
            mb += gray_image2.at<uchar>(i, j);
        }
    }

    ma /= N;
    mb /= N;

    for (int i = 0; i < gray_image1.rows; i++)
    {
        for (int j = 0; j < gray_image1.cols; j++)
        {
            sigma_a += pow(gray_image1.at<uchar>(i, j) - ma, 2);
            sigma_b += pow(gray_image2.at<uchar>(i, j) - mb, 2);
            cov += abs((gray_image1.at<uchar>(i, j) - ma) * (gray_image2.at<uchar>(i, j) - mb));
        }
    }

    int n = N - 1;
    sigma_a /= n;
    sigma_b /= n;
    cov /= n;

    float c1 = 0.01 * 255 * 0.01 * 255; // 6.5025
    float c2 = 0.03 * 255 * 0.03 * 255; // 58.5225
    double c3 = c2 / 2;

    double l = (2 * ma * mb + c1) / (pow(ma, 2) + pow(mb, 2) + c1);
    double c = (2 * sqrt(sigma_a) * sqrt(sigma_b) + c2) / (sigma_a + sigma_b + c2);
    double s = (cov + c3) / (sqrt(sigma_a) * sqrt(sigma_b) + c3);

    double result = l * c * s;
    return result;
}

int main(int argc, char** argv)
{
    Mat image, noise;

    image = imread("../images/1.jpg");
    noise = image.clone();

    //==========Add noise==========//
    //Salt and Pepper noise
    int min = 0;
    int max = 50;
    salt_pepper_noise(noise, min, max);
    imwrite("../images/s.p_noise.jpg", noise);

    //==========Denoising==========//
    //Median
    Mat denoising_median;
    clock_t s1 = clock();
    denoising_median = median_filter(noise, 1);
    clock_t s2 = clock();
    imwrite("../images/denoising_median.jpg", denoising_median);

    //Geometric mean
    Mat denoising_geom;
    clock_t s3 = clock();
    denoising_geom = geometric_mean(noise);
    clock_t s4 = clock();
    imwrite("../images/denois_geom.jpg", denoising_geom);

    //OpenCV - GaussianBlur
    Mat denoising_CV;
    clock_t s5 = clock();
    GaussianBlur(noise, denoising_CV, Size(5, 5), 0, 0);
    clock_t s6 = clock();
    imwrite("../images/denois_CV.jpg", denoising_CV);

    //==========Сomparison==========//
    float m4 = SSIM(image, noise);
    cout << endl << "\tSSIM: Original image and Noise = " << m4 << endl << endl;

    cout << "\tTime of my Median filter = " << (double)(s2 - s1) / CLOCKS_PER_SEC << " seconds" << endl;
    float m1 = SSIM(image, denoising_median);
    cout << "\tSSIM: Original image and Median filter = " << m1 << endl << endl;

    cout << "\tTime of my Geometric mean filter = " << (double)(s4 - s3) / CLOCKS_PER_SEC << " seconds" << endl;
    float m2 = SSIM(image, denoising_geom);
    cout << "\tSSIM: Original image and Geometric mean filter = " << m2 << endl << endl;

    cout << "\tTime of OpenCV Gaussian filter = " << (double)(s6 - s5) / CLOCKS_PER_SEC << " seconds" << endl;
    float m3 = SSIM(image, denoising_CV);
    cout << "\tSSIM: Original image and OpenCV Gaussian filter = " << m3 << endl << endl;

    cout << "=============================================================================" << endl;

    system("pause");
}