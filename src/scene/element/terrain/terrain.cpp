#include "terrain.hpp"
#include <random>

Terrain::Terrain() {
    this->width = 256;
    this->scanDepth = 80.0;
    this->terrainMul = 50.0;
    this->textureMul = 0.25;
    this->widthScale = 5.0;
    this->heightMul = 175.0;
    this->fogColor[0] = 0.75f;
    this->fogColor[1] = 0.9f;
    this->fogColor[2] = 1.0f;
    this->fogColor[3] = 1.0f;

    this->position = glm::vec3(0,0,0);
    this->velocity = glm::vec3(0,0,0);
    this->acceleration = glm::vec3(0,0,0);
    this->size = this->width * this->terrainMul * 2;

    buildTerrain(this->width, 0.5f);
}

Terrain::Terrain(int _width, float rFactor) {
    this->width = _width;
    this->scanDepth = 80.0;
    this->terrainMul = 50.0;
    this->textureMul = 0.25;
    this->widthScale = 5.0;
    this->heightMul = 175.0;
    this->fogColor[0] = 0.75f;
    this->fogColor[1] = 0.9f;
    this->fogColor[2] = 1.0f;
    this->fogColor[3] = 1.0f;

    this->position = glm::vec3(0,0,0);
    this->velocity = glm::vec3(0,0,0);
    this->acceleration = glm::vec3(0,0,0);
    this->size = this->width * this->terrainMul * 2;

    buildTerrain(this->width, rFactor);
}

void Terrain::buildTerrain(int _width, float rFactor) {
    this->width = _width;
    this->heightMap = new float[this->width * this->width];
    makeTerrainPlasma(this->heightMap, this->width, rFactor);
    this->terrainTexture[0].load("textures/ground.tga"); // TODO make this provisionable via args
    glGenTextures(1, &this->terrainTexture[0].id);
    glBindTexture(GL_TEXTURE_2D, this->terrainTexture[0].id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int mode = (this->terrainTexture[0].bitDepth == 24) ? GL_RGB : GL_RGBA;
    gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        mode,
        this->terrainTexture[0].width,
        this->terrainTexture[0].height,
        mode,
        GL_UNSIGNED_BYTE,
        this->terrainTexture[0].data
    );
}

void Terrain::onCollision(Element *element) {}

void Terrain::onDraw(Camera* camera) {
    glEnable(GL_DEPTH_TEST);

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, this->fogColor);
    glFogf(GL_FOG_START, this->scanDepth * 0.1f);
    glFogf(GL_FOG_END, this->scanDepth * 2);
    glHint(GL_FOG_HINT, GL_FASTEST);
//    glEnable(GL_FOG);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.0);
    glDisable(GL_ALPHA_TEST);
    // push/pop objects that move with the camera (e.g. a sun, the sky)
    //glTranslatef(camera->x, camera->y, camera->z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->terrainTexture[0].id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->terrainTexture[0].id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glColor3f(1.0, 1.0, 1.0);

    int z, x;
    for (z = (int)(camera->position.z / this->terrainMul - this->scanDepth), z = z < 0 ? 0 : z; (z < camera->position.z / this->terrainMul + this->scanDepth) && z < this->width-1; z++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (x = (int)(camera->position.x / this->terrainMul - this->scanDepth), x = x < 0 ? 0 : x; (x < camera->position.x / this->terrainMul + this->scanDepth) && x < this->width-1; x++) {
            glTexCoord2f(this->textureMul * (float) x, this->textureMul * (float) z);
            glVertex3f((float) x * this->textureMul, (float) this->heightMap[x + z * this->width] * this->heightMul, (float) z * this->terrainMul);

            glTexCoord2f(this->textureMul * (float)(x + 1), this->textureMul * (float) z);
            glVertex3f((float)(x + 1) * this->terrainMul, (float) this->heightMap[x + 1 + z * this->width] * this->heightMul, (float) z * this->terrainMul);

            glTexCoord2f(this->textureMul * (float) x, this->textureMul * (float)(z + 1));
            glVertex3f((float) x * this->terrainMul, (float) this->heightMap[x + (z + 1) * this->width] * this->heightMul, (float)(z + 1) * this->terrainMul);

            glTexCoord2f(this->textureMul * (float)( x + 1), this->textureMul * (float)(z + 1));
            glVertex3f((float)(x + 1) * this->terrainMul, (float) this->heightMap[x + 1 + (z + 1) * this->width] * this->heightMul, (float)(z + 1) * this->terrainMul);
        }
        glEnd();
    }
}

double normalizedUniformRand() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}

float Terrain::rangedRandom(float v1, float  v2) {
    return v1 + (v2 - v1) * normalizedUniformRand();
}

void Terrain::normalizeTerrain(float field[], int size) {
    float maxVal = field[0];
    float minVal = field[0];
    for (int i = 1; i < (size * size); i++) {
        if (field[i] > maxVal) {
            maxVal = field[i];
        } else if (field[i] < minVal) {
            minVal = field[i];
        }
    }
    if (maxVal <= minVal) return;
    float dh = maxVal - minVal;
    for (int i = 0; i < (size * size); i++) {
        field[i] = (field[i] - minVal) / dh;
    }
}

void Terrain::filterHeightBand(float* band, int stride, int count, float filter) {
    int j = stride;
    float v = band[0];
    for (int i = 0; i < count - 1; i++) {
        band[j] = filter * v + (1 - filter) * band[j];
        v = band[j];
        j += stride;
    }
}

void Terrain::filterHeightField(float field[], int size, float filter) {
    for (int i = 0; i < size; i++) {
        filterHeightBand(&field[size * i], 1, size, filter);
    }
    for (int i = 0; i < size; i++) {
        filterHeightBand(&field[size * i + size - 1], -1, size, filter);
    }
    for (int i = 0; i < size; i++) {
        filterHeightBand(&field[i], size, size, filter);
    }
    for (int i = 0; i < size; i++) {
        filterHeightBand(&field[size * (size - 1) + i], -size, size, filter);
    }
}

void Terrain::makeTerrainPlasma(float field[], int size, float rough) {
    int i,j,ni,nj,mi,mj,pmi,pmj,rectSize = size;
    float dh = (float) rectSize / 2;
    float r = (float) pow(2.0f, (int) -1 * rough);
    field[0] = 0;

    while(rectSize > 0) {
        for (i = 0; i < size; i += rectSize) {
            for (j = 0; j < size; j+= rectSize) {
                ni = (i + rectSize) % size;
                nj = (j + rectSize) % size;

                mi = i + rectSize / 2;
                mj = j + rectSize / 2;

                field[mi + mj * size] = (field[i + j * size] + field[ni + j * size] + field[i + nj * size] + field[ni + nj * size]) / 4 + rangedRandom(-dh / 2,dh / 2);
            }
        }
        for (i = 0; i < size; i += rectSize) {
            for (j = 0; j < size; j += rectSize) {
                ni = (i + rectSize) % size;
                nj = (j+rectSize) % size;

                mi = i + rectSize / 2;
                mj = j + rectSize / 2;

                pmi = (i - rectSize / 2 + size) % size;
                pmj = (j - rectSize / 2 + size) % size;

                field[mi + j * size] = (field[i + j * size] + field[ni + j * size] + field[mi + pmj * size] + field[mi + mj * size]) / 4 + rangedRandom(-dh / 2,dh / 2);
                field[i + mj * size] = (field[i + j * size] + field[i + nj * size] + field[pmi + mj * size] + field[mi + mj * size]) / 4 + rangedRandom(-dh / 2,dh / 2);

            }
        }
        rectSize /= 2;
        dh *= r;
    }
    normalizeTerrain(field, size);
}

float Terrain::getHeight(float x, float z) {
    float projCameraX = x / this->terrainMul;
    float projCameraZ = z / this->terrainMul;

    int col0 = (int) projCameraX;
    int row0 = (int) projCameraZ;
    int col1 = col0 + 1;
    int row1 = row0 + 1;

    if (col1 > this->width) {
        col1 = 0;
    }
    if (row1 > this->width) {
        row1 = 0;
    }

    float h00 = this->heightMul * (float) this->heightMap[col0 + row0 * this->width];
    float h01 = this->heightMul * (float) this->heightMap[col1 + row0 * this->width];
    float h10 = this->heightMul * (float) this->heightMap[col0 + row1 * this->width];
    float h11 = this->heightMul * (float) this->heightMap[col1 + row1 * this->width];

    float tx = projCameraX - (float) col0;
    float ty = projCameraZ - (float) row0;
    float txty = tx * ty;
    return h00 * (1.0f - ty - tx + txty)
            + h01 * (tx - txty)
            + h10 * (ty - txty)
            + h11 * txty;
}