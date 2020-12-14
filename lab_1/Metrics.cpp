#include "Metrics.h"

using namespace cv;

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
