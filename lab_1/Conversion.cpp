#include "Conversion.h"

using namespace cv;

Mat bgr_to_YCrCb(const Mat& image)
{
    Mat image_YCrCb = image.clone();
    float Y = 0.0, Cb = 0.0, Cr = 0.0;

    for (int i = 0; i < image_YCrCb.rows; i++)
    {
        for (int j = 0; j <  image_YCrCb.cols; j++)
        {
            Y = image_YCrCb.at<Vec3b>(i, j).val[0] * 0.114 + image_YCrCb.at<Vec3b>(i, j).val[1] * 0.587 + image_YCrCb.at<Vec3b>(i, j).val[2] * 0.299;
            Cb = 128 + image_YCrCb.at<Vec3b>(i, j).val[0] * 0.5 - image_YCrCb.at<Vec3b>(i, j).val[1] * 0.331264 - image_YCrCb.at<Vec3b>(i, j).val[2] * 0.168736;
            Cr = 128 - image_YCrCb.at<Vec3b>(i, j).val[0] * 0.081312 - image_YCrCb.at<Vec3b>(i, j).val[1] * 0.418688 + image_YCrCb.at<Vec3b>(i, j).val[2] * 0.5;

            image_YCrCb.at<Vec3b>(i, j).val[0] = Y;
            image_YCrCb.at<Vec3b>(i, j).val[1] = Cr;
            image_YCrCb.at<Vec3b>(i, j).val[2] = Cb;
        }
    }
    return  image_YCrCb;
}

Mat YCrCb_to_bgr(const Mat& image)
{
    Mat image_BGR = image.clone();
    float B = 0.0, G = 0.0, R = 0.0;

    for (int i = 0; i < image_BGR.rows; i++)
    {
        for (int j = 0; j < image_BGR.cols; j++)
        {
            R = image.at<Vec3b>(i, j).val[0] + 1.4022 * (image.at<Vec3b>(i, j).val[1] - 128.0);
            G = image.at<Vec3b>(i, j).val[0] - 0.3456 * (image.at<Vec3b>(i, j).val[2] - 128.0) - 0.7145 * (image.at<Vec3b>(i, j).val[1] - 128.0);
            B = image.at<Vec3b>(i, j).val[0] + 1.7710 * (image.at<Vec3b>(i, j).val[2] - 128.0);

            image_BGR.at<Vec3b>(i, j).val[0] = B;
            image_BGR.at<Vec3b>(i, j).val[1] = G;
            image_BGR.at<Vec3b>(i, j).val[2] = R;
        }
    }
    return image_BGR;
}

Mat brightness_bgr(const Mat& image)
{
    Mat image_bright = image.clone();

    for (int i = 0; i < image_bright.rows; i++)
    {
        for (int j = 0; j < image_bright.cols; j++)
        {
            if (image_bright.at<Vec3b>(i, j).val[0] + 100 > 255)
            {
                image_bright.at<Vec3b>(i, j).val[0] = 255;
            }
            else
            {
                image_bright.at<Vec3b>(i, j).val[0] += 100;
            }

            if (image_bright.at<Vec3b>(i, j).val[1] + 100 > 255)
            {
                image_bright.at<Vec3b>(i, j).val[1] = 255;
            }
            else
            {
                image_bright.at<Vec3b>(i, j).val[1] += 100;
            }

            if (image_bright.at<Vec3b>(i, j).val[2] + 100 > 255)
            {
                image_bright.at<Vec3b>(i, j).val[2] = 255;
            }
            else
            {
                image_bright.at<Vec3b>(i, j).val[2] += 100;
            }
        }
    }
    return image_bright;    
}

Mat brightness_YCrCb(const Mat& image)
{
    Mat image_bright = image.clone();

    for (int i = 0; i < image_bright.rows; i++)
    {
        for (int j = 0; j < image_bright.cols; j++)
        {
            if (image_bright.at<Vec3b>(i, j).val[0] + 100 > 255)
            {
                image_bright.at<Vec3b>(i, j).val[0] = 255;
            }
            else
            {
                image_bright.at<Vec3b>(i, j).val[0] += 100;
            }
        }
    }
    return image_bright;
}