// 01Hello.cpp : OpenCV installation verifying

#include <direct.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
    #pragma comment(lib, "opencv_world440d.lib")
#else
    #pragma comment(lib, "opencv_world440.lib")
#endif //  DEBUG

using namespace std;

int main()
{
    char buf[_MAX_PATH];
    _getcwd(buf, _MAX_PATH);

    string strTitleIn("Sample");
    string strTitleOut("OutX");
    string strPath(buf);
    strPath += R"(/front.jpg)";

    cv::Mat image = cv::imread(strPath);
    cv::namedWindow(strTitleIn, cv::WINDOW_AUTOSIZE);

    double f = 1500;
    cv::Size rc(800, 450);

    cv::Mat k = (cv::Mat_<double>(3, 3) << f, 0.0, 400.0, 0.0, f, 225.0, 0.0, 0.0, 1.0);

    cv::Mat d0 = (cv::Mat_<double>(1, 4) << 0, 0, 0, 0);
    cv::Mat d = (cv::Mat_<float>(1, 4) << -3.6f, -0.0f, 1.0f, 0.5f);
    
    

    cv::Mat outX;
    cv::fisheye::undistortImage(image, outX, k, d, k, rc);

    cv::namedWindow(strTitleOut, cv::WINDOW_AUTOSIZE);

    cv::imshow(strTitleIn, image);
    cv::imshow(strTitleOut, outX);

    cv::waitKey(0);
    cv::destroyWindow(strTitleIn);
    cv::destroyWindow(strTitleOut);


    // cv::Size size = inputImage.size();
    // Bitmap bitmap(size.width, size.height, inputImage.step1(), PixelFormat24bppRGB, inputImage.data);
    // return bitmap;

    return 0;
}
