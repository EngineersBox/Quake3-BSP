#include "frustum.hpp"
#include <cmath>
#include <algorithm>
#include <array>

enum FrustumSide {
    RIGHT = 0,
    LEFT = 1,
    BOTTOM = 2,
    TOP = 3,
    BACK = 4,
    FRONT = 5
};

enum PlaneData{
    A = 0, // Plane x normal
    B = 1, // Plane y normal
    C = 2, // Plane z normal
    D = 3  // Dist from plane to origin
};

void normalizePlane(std::array<std::array<float, 4>, 6> frustum, int side) {
    float magnitude = 1.0f / (float)sqrt(
        frustum[side][A] * frustum[side][A]
        + frustum[side][B] * frustum[side][B]
        + frustum[side][C] * frustum[side][C]
    );

    frustum[side][A] *= magnitude;
    frustum[side][B] *= magnitude;
    frustum[side][C] *= magnitude;
    frustum[side][D] *= magnitude;
}


void Frustum::calculateFrustum() {
    float proj[16];
    float modl[16];
    float clip[16];

    glGetFloatv(GL_PROJECTION_MATRIX, proj);
    glGetFloatv(GL_MODELVIEW_MATRIX, modl);

    clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
    clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
    clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
    clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

    clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
    clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
    clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
    clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

    clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
    clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
    clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

    this->frustum[RIGHT][A] = clip[3] - clip[0];
    this->frustum[RIGHT][B] = clip[7] - clip[4];
    this->frustum[RIGHT][C] = clip[11] - clip[8];
    this->frustum[RIGHT][D] = clip[15] - clip[12];

    normalizePlane(this->frustum, RIGHT);

    this->frustum[LEFT][A] = clip[3] + clip[0];
    this->frustum[LEFT][B] = clip[7] + clip[4];
    this->frustum[LEFT][C] = clip[11] + clip[8];
    this->frustum[LEFT][D] = clip[15] + clip[12];

    normalizePlane(this->frustum, LEFT);

    this->frustum[BOTTOM][A] = clip[3] + clip[1];
    this->frustum[BOTTOM][B] = clip[7] + clip[5];
    this->frustum[BOTTOM][C] = clip[11] + clip[9];
    this->frustum[BOTTOM][D] = clip[15] + clip[13];

    normalizePlane(this->frustum, BOTTOM);

    this->frustum[TOP][A] = clip[3] - clip[1];
    this->frustum[TOP][B] = clip[7] - clip[5];
    this->frustum[TOP][C] = clip[11] - clip[9];
    this->frustum[TOP][D] = clip[15] - clip[13];

    normalizePlane(this->frustum, TOP);

    this->frustum[BACK][A] = clip[3] - clip[2];
    this->frustum[BACK][B] = clip[7] - clip[6];
    this->frustum[BACK][C] = clip[11] - clip[10];
    this->frustum[BACK][D] = clip[15] - clip[14];

    normalizePlane(this->frustum, BACK);

    this->frustum[FRONT][A] = clip[3] + clip[2];
    this->frustum[FRONT][B] = clip[7] + clip[6];
    this->frustum[FRONT][C] = clip[11] + clip[10];
    this->frustum[FRONT][D] = clip[15] + clip[14];

    normalizePlane(this->frustum, FRONT);
}

bool Frustum::pointInFrustum(float x, float y, float z) {
    return std::none_of(
        this->frustum.begin(),
        this->frustum.end(),
        [x,y,z](const std::array<float, 4> &i){
            return i[A] * x + i[B] * y + i[C] * z + i[D] <= 0;
        }
    );
}


bool Frustum::sphereInFrustum(float x, float y, float z, float radius) {
    return std::none_of(
        this->frustum.begin(),
        this->frustum.end(),
        [x,y,z, radius](const std::array<float, 4> &i){
            return i[A] * x + i[B] * y + i[C] * z + i[D] <= -radius;
        }
    );
}

bool Frustum::cubeInFrustum(float x, float y, float z, float size) {
    return std::all_of(
        this->frustum.begin(),
        this->frustum.end(),
        [x,y,z, size](const std::array<float, 4> &i){
            return (i[A] * (x - size) + i[B] * (y - size) + i[C] * (z - size) + i[D] > 0)
                || (i[A] * (x + size) + i[B] * (y - size) + i[C] * (z - size) + i[D] > 0)
                || (i[A] * (x - size) + i[B] * (y + size) + i[C] * (z - size) + i[D] > 0)
                || (i[A] * (x + size) + i[B] * (y + size) + i[C] * (z - size) + i[D] > 0)
                || (i[A] * (x - size) + i[B] * (y - size) + i[C] * (z + size) + i[D] > 0)
                || (i[A] * (x + size) + i[B] * (y - size) + i[C] * (z + size) + i[D] > 0)
                || (i[A] * (x - size) + i[B] * (y + size) + i[C] * (z + size) + i[D] > 0)
                || (i[A] * (x + size) + i[B] * (y + size) + i[C] * (z + size) + i[D] > 0);
        }
    );
}

bool Frustum::quadInFrustum(float x, float y, float z, float x2, float y2, float z2) {
    return std::all_of(
        this->frustum.begin(),
        this->frustum.end(),
        [x,sizeX,y,sizeY,z,sizeZ](const std::array<float, 4> &i){
            return (i[A] * x + i[B] * y + i[C] * z + i[D] > 0)
                || (i[A] * sizeX + i[B] * y + i[C] * z + i[D] > 0)
                || (i[A] * x + i[B] * sizeY + i[C] * z + i[D] > 0)
                || (i[A] * sizeX + i[B] * sizeY + i[C] * z + i[D] > 0)
                || (i[A] * x + i[B] * y + i[C] * sizeZ + i[D] > 0)
                || (i[A] * sizeX + i[B] * y + i[C] * sizeZ + i[D] > 0)
                || (i[A] * x + i[B] * sizeY + i[C] * sizeZ + i[D] > 0)
                || (i[A] * sizeX + i[B] * sizeY + i[C] * sizeZ + i[D] > 0);
        }
    );
}