#ifndef VECTOR_UTILS_HPP
#define VECTOR_UTILS_HPP

#include "raylib.h"
#include <cmath>

Vector3 Vector3Add(Vector3 v1, Vector3 v2)
{
    return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// Function to subtract two vectors
Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)
{
    return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// Function to normalize a vector
Vector3 Vector3Normalize(Vector3 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0)
        length = 1.0f;
    return (Vector3){v.x / length, v.y / length, v.z / length};
}

// Function to scale a vector
Vector3 Vector3Scale(Vector3 v, float scale)
{
    return (Vector3){v.x * scale, v.y * scale, v.z * scale};
}
// Function to calculate the cross product of two vectors
Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)
{
    return (Vector3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

#endif // VECTOR_UTILS_HPP