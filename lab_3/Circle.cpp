#define _USE_MATH_DEFINES

#include "Circle.h"

#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef DEG2RAD
	#define DEG2RAD 0.017453293f
#endif

HoughCircle::HoughCircle() :_accu(nullptr), _accu_width(0), _accu_height(0), _img_width(0), _img_height(0)
{

}

HoughCircle::~HoughCircle() {
	if (_accu != nullptr)
		delete[] _accu;
}

int HoughCircle::Transform(unsigned char* img_data, int width, int height, int r)
{
	_r = r;

	_img_width = width;
	_img_height = height;
	_accu_height = height;
	_accu_width = width;

	if (_accu != nullptr)
		delete[] _accu;

	_accu = new unsigned int[_accu_height * width]();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (img_data[(y * width) + x] > 250)
			{
				for (int t = 1; t <= 360; t++)
				{
					int a = ((double)x - ((double)_r * cos((double)t * DEG2RAD)));
					int b = ((double)y - ((double)_r * sin((double)t * DEG2RAD)));

					if ((b >= 0 && b < _accu_height) && (a >= 0 && a < _accu_width))
						_accu[(b * _accu_width) + a]++;
				}
			}
		}
	}

	return 0;
}

const unsigned int* HoughCircle::Accumulator(int* width, int* height)
{
	*width = _accu_width;
	*height = _accu_height;

	return _accu;
}

int HoughCircle::Circles(int threshold, std::vector< std::pair< std::pair<int, int>, int> >& result)
{
	int found = 0;

	if (_accu == 0)
		return found;

	for (int b = 0; b < _accu_height; b++)
	{
		for (int a = 0; a < _accu_width; a++)
		{
			if ((int)_accu[(b * _accu_width) + a] >= threshold)
			{
					
				int max = _accu[(b * _accu_width) + a];
				for (int ly = -4; ly <= 4; ly++)
				{
					for (int lx = -4; lx <= 4; lx++)
					{
						if ((ly + b >= 0 && ly + b < _accu_height) && (lx + a >= 0 && lx + a < _accu_width))
						{
							if ((int)_accu[((b + ly) * _accu_width) + (a + lx)] > max)
							{
								max = _accu[((b + ly) * _accu_width) + (a + lx)];
								ly = lx = 5;
							}
						}
					}
				}
				if (max > (int)_accu[(b * _accu_width) + a])
					continue;

				result.push_back(std::pair< std::pair<int, int>, int>(std::pair<int, int>(a, b), _r));
				found++;

			}
		}
	}

	return found;
}

struct SortCirclesDistance
{
	bool operator()(const std::pair< std::pair<int, int>, int>& a, const std::pair< std::pair<int, int>, int>& b)
	{
		int d = sqrt(pow(abs(a.first.first - b.first.first), 2) + pow(abs(a.first.second - b.first.second), 2));
		if (d <= a.second + b.second)
		{
			return a.second > b.second;
		}
		return false;
	}

};

cv::Mat circleHough(const cv::Mat& img, const cv::Mat& canny)
{
	int w = canny.cols;
	int h = canny.rows;

	HoughCircle hough;

	std::vector< std::pair< std::pair<int, int>, int> > circles;
	cv::Mat img_accu;
	for (int r = 19; r < h / 2; r = r + 1)
	{
		hough.Transform(canny.data, w, h, r);

		int	threshold = 0.95 * (2.0 * (double)r * M_PI);

		{
			hough.Circles(threshold, circles);

			int aw, ah, maxa;
			aw = ah = maxa = 0;
			const unsigned int* accu = hough.Accumulator(&aw, &ah);

			for (int p = 0; p < (ah * aw); p++)
			{
				if ((int)accu[p] > maxa)
					maxa = accu[p];
			}
			double contrast = 1.0;
			double coef = 255.0 / (double)maxa * contrast;
			img_accu = cv::Mat(ah, aw, CV_8UC3);
			for (int p = 0; p < (ah * aw); p++)
			{
				unsigned char c = (double)accu[p] * coef < 255.0 ? (double)accu[p] * coef : 255.0;
				img_accu.data[(p * 3) + 0] = 255;
				img_accu.data[(p * 3) + 1] = 255 - c;
				img_accu.data[(p * 3) + 2] = 255 - c;
			}
		}
	}

	std::sort(circles.begin(), circles.end(), SortCirclesDistance());
	int a, b, r;
	a = b = r = 0;
	std::vector< std::pair< std::pair<int, int>, int> > result;
	//std::vector< std::pair< std::pair<int, int>, int> >::iterator it;
	for (auto it = circles.begin(); it != circles.end(); it++)
	{
		int d = sqrt(pow(abs(it->first.first - a), 2) + pow(abs(it->first.second - b), 2));
		if (d > it->second + r)
		{
			result.push_back(*it);
			a = it->first.first;
			b = it->first.second;
			r = it->second;
		}
	}

	cv::Mat img_res = img.clone();
	for (auto it = result.begin(); it != result.end(); it++)
	{
		cv::circle(img_res, cv::Point(it->first.first, it->first.second), it->second, cv::Scalar(0, 0, 255), 2, 8);
	}

	return img_res;
}