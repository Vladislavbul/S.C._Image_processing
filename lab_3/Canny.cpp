#include "Canny.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

template<typename T>
T Clamp(T n)
{
	n = n > 255 ? 255 : n;
	return n < 0 ? 0 : n;
}

cv::Mat gray(const cv::Mat& pic) {
	int width = pic.cols;
	int height = pic.rows;
	cv::Mat result = cv::Mat(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int b = pic.at<cv::Vec3b>(i, j)[0];
			int g = pic.at<cv::Vec3b>(i, j)[1];
			int r = pic.at<cv::Vec3b>(i, j)[2];

			double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
			result.at<uchar>(i, j) = newValue;

		}
	}
	return result;
}

cv::Mat medianFilter(const cv::Mat& pic)
{
	cv::Mat result = pic.clone();
	int width = pic.cols;
	int height = pic.rows;
	uint8_t pix[9];

	for (int i = 1; i < height - 1; ++i)
	{
		for (int j = 1; j < width - 1; ++j)
		{
			pix[0] = pic.at<uint8_t>(i - 1, j - 1);
			pix[1] = pic.at<uint8_t>(i, j - 1);
			pix[2] = pic.at<uint8_t>(i + 1, j - 1);
			pix[3] = pic.at<uint8_t>(i - 1, j);
			pix[4] = pic.at<uint8_t>(i, j);
			pix[5] = pic.at<uint8_t>(i + 1, j);
			pix[6] = pic.at<uint8_t>(i - 1, j + 1);
			pix[7] = pic.at<uint8_t>(i, j + 1);
			pix[8] = pic.at<uint8_t>(i + 1, j + 1);

			std::sort(pix, pix + 9, [](const uint8_t& v1, const uint8_t& v2) -> bool
			{
				return v1 > v2;
			});
			result.at<uint8_t>(i, j) = pix[4];
		}
	}
	return result;
}

cv::Mat operatorSobel(const cv::Mat& pic, cv::Mat& anglesVectors) {

	double x1[] = { -1.0, 0, 1.0 };
	double x2[] = { -2.0, 0, 2.0 };
	double x3[] = { -1.0, 0, 1.0 };

	std::vector<std::vector<double>> xFilter(3);

	xFilter[0].assign(x1, x1 + 3);
	xFilter[1].assign(x2, x2 + 3);
	xFilter[2].assign(x3, x3 + 3);


	double y1[] = { 1.0, 2.0, 1.0 };
	double y2[] = { 0, 0, 0 };
	double y3[] = { -1.0, -2.0, -1.0 };

	std::vector<std::vector<double>> yFilter(3);

	yFilter[0].assign(y1, y1 + 3);
	yFilter[1].assign(y2, y2 + 3);
	yFilter[2].assign(y3, y3 + 3);

	int size = (int)xFilter.size() / 2;

	cv::Mat result = cv::Mat(pic.rows - 2 * size, pic.cols - 2 * size, CV_8UC1);
	anglesVectors = cv::Mat(pic.rows - 2 * size, pic.cols - 2 * size, CV_32FC1);

	for (int i = size; i < pic.rows - size; i++)
	{
		for (int j = size; j < pic.cols - size; j++)
		{
			double sumx = 0;
			double sumy = 0;

			for (int x = 0; x < xFilter.size(); x++)
				for (int y = 0; y < xFilter.size(); y++)
				{
					sumx += xFilter[x][y] * (double)(pic.at<uchar>(i + x - size, j + y - size));
					sumy += yFilter[x][y] * (double)(pic.at<uchar>(i + x - size, j + y - size));
				}
			double sumxsq = sumx * sumx;
			double sumysq = sumy * sumy;

			double sq2 = sqrt(sumxsq + sumysq);

			if (sq2 > 255)
				sq2 = 255;
			result.at<uchar>(i - size, j - size) = sq2;

			if (sumx == 0)
				anglesVectors.at<float>(i - size, j - size) = 90;
			else
				anglesVectors.at<float>(i - size, j - size) = atan(sumy / sumx);
		}
	}
	return result;
}

cv::Mat nonMax(const cv::Mat& pic, cv::Mat& anglesVectors) {

	cv::Mat result = cv::Mat(pic.rows - 2, pic.cols - 2, CV_8UC1);

	for (int i = 1; i < pic.rows - 1; i++) {
		for (int j = 1; j < pic.cols - 1; j++) {
			float Tangent = anglesVectors.at<float>(i, j);

			result.at<uchar>(i - 1, j - 1) = pic.at<uchar>(i, j);

			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
			{
				if ((pic.at<uchar>(i, j) < pic.at<uchar>(i, j + 1)) || (pic.at<uchar>(i, j) < pic.at<uchar>(i, j - 1)))
					result.at<uchar>(i - 1, j - 1) = 0;
			}

			if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
			{
				if ((pic.at<uchar>(i, j) < pic.at<uchar>(i + 1, j)) || (pic.at<uchar>(i, j) < pic.at<uchar>(i - 1, j)))
					result.at<uchar>(i - 1, j - 1) = 0;
			}

			if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
			{
				if ((pic.at<uchar>(i, j) < pic.at<uchar>(i - 1, j + 1)) || (pic.at<uchar>(i, j) < pic.at<uchar>(i + 1, j - 1)))
					result.at<uchar>(i - 1, j - 1) = 0;
			}

			if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
			{
				if ((pic.at<uchar>(i, j) < pic.at<uchar>(i + 1, j + 1)) || (pic.at<uchar>(i, j) < pic.at<uchar>(i - 1, j - 1)))
					result.at<uchar>(i - 1, j - 1) = 0;
			}
		}
	}
	return result;
}

cv::Mat doubleThresholdAndTrace(const cv::Mat& pic, uint8_t min, uint8_t max) {
	if (min > 255)
		min = 255;
	if (max > 255)
		max = 255;
	cv::Mat result = pic.clone();
	int width = pic.cols;
	int height = pic.rows;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			result.at<uchar>(i, j) = pic.at<uchar>(i, j);
			if (result.at<uchar>(i, j) > max)
				result.at<uchar>(i, j) = 255;
			else if (result.at<uchar>(i, j) < min)
				result.at<uchar>(i, j) = 0;
			else
			{
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++)
				{
					for (int y = j - 1; y < j + 2; y++)
					{
						if (x <= 0 || y <= 0 || height || y > width)
							continue;
						else
						{
							if (result.at<uchar>(x, y) > max)
							{
								result.at<uchar>(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (result.at<uchar>(x, y) <= max && result.at<uchar>(x, y) >= min)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 2; x < i + 3; x++)
					{
						for (int y = j - 1; y < j + 3; y++)
						{
							if (x < 0 || y < 0 || x > height || y > width)
								continue;
							else
							{
								if (result.at<uchar>(x, y) > max)
								{
									result.at<uchar>(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					result.at<uchar>(i, j) = 0;
			}
		}
	}
	return result;
}

cv::Mat algorithmCanny(const cv::Mat& img, cv::Mat& angleMat)
{
	cv::Mat result = img.clone();

	result = gray(result);
	result = medianFilter(result);
	result = operatorSobel(result, angleMat);
	result = nonMax(result, angleMat);
	result = doubleThresholdAndTrace(result, 200, 210);

	return result;
}
