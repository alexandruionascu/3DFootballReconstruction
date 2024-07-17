#ifndef CV_UTILS_HPP
#define CV_UTILS_HPP

#include <opencv2/opencv.hpp>

cv::Mat extractWhiteLines(const cv::Mat &image)
{
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Apply a binary threshold to get the white lines
    cv::Mat thresh;
    cv::threshold(gray, thresh, 115, 255, cv::THRESH_BINARY);

    // Morphological operations to clean up the threshold image
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat clean;
    cv::morphologyEx(thresh, clean, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(clean, clean, cv::MORPH_OPEN, kernel);

    return clean;
}

cv::Mat dilateWhiteLines(const cv::Mat &image, int dilationSize = 10)
{
    // Create a kernel for dilation
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dilated;
    cv::dilate(image, dilated, kernel);

    return dilated;
}

#endif // CV_UTILS_HPP