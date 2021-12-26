#pragma once

/**
 * BSP fustrum culling
 * Originaly authored by DigiBen in BSP Loader 3 5/12/2002
 */

#ifndef FRUSTUM_H
#define FRUSTUM_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <array>

class Frustum {
public:
    void calculateFrustum();
    bool pointInFrustum(float x, float y, float z);
    bool sphereInFrustum(float x, float y, float z, float radius);
    bool cubeInFrustum(float x, float y, float z, float size);
    bool quadInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
private:
    std::array<std::array<float, 4>, 6> frustum;
};

#endif // FRUSTUM_H