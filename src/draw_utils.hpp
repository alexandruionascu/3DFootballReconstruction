#ifndef DRAW_UTILS_HPP
#define DRAW_UTILS_HPP

#include "raylib.h"
#include "vector_utils.hpp"

void DrawThickLine3D(Vector3 start, Vector3 end, float thickness, Color color)
{
    Vector3 direction = Vector3Subtract(end, start);
    direction = Vector3Normalize(direction);

    // Define an up vector for the horizontal plane (Y-axis)
    Vector3 up = {0.0f, 1.0f, 0.0f};

    // Calculate the perpendicular vector in the horizontal plane
    Vector3 perpendicular = Vector3CrossProduct(direction, up);
    perpendicular = Vector3Normalize(perpendicular);
    perpendicular = Vector3Scale(perpendicular, thickness / 2.0f);

    Vector3 quad[4] = {
        Vector3Subtract(start, perpendicular),
        Vector3Add(start, perpendicular),
        Vector3Add(end, perpendicular),
        Vector3Subtract(end, perpendicular)};

    DrawTriangle3D(quad[0], quad[1], quad[2], color);
    DrawTriangle3D(quad[0], quad[2], quad[3], color);
}

void DrawThickCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color, int thickness)
{
    // Loop to draw concentric circles for thickness
    for (int i = 0; i < thickness; i++)
    {
        DrawCircle3D(center, radius + i * 0.005f, rotationAxis, rotationAngle, color);
    }
}

// @deprecated
void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint)
{
    if ((texture.id <= 0) || (scale <= 0.0f))
        return; // Wanna see a infinite loop?!...just delete this line!
    if ((source.width == 0) || (source.height == 0))
        return;

    int tileWidth = (int)(source.width * scale), tileHeight = (int)(source.height * scale);
    if ((dest.width < tileWidth) && (dest.height < tileHeight))
    {
        // Can fit only one tile
        DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, ((float)dest.height / tileHeight) * source.height},
                       (Rectangle){dest.x, dest.y, dest.width, dest.height}, origin, rotation, tint);
    }
    else if (dest.width <= tileWidth)
    {
        // Tiled vertically (one column)
        int dy = 0;
        for (; dy + tileHeight < dest.height; dy += tileHeight)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, source.height}, (Rectangle){dest.x, dest.y + dy, dest.width, (float)tileHeight}, origin, rotation, tint);
        }

        // Fit last tile
        if (dy < dest.height)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                           (Rectangle){dest.x, dest.y + dy, dest.width, dest.height - dy}, origin, rotation, tint);
        }
    }
    else if (dest.height <= tileHeight)
    {
        // Tiled horizontally (one row)
        int dx = 0;
        for (; dx + tileWidth < dest.width; dx += tileWidth)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)dest.height / tileHeight) * source.height}, (Rectangle){dest.x + dx, dest.y, (float)tileWidth, dest.height}, origin, rotation, tint);
        }

        // Fit last tile
        if (dx < dest.width)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, ((float)dest.height / tileHeight) * source.height},
                           (Rectangle){dest.x + dx, dest.y, dest.width - dx, dest.height}, origin, rotation, tint);
        }
    }
    else
    {
        // Tiled both horizontally and vertically (rows and columns)
        int dx = 0;
        for (; dx + tileWidth < dest.width; dx += tileWidth)
        {
            int dy = 0;
            for (; dy + tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, source, (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, (float)tileHeight}, origin, rotation, tint);
            }

            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, dest.height - dy}, origin, rotation, tint);
            }
        }

        // Fit last column of tiles
        if (dx < dest.width)
        {
            int dy = 0;
            for (; dy + tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, (float)tileHeight}, origin, rotation, tint);
            }

            // Draw final tile in the bottom right corner
            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, dest.height - dy}, origin, rotation, tint);
            }
        }
    }
}

#endif