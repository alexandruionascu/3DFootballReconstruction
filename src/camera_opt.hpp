#ifndef CAMERA_OPT_HPP
#define CAMERA_OPT_HPP

#include "raylib.h"
#include <opencv2/opencv.hpp>
#include <limits>
#include <random>


// Define the camera parameters struct
struct CameraParameters
{
    float fov;
    Vector3 position;
    Vector3 rotation;
};

// Abstract base class for the optimizer
class CameraOptimizer
{
public:
    virtual void Optimize(const cv::Mat &synthetic, const cv::Mat &real, int iterations) = 0;
    CameraParameters GetBestParams() const { return bestParams; }
    float GetBestScore() const { return bestScore; }

protected:
    CameraParameters bestParams;
    float bestScore;

    virtual float Evaluate(const CameraParameters &params, const cv::Mat &synthetic, const cv::Mat &real) = 0;
    virtual cv::Mat RenderSyntheticImage(const CameraParameters &params) = 0;
    virtual float CompareImages(const cv::Mat &img1, const cv::Mat &img2) = 0;
};

// Define the random search optimizer class
class RandomSearchOptimizer : public CameraOptimizer
{
public:
    RandomSearchOptimizer()
        : fovDist(30.0f, 120.0f),
          positionDist(-10.0f, 10.0f),
          rotationDist(-M_PI, M_PI),
          generator(std::random_device{}())
    {
        bestScore = std::numeric_limits<float>::max();
    }

    void Optimize(const cv::Mat &synthetic, const cv::Mat &real, int iterations) override
    {
        for (int i = 0; i < iterations; ++i)
        {
            CameraParameters params;
            params.fov = fovDist(generator);
            params.position = {positionDist(generator), positionDist(generator), positionDist(generator)};
            params.rotation = {rotationDist(generator), rotationDist(generator), rotationDist(generator)};

            float score = Evaluate(params, synthetic, real);
            if (score < bestScore)
            {
                bestScore = score;
                bestParams = params;
                std::cout << "New best score: " << bestScore << std::endl;
            }
        }
    }

protected:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> fovDist;
    std::uniform_real_distribution<float> positionDist;
    std::uniform_real_distribution<float> rotationDist;

    float Evaluate(const CameraParameters &params, const cv::Mat &synthetic, const cv::Mat &real) override
    {
        cv::Mat rendered = RenderSyntheticImage(params);
        return CompareImages(rendered, real);
    }

    cv::Mat RenderSyntheticImage(const CameraParameters &params) override
    {
        // Initialize Raylib window and camera
        int screenWidth = 800;
        int screenHeight = 600;
        InitWindow(screenWidth, screenHeight, "RenderSyntheticImage");

        Camera camera = {0};
        camera.position = params.position;
        camera.target = {0.0f, 0.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = params.fov;
        // camera.type = CAMERA_PERSPECTIVE;

        // Set camera rotation
        camera.target.x = camera.position.x + cos(params.rotation.y) * cos(params.rotation.x);
        camera.target.y = camera.position.y + sin(params.rotation.x);
        camera.target.z = camera.position.z + sin(params.rotation.y) * cos(params.rotation.x);

        // Main rendering loop
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        // Your function to draw the pitch

        EndMode3D();

        EndDrawing();

        // Take a screenshot and convert it to OpenCV Mat
        TakeScreenshot("screenshot.png");
        // Load the screenshot with OpenCV
        cv::Mat synthetic = cv::imread("screenshot.png", cv::IMREAD_GRAYSCALE);

        CloseWindow();

        return synthetic;
    }

    float CompareImages(const cv::Mat &img1, const cv::Mat &img2) override
    {
        // Simple example comparison using Mean Squared Error
        cv::Mat diff;
        cv::absdiff(img1, img2, diff);
        diff.convertTo(diff, CV_32F);
        diff = diff.mul(diff);
        cv::Scalar s = cv::sum(diff);
        return s[0];
    }
};


#endif
