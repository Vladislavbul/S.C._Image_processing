#include "Gray.h"

using namespace cv;

Mat shades_gray (const Mat& image)
{
	Mat gray_image = image.clone();

	for (int i = 0; i < gray_image.rows; i++)
	{
		for (int j = 0; j < gray_image.cols; j++)
		{
			uchar gray = 0.0722 * gray_image.at<Vec3b>(i, j)[0] + 0.7152 * gray_image.at<Vec3b>(i, j)[1] + 0.2126 * gray_image.at<Vec3b>(i, j)[2];
			gray_image.at<Vec3b>(i, j) = { gray, gray, gray };
		}
	}
	return gray_image;
}