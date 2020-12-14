#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <ctime>

#include "Metrics.h"
#include "Gray.h"
#include "Conversion.h"

using namespace cv;
using namespace std;

int main()
{
	Mat A;
	Mat B;
	Mat C;

	//A = imread("1.jpg");
	//A = imread("1.jpg");
	A = imread("../images/1.jpg");
	//imshow("1", A);

	//B = imread("2.jpg");
	//B = imread("2.jpg");
	B = imread("../images/2.jpg");
	//imshow("2", B);

	//C = imread("3.jpg");
	//C = imread("3.jpg");
	C = imread("../images/3.jpg");
	//imshow("3", C);

	//waitKey();

	//==================== SSIM (1) ====================//
	float metric1 = SSIM(A, A);
	float metric2 = SSIM(A, B);
	float metric3 = SSIM(A, C);
	cout << "SSIM: image 1 and image 1 = " << metric1 << endl;
	cout << "SSIM: image 1 and image 2 = " << metric2 << endl;
	cout << "SSIM: image 1 and image 3 = " << metric3 << endl;
	cout << "========================================" << endl;

	//==================== Shades of gray (2) ====================//
	//===== A =====//
	Mat gray_my = A.clone();
	gray_my = shades_gray(A);
	//imshow("gray_my", gray_my);
	//waitKey();
	//imwrite("gray_my.jpg", gray_A);
	imwrite("../images/gray_my.jpg", gray_my);

	//===== B =====//
	Mat gray_CV = A.clone();
	cvtColor(gray_CV, gray_CV, COLOR_RGB2GRAY);
	//imshow("gray_CV", gray_CV);
	//waitKey();
	//imwrite("gray_CV.jpg", gray_CV);
	imwrite("../images/gray_CV.jpg", gray_CV);

	//===== C =====//
	float metric4 = SSIM(A, gray_my);
	cout << "SSIM: opencv_gray and my_gray = " << metric4 << endl;
	cout << "========================================" << endl;

	//==================== Conversion (3) ====================//
	//===== A =====//
	Mat bgr_to_YCrCb_my = bgr_to_YCrCb(A);
	//imshow("bgr_to_YCrCb_my", bgr_to_YCrCb_my);
	//waitKey();
	//imwrite("bgr_to_YCrCb (my).jpg", bgr_to_YCrCb_my);
	imwrite("../images/bgr_to_YCrCb (my).jpg", bgr_to_YCrCb_my);

	Mat YCrCb_to_bgr_my = YCrCb_to_bgr(bgr_to_YCrCb_my);
	//imshow("YCrCb_to_bgr_my", YCrCb_to_bgr_my);
	//waitKey();
	//imwrite("YCrCb_to_bgr (my).jpg", YCrCb_to_bgr_my);
	imwrite("../images/YCrCb_to_bgr (my).jpg", YCrCb_to_bgr_my);

	//===== B =====//
	Mat bgr_to_YCrCb_CV;
	cvtColor(A, bgr_to_YCrCb_CV, COLOR_BGR2YCrCb);
	//imshow("bgr_to_YCrCb_CV", bgr_to_YCrCb_CV);
	//waitKey();
	//imwrite("bgr_to_YCrCb (CV).jpg", bgr_to_YCrCb_CV);
	imwrite("../images/bgr_to_YCrCb (CV).jpg", bgr_to_YCrCb_CV);

	Mat YCrCb_to_bgr_CV;
	cvtColor(bgr_to_YCrCb_CV, YCrCb_to_bgr_CV, COLOR_YCrCb2BGR);
	//imshow("YCrCb_to_bgr_CV", YCrCb_to_bgr_CV);
	//waitKey();
	//imwrite("YCrCb_to_bgr (CV).jpg", YCrCb_to_bgr_CV);
	imwrite("../images/YCrCb_to_bgr (CV).jpg", YCrCb_to_bgr_CV);

	//===== C =====//
	float metric6 = SSIM(bgr_to_YCrCb_my, bgr_to_YCrCb_CV);
	float metric5 = SSIM(A, bgr_to_YCrCb_CV);
	float metric8 = SSIM(bgr_to_YCrCb_my, A);
	float metric7 = SSIM(YCrCb_to_bgr_my, YCrCb_to_bgr_CV);
	cout << "SSIM: BGR_to_YCrCb (my) and BGR_to_YCrCb (CV) = " << metric6 << endl;
	cout << "SSIM: YCrCb_to_BGR (my) and Original_image = " << metric5 << endl;
	cout << "SSIM: YCrCb_to_BGR (CV) and Original_image = " << metric8 << endl;
	cout << "SSIM: YCrCb_to_BGR (my) and YCrCb_to_BGR (CV) = " << metric7 << endl;
	cout << "========================================" << endl;

	//===== D =====//
	clock_t seconds1 = clock();
	Mat bgr_brightness = brightness_bgr(A);
	clock_t seconds2 = clock();
	imwrite("../images/bgr_brightness.jpg", bgr_brightness);

	clock_t seconds3 = clock();
	Mat YCrCb_brightness = brightness_YCrCb(bgr_to_YCrCb_CV);
	clock_t seconds4 = clock();
	imwrite("../images/YCrCb_brightness.jpg", YCrCb_brightness);

	//===== E =====//
	float metric9 = SSIM(A, bgr_brightness);
	float metric10 = SSIM(bgr_to_YCrCb_CV, YCrCb_brightness);
	cout << "SSIM: Original_image and bgr_brightness = " << metric9 << endl;
	cout << "SSIM: BGR_to_YCrCb (CV) and YCrCb_brightness = " << metric10 << endl;
	cout << "========================================" << endl;
	cout << "Time bgr_brightness = " << (double)(seconds2 - seconds1) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "Time YCrCb_brightnesss = " << (double)(seconds4 - seconds3) / CLOCKS_PER_SEC << " sec" << endl << endl;

	system("pause");
}	