#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "../element.hpp"
#include "../../../resource/material/texture.hpp"

class Terrain: public Element {
private:
    int width;
    float widthScale;
    float terrainMul;
    float heightMul;
    float scanDepth;
    float textureMul;
    float rangedRandom(float v1, float v2);
    void normalizeTerrain(float field[], int size);
    void filterHeightBand(float* band, int stride, int count, float filter);
    void filterHeightField(float field[], int size, float filter);
    void makeTerrainPlasma(float field[], int size, float rough);
protected:
    void onAnimate(float deltaTime) override {}
    void onDraw(Camera* camera) override;
    void onCollision(Element* element) override;
public:
    float* heightMap;
    Material::Texture terrainTexture[5];

    Terrain();
    Terrain(int _width, float rFactor);
    ~Terrain() override { delete this->heightMap; }

    void load() override {}
    void unload() override {}
    void buildTerrain(int _width, float rFactor);
    int getWidth() { return this->width; }
    float getHeight(float x, float z);
    float getMul() { return this->terrainMul; }
    float getScanDepth() { return this->scanDepth; }
};

#endif // TERRAIN_H