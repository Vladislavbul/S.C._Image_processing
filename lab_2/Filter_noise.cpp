#include "Filter_noise.h"

Mat geometric_mean(Mat image)
{
    Mat result;
    image.copyTo(result);
    int Radx = 2;
    int Rady = 2;
    for (int x = 0; x < image.cols; x++) {
        for (int y = 0; y < image.rows; y++) {
            double result_R = 1;
            double result_G = 1;
            double result_B = 1;
            for (int i = -Rady; i < Rady; i++) {
                for (int j = -Radx; j < Radx; j++) {
                    int X = std::min(x + j, image.cols - 1);
                    X = std::max(X, 0);
                    int Y = std::min(y + i, image.rows - 1);
                    Y = std::max(Y, 0);
                    Vec3b bgr = image.at<Vec3b>(Y, X);
                    if (bgr[2]) {
                        result_R *= bgr[2];
                    }
                    if (bgr[1]) {
                        result_G *= bgr[1];
                    }
                    if (bgr[0]) {
                        result_B *= bgr[0];
                    }
                }
                int a = pow(result_B, 1.0 / (9 * Radx * Rady));
                result.at<Vec3b>(y, x)[0] = std::min(pow(result_B, 1.0 / (16)), 255.0);
                result.at<Vec3b>(y, x)[1] = std::min(pow(result_G, 1.0 / (16)), 255.0);
                result.at<Vec3b>(y, x)[2] = std::min(pow(result_R, 1.0 / (16)), 255.0);
            }
        }
    }
    return result;
}

int clamp(int val, int max, int min)
{
    if (val > max) {
        return max;
    }
    else if (val < min) {
        return min;
    }
    else {
        return val;
    }
}

Mat median_filter(Mat& image, int rad)
{
    int r = image.rows;
    int c = image.cols;
    Mat result = image.clone();
    int diam = 2 * rad + 1;
    int d = diam * diam;
    std::vector<int> R(d), B(d), G(d);

    for (int x = 0; x < r; x++)
    {
        for (int y = 0; y < c; y++)
        {
            for (int i = -rad; i <= rad; i++)
            {
                for (int j = -rad; j <= rad; j++)
                {
                    int id = (i + rad) * diam + j + rad;
                    Vec3b color = result.at<Vec3b>(clamp(x + j, r - 1, 0), clamp(y + i, c - 1, 0));
                    B[id] = color[0];
                    G[id] = color[1];
                    R[id] = color[2];
                }
            }

            sort(R.begin(), R.end());
            sort(B.begin(), B.end());
            sort(G.begin(), G.end());

            int center_R = R[(d) / 2];
            int center_B = B[(d) / 2];
            int center_G = G[(d) / 2];
            
            Vec3b cl = { static_cast<unsigned char>(center_B), static_cast<unsigned char>(center_G), static_cast<unsigned char>(center_R) };
            result.at<Vec3b>(x, y) = cl;
        }
    }
    return result;
}
