#include "raylib.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>
#include "pcd.hpp"
#include "camera_opt.hpp"
#include "pitch.hpp"
#include "cv_utils.hpp"


#define GLSL_VERSION 330

using namespace std;
using namespace cv;


int main()
{

    /*** Example PCD.hpp usage
    auto cloud = readBinaryPCD("./resources/pcd/ramos_1.pcd");
    cloud = downscalePointCloud(cloud, 100.0f);
    ***/

    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;


    Mat image = imread("frames/26857.png");
    Mat resizedImage;
    resize(image, resizedImage, Size(screenWidth, screenHeight));
    cv::Mat realLines = extractWhiteLines(image);
    cv::Mat dilated = dilateWhiteLines(realLines);

    /*** Show OpenCV Overlay Window
    cv::imshow("Overlay", dilated);
    ***/

    // Set configuration flags for better visuals
    // SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "3D Football Pitch");
    // Load the grass texture
    Texture2D pitchTexture = LoadTexture("resources/textures/grass.png");

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 15.0f, 30.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 5.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    // camera.type = CAMERA_PERSPECTIVE;

    Texture2D texture = LoadTexture("resources/textures/grass_court.png");

    Mesh mesh = GenMeshPlane(10.0f, 10.0f, 4, 4);

    float repeat = 10.0f; // Number of times to repeat the texture
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        mesh.texcoords[i * 2] *= repeat;     // Scale U coordinate
        mesh.texcoords[i * 2 + 1] *= repeat; // Scale V coordinate
    }

    Model model = LoadModelFromMesh(mesh);
    SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // SetCameraMode(camera, CAMERA_ORTHOGRAPHIC); // Set a free camera mode

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        float deltaTime = GetFrameTime();

        // Draw
        BeginDrawing();
        ClearBackground(BLACK); // Set background color to sky blue

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        BeginMode3D(camera);

        DrawFootballPitch(pitchTexture);
        
        // Draw the pitch model
        int gridSize = 20;
        float spacing = 10.0f;
        for (int x = -gridSize / 2; x < gridSize / 2; x++)
        {
            for (int z = -gridSize / 2; z < gridSize / 2; z++)
            {
                DrawModel(model, (Vector3){x * spacing, 0, z * spacing}, 1.0f, WHITE);
            }
        }

        DrawGrid(200.0f, 2.5f);

        // Draw WIP sky
        // DrawCube(Vector3{0, 50, 0}, 400, 100, 400, SKYBLUE);

        /*** Draw Point Cloud
        for (const auto &point : cloud)
        {
            Color color = (Color){point.r, point.g, point.b, 255};
            DrawCube((Vector3){point.x, point.y, point.z}, 0.105f, 0.105f, 0.105f, color);
        }
        ***/

        // Draw a ball at the center of the pitch 
        DrawSphere({0, 0, 0}, 1.0f, WHITE);

        EndMode3D();

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(pitchTexture); // Unload the grass texture
    CloseWindow();

    return 0;
}
