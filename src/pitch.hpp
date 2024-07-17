#ifndef PITCH_HPP
#define PITCH_HPP

#include "raylib.h"
#include "draw_utils.hpp"

void DrawFootballPitch(Texture2D grassTexture)
{
    // Pitch dimensions
    float pitchWidth = 105.0f;
    float pitchHeight = 0.0f;
    float pitchLength = 68.0f;

    // Colors
    Color lineColor = WHITE;

    // Draw pitch ground with grass texture
    Rectangle sourceRec = {0, 0, (float)grassTexture.width, (float)grassTexture.height};
    Vector2 grassTextureSize = {static_cast<float>(grassTexture.width), static_cast<float>(grassTexture.height)};

    for (float x = -pitchWidth / 2; x < pitchWidth / 2; x += grassTextureSize.x)
    {
        for (float z = -pitchLength / 2; z < pitchLength / 2; z += grassTextureSize.y)
        {
            // Calculate texture position in 3D space
            Vector3 texturePosition3D = {x + grassTextureSize.x / 2, 0.0f, z + grassTextureSize.y / 2};

            // Draw texture on the ground plane
            DrawTexturePro(grassTexture, sourceRec, Rectangle{texturePosition3D.x, texturePosition3D.z, grassTextureSize.x, grassTextureSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
    }

    // Draw pitch boundary lines
    DrawCubeWires(Vector3{0, 0, 0}, pitchWidth, pitchHeight, pitchLength, lineColor);
    // DrawThickLine3D(Vector3{pitchWidth/2, 0, -pitchLength/2}, Vector3{pitchWidth/2, 0, pitchLength/2}, 50, WHITE);
    DrawPlane(Vector3{pitchWidth / 2, 0.05f, 0}, Vector2{0.5f, pitchLength}, WHITE);
    DrawPlane(Vector3{-pitchWidth / 2, 0.05f, 0}, Vector2{0.5f, pitchLength}, WHITE);
    DrawPlane(Vector3{0, 0.05f, pitchLength / 2}, Vector2{pitchWidth, 0.5f}, WHITE);
    DrawPlane(Vector3{0, 0.05f, -pitchLength / 2}, Vector2{pitchWidth, 0.5f}, WHITE);

    DrawPlane(Vector3{0, 0.05f, 0}, Vector2{0.5f, pitchLength}, WHITE);

    // Center circle
    DrawThickCircle3D(Vector3{0, 0, 0}, 9.15f, Vector3{1, 0, 0}, 90.0f, lineColor, 50.50f);

    // Center line
    DrawLine3D(Vector3{0, 0, -pitchLength / 2}, Vector3{0, 0, pitchLength / 2}, lineColor);

    // Penalty areas
    DrawLine3D(Vector3{-pitchWidth / 2, 0, -pitchLength / 4}, Vector3{-pitchWidth / 2 + 16.5f, 0, -pitchLength / 4}, lineColor);
    DrawLine3D(Vector3{-pitchWidth / 2 + 16.5f, 0, -pitchLength / 4}, Vector3{-pitchWidth / 2 + 16.5f, 0, pitchLength / 4}, lineColor);
    DrawLine3D(Vector3{-pitchWidth / 2 + 16.5f, 0, pitchLength / 4}, Vector3{-pitchWidth / 2, 0, pitchLength / 4}, lineColor);

    DrawLine3D(Vector3{pitchWidth / 2, 0, -pitchLength / 4}, Vector3{pitchWidth / 2 - 16.5f, 0, -pitchLength / 4}, lineColor);
    DrawLine3D(Vector3{pitchWidth / 2 - 16.5f, 0, -pitchLength / 4}, Vector3{pitchWidth / 2 - 16.5f, 0, pitchLength / 4}, lineColor);
    DrawLine3D(Vector3{pitchWidth / 2 - 16.5f, 0, pitchLength / 4}, Vector3{pitchWidth / 2, 0, pitchLength / 4}, lineColor);

    // Goals
    DrawLine3D(Vector3{-pitchWidth / 2, 0, -5}, Vector3{-pitchWidth / 2 - 5, 0, -5}, lineColor);
    DrawLine3D(Vector3{-pitchWidth / 2 - 5, 0, -5}, Vector3{-pitchWidth / 2 - 5, 0, 5}, lineColor);
    DrawLine3D(Vector3{-pitchWidth / 2 - 5, 0, 5}, Vector3{-pitchWidth / 2, 0, 5}, lineColor);

    DrawLine3D(Vector3{pitchWidth / 2, 0, -5}, Vector3{pitchWidth / 2 + 5, 0, -5}, lineColor);
    DrawLine3D(Vector3{pitchWidth / 2 + 5, 0, -5}, Vector3{pitchWidth / 2 + 5, 0, 5}, lineColor);
    DrawLine3D(Vector3{pitchWidth / 2 + 5, 0, 5}, Vector3{pitchWidth / 2, 0, 5}, lineColor);
}

#endif // PITCH_HPP