#ifndef PCD_HPP
#define PCD_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <opencv2/opencv.hpp>
#include "raylib.h"

struct PointXYZRGB {
    float x, y, z;
    uint8_t r, g, b;
};

std::vector<PointXYZRGB> readBinaryPCD(const std::string& fileName) {
    std::vector<PointXYZRGB> cloud;
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Couldn't open file " << fileName << std::endl;
        return cloud;
    }

    std::string line;
    bool headerParsed = false;
    int pointCount = 0;
    while (std::getline(file, line) && !headerParsed) {
        if (line.substr(0, 6) == "FIELDS") {
            // Ensure the file contains the correct fields (x, y, z, rgb)
            if (line.find("x") == std::string::npos || line.find("y") == std::string::npos ||
                line.find("z") == std::string::npos || line.find("rgb") == std::string::npos) {
                std::cerr << "PCD file does not contain necessary fields (x, y, z, rgb)" << std::endl;
                return cloud;
            }
        } else if (line.substr(0, 6) == "POINTS") {
            pointCount = std::stoi(line.substr(7));
        } else if (line.substr(0, 4) == "DATA") {
            headerParsed = true;
        }
    }

    if (!headerParsed) {
        std::cerr << "Invalid PCD file: header not parsed correctly" << std::endl;
        return cloud;
    }

    cloud.resize(pointCount);
    file.read(reinterpret_cast<char*>(cloud.data()), pointCount * sizeof(PointXYZRGB));

    return cloud;
}



std::vector<PointXYZRGB> downscalePointCloud(const std::vector<PointXYZRGB>& cloud, float targetSize) {
    float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max(), minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min(), maxY = std::numeric_limits<float>::min(), maxZ = std::numeric_limits<float>::min();

    for (const auto& point : cloud) {
        if (point.x < minX) minX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.z < minZ) minZ = point.z;
        if (point.x > maxX) maxX = point.x;
        if (point.y > maxY) maxY = point.y;
        if (point.z > maxZ) maxZ = point.z;
    }

    float scale = std::min({targetSize / (maxX - minX), targetSize / (maxY - minY), targetSize / (maxZ - minZ)});

    std::vector<PointXYZRGB> downscaledCloud = cloud;
    for (auto& point : downscaledCloud) {
        point.x = (point.x - minX) * scale;
        point.y = (point.y - minY) * scale;
        point.z = (point.z - minZ) * scale;
    }

    return downscaledCloud;
}


void DrawPointCloud(cv::Mat image)
{
    BeginDrawing();
    // ClearBackground(RAYWHITE);

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            cv::Vec3b intensity = image.at<cv::Vec3b>(y, x);
            Color color = {intensity[2], intensity[1], intensity[0], 255}; // OpenCV uses BGR, Raylib uses RGB
            DrawPixel(x, y, color);
        }
    }

    EndDrawing();
}

#endif // PCD_HPP
