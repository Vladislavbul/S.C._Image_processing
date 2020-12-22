#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Hough.h"
#include "Circle.h"
#include "Canny.h"


int main()
{
	cv::Mat matAngle;
	cv::Mat lines = cv::imread("img3.jpg");
	cv::Mat resultCannyLines = algorithmCanny(lines, matAngle);
	cv::imshow("Canny", resultCannyLines);

	cv::Mat resultOpenCV;
	cv::Canny(lines, resultOpenCV, 100, 130, 3);
	cv::imshow("OpenCV Canny", resultOpenCV);
	
	cv::Mat resultLineHough = lineHough(lines, resultCannyLines);
	cv::imshow("Line Hough Method", resultLineHough);

	cv::Mat circles = cv::imread("img7.jpg");
	cv::Mat resultCircles = algorithmCanny(circles, matAngle);
	cv::Mat resultCirclesHough = circleHough(circles, resultCircles);
	cv::imshow("Circle Hough Method", resultCirclesHough);
	
	cv::waitKey(0);
	return 0;
}