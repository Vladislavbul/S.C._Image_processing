#include <vector>

#include <opencv2/core/core.hpp>

class Hough
{
public:
	Hough();
	virtual ~Hough();
	int Transform(unsigned char* img_data, int width, int height);
	const unsigned int* Accumulator(int* width, int* height);
	std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > Lines(int threshold);
private:
	unsigned int* _accu;
	int _accu_width;
	int _accu_height;
	int _img_width;
	int _img_height;
};

cv::Mat lineHough(const cv::Mat& img, const cv::Mat& canny);
