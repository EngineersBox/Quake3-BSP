#include "level.hpp"

#define EPSILON 0.03125f
#define TRACE_TYPE_RAY 0
#define TRACE_TYPE_SPHERE 1

Level::Level(const char* mapFilePath) {
	if (readMap(mapFilePath, this->map)) {
		spdlog::info("Loaded map {0}", mapFilePath);
		convertEdges();
		generateLightmaps();
		generateAlbedos();
	} else {
		spdlog::error("Could not load map {0}", mapFilePath);
	}
}

Level::~Level() {
	freeMap(this->map);
}

void Level::convertEdges() {
	float fTemp;

	for (QVertex& mVertex : this->map.mVertices) {
		fTemp = mVertex.mPosition[1];
        mVertex.mPosition[1] = mVertex.mPosition[2];
        mVertex.mPosition[2] = -fTemp;

		fTemp = mVertex.mNormal[1];
        mVertex.mNormal[1] = mVertex.mNormal[2];
        mVertex.mNormal[2] = -fTemp;

		fTemp = mVertex.mTexCoord[1][0];
        mVertex.mTexCoord[1][0] = mVertex.mTexCoord[1][1];
        mVertex.mTexCoord[1][1] = fTemp;
	}

	for (QPlane& mPlane : this->map.mPlanes) {
		fTemp = mPlane.mNormal[1];
		mPlane.mNormal[1] = mPlane.mNormal[2];
		mPlane.mNormal[2] = -fTemp;
	}

	for (QFace& mFace : this->map.mFaces) {
		fTemp = mFace.mNormal[1];
		mFace.mNormal[1] = mFace.mNormal[2];
		mFace.mNormal[2] = -fTemp;
	}

	int iTemp;
	for (QLeaf& mLeaf : this->map.mLeaves) {
		iTemp = mLeaf.mMins[1];
        mLeaf.mMins[1] = mLeaf.mMins[2];
        mLeaf.mMins[2] = -iTemp;
		iTemp = mLeaf.mMaxs[1];
        mLeaf.mMaxs[1] = mLeaf.mMaxs[2];
        mLeaf.mMaxs[2] = -iTemp;
	}
}

void Level::onCollision(Element* element) {}

void Level::onDraw(Camera* camera) {
	int leafIndex;
	int cluster;
	int nLeaves = this->map.mLeaves.size();

	glDisable(GL_COLOR_MATERIAL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	leafIndex = findLeaf(camera->position);
	cluster = this->map.mLeaves[leafIndex].mCluster;

	this->frustum.calculateFrustum();
	this->triangles = 0;
	while (nLeaves--) {
		QLeaf* leaf = &this->map.mLeaves[nLeaves];
		if (!clusterVisible(cluster, leaf->mCluster)) continue;
		if (!this->frustum.boxInFrustum(
			(float)leaf->mMins[0],
			(float)leaf->mMins[1],
			(float)leaf->mMins[2],
			(float)leaf->mMaxs[0],
			(float)leaf->mMaxs[1],
			(float)leaf->mMaxs[2])) continue;
		int nFaces = leaf->mNbLeafFaces;
		while (nFaces--) {
			int faceIndex = this->map.mLeafFaces[leaf->mLeafFace + nFaces].mFaceIndex;
			if (this->map.mFaces[faceIndex].mType == 1) {
				drawTriangleVA(faceIndex);
			}
			if (this->map.mFaces[faceIndex].mType == 3) {
				drawTriangleMesh(faceIndex);
			}
			this->triangles++;
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Level::drawTriangle(int faceIndex) {
    if (this->map.mFaces[faceIndex].mLightmapIndex >= 0) {
        glBindTexture(GL_TEXTURE_2D, this->lightmaps[this->map.mFaces[faceIndex].mLightmapIndex].id);
        glEnable(GL_TEXTURE_2D);
    }
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(this->map.mFaces[faceIndex].mNormal[0], this->map.mFaces[faceIndex].mNormal[1], this->map.mFaces[faceIndex].mNormal[2]);
    for (int i = this->map.mFaces[faceIndex].mVertex; i < (this->map.mFaces[faceIndex].mVertex + this->map.mFaces[faceIndex].mNbVertices); i++) {
        glTexCoord2f(this->map.mVertices[i].mTexCoord[1][0], this->map.mVertices[i].mTexCoord[1][1]);
        glColor3ubv(this->map.mVertices[i].mColor);
        glNormal3fv(this->map.mVertices[i].mNormal);
        glVertex3fv(this->map.mVertices[i].mPosition);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

inline void Level::bindLightmapAndTexture(int faceIndex) {
    if (this->map.mFaces[faceIndex].mTextureIndex >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[faceIndex].mTexCoord[0]));
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->albedos[this->map.mFaces[faceIndex].mTextureIndex].id);
    }
    if (this->map.mFaces[faceIndex].mLightmapIndex >= 0) {
        glActiveTexture(GL_TEXTURE1);
        glClientActiveTexture(GL_TEXTURE1);
        glTexCoordPointer(2, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[faceIndex].mTexCoord[1]));
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->albedos[this->map.mFaces[faceIndex].mLightmapIndex].id);
    }
}

void Level::drawTriangleVA(int faceIndex) {
    bindLightmapAndTexture(faceIndex);
    glVertexPointer(3, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mPosition));
    glNormalPointer(GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[1].mNormal));
    glDrawArrays(GL_TRIANGLE_FAN, this->map.mFaces[faceIndex].mVertex, this->map.mFaces[faceIndex].mNbVertices);
    glDisable(GL_TEXTURE_2D);
}

void Level::drawTriangleMesh(int faceIndex) {
    bindLightmapAndTexture(faceIndex);
    glVertexPointer(3, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mPosition));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(QVertex), &(this->map.mVertices[0].mColor));
    glTexCoordPointer(2, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mTexCoord[1]));
    glDrawElements(GL_TRIANGLES, this->map.mFaces[faceIndex].mNbMeshVertices, GL_UNSIGNED_INT, &(this->map.mMeshVertices[this->map.mFaces[faceIndex].mMeshVertex]));
    glDisable(GL_TEXTURE_2D);
}

int Level::findLeaf(const glm::vec3 &cameraPos) {
    double dist;
    int index = 0;
    while (index >= 0) {
        const QNode& node = this->map.mNodes[index];
        const QPlane& plane = this->map.mPlanes[node.mPlane];
        dist = plane.mNormal[0] * cameraPos.x + plane.mNormal[1] * cameraPos.y + plane.mNormal[2] * cameraPos.z - plane.mDistance;
        index = node.mChildren[dist < 0];
    }
    return - index - 1;
}

bool Level::clusterVisible(int actual, int leafCluster) {
    if (!this->map.mVisData.mBuffer || actual < 0) return 1;
    unsigned char visibilitySet = this->map.mVisData.mBuffer[(actual * this->map.mVisData.mBytesPerCluster) + (leafCluster >> 3)];
    return (visibilitySet & (1 << ((leafCluster) & 7))) != 0;
}

constexpr int TEX_WIDTH = 128;
constexpr float BRIGHTNESS_SCALAR = 255.0;

void Level::generateLightmaps() {
    float r,g,b,temp,scale;
    float brightness = 0;
    int c;

    this->lightmaps.resize(this->map.mLightMaps.size());
    for (int i = 0; i < this->lightmaps.size(); i++) {
        this->lightmaps[i].data = new GLubyte[TEX_WIDTH * TEX_WIDTH * 3];
        this->lightmaps[i].height = TEX_WIDTH;
        this->lightmaps[i].width = TEX_WIDTH;
        c = 0;
        for (int y = 0; y < TEX_WIDTH; y++) {
            for (int x = 0; x < TEX_WIDTH; x++) {
                temp = 0;
                scale = 1.0f;
                r = ((float) this->map.mLightMaps[i].mMapData[x][y][0]) * brightness / BRIGHTNESS_SCALAR;
                g = ((float) this->map.mLightMaps[i].mMapData[x][y][1]) * brightness / BRIGHTNESS_SCALAR;
                b = ((float) this->map.mLightMaps[i].mMapData[x][y][2]) * brightness / BRIGHTNESS_SCALAR;
                if (r > 1.0f && (temp = (1.0f / r)) < scale) scale = temp;
                if (g > 1.0f && (temp = (1.0f / g)) < scale) scale = temp;
                if (b > 1.0f && (temp = (1.0f / b)) < scale) scale = temp;
                scale *= BRIGHTNESS_SCALAR;
                this->lightmaps[i].data[c] = (unsigned char) (r * scale);
                this->lightmaps[i].data[c + 1] = (unsigned char) (g * scale);
                this->lightmaps[i].data[c + 2] = (unsigned char) (b * scale);
                c += 3;
            }
        }
        glGenTextures(1, &this->lightmaps[i].id);
        glBindTexture(GL_TEXTURE_2D, this->lightmaps[i].id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            GL_RGBA,
            this->lightmaps[i].width,
            this->lightmaps[i].height,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            this->lightmaps[i].data
        );
    }
}

void Level::generateAlbedos() {
    std::string temp,temp2,temp3;
    char *route;

    int c;
    this->albedos.resize(this->map.mTextures.size());
    for(int i = 0; i < this->albedos.size(); i++) {
        c=0;

        while (this->map.mTextures[i].mName[c] != 0){
            if (this->map.mTextures[i].mName[c]=='/') {
                temp.push_back('/');
                temp.push_back('/');
            } else {
                temp.push_back(this->map.mTextures[i].mName[c]);
            }
            c++;
        }
        temp2 = temp + ".tga";
        temp3 = temp + ".jpg";


        if (fopen(temp2.data(),"r")) {
            route = new char[temp2.size() + 1];
            strcpy(route, temp2.c_str());
            this->albedos[i].load(route);
            delete[] route;
        }
        if (fopen(temp3.data(),"r")) {
            route = new char[temp3.size() + 1];
            strcpy(route, temp3.c_str());
            this->albedos[i].load(route);
            delete[] route;

        }

        temp.clear();

        glGenTextures(1, &this->albedos[i].id);
        glBindTexture(GL_TEXTURE_2D, this->albedos[i].id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            GL_RGBA,
            this->albedos[i].width,
            this->albedos[i].height,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            this->albedos[i].data
        );
    }
}

void Level::checkNode(int nodeIndex, float fractionInitial, float fractionFinal, glm::vec3 pinical, glm::vec3 pFinal) {
    if (nodeIndex < 0) {
        QLeaf* leaf = &this->map.mLeaves[-(nodeIndex + 1)];
        for (int i = 0; i < leaf->mNbLeafBrushes; i++) {
            QBrush* brush = &this->map.mBrushes[this->map.mLeafBrushes[leaf->mLeafBrush + i].mBrushIndex];
            if (brush->mNbBrushSides > 0 && this->map.mTextures[brush->mTextureIndex].mContents & 1) {
                checkBrush(brush, pinical, pFinal);
            }
        }
        return;
    }
    const QNode* node = &this->map.mNodes[nodeIndex];
    const QPlane* plane = &this->map.mPlanes[node->mPlane];
    float startDist = (plane->mNormal[0] * pinical.x + plane->mNormal[1] * pinical.y + plane->mNormal[2] * pinical.z) - plane->mDistance;
    float endDist = (plane->mNormal[0] * pFinal.x + plane->mNormal[1] * pFinal.y + plane->mNormal[2] * pFinal.z) - plane->mDistance;
    float offset = 0;
    if (this->traceType == TRACE_TYPE_SPHERE) {
        offset = this->sphereRadius;
    }
    if (this->traceType == TRACE_TYPE_RAY) {
        offset = 0.0;
    }
    if (startDist >= offset && endDist >= offset) {
        checkNode(node->mChildren[0], fractionInitial, fractionFinal, pinical, pFinal);
        return;
    } else if (startDist < -offset && endDist < -offset) {
        checkNode(node->mChildren[1], fractionInitial, fractionFinal, pinical, pFinal);
        return;
    }
    int side = 0;
    float fraction1 = 1.0f;
    float fraction2 = 0.0f;
    float halfFraction = 0.0f;
    glm::vec3 half;
    if (startDist < endDist) {
        side = 1;
        float inverseDistance = 1.0f / (startDist - endDist);
        fraction1 = (startDist + offset + EPSILON) * inverseDistance;
        fraction2 = (startDist - offset - EPSILON) * inverseDistance;
    }
    if (fraction1 < 0.0f) {
        fraction1 = 0.0f;
    } else if (fraction2 > 1.0f) {
        fraction1 = 1.0f;
    }
    if (fraction2 < 0.0f) {
        fraction2 = 0.0f;
    } else if (fraction2 > 1.0f) {
        fraction2 = 1.0f;
    }
    halfFraction = fractionInitial + (fractionFinal - fractionInitial) * fraction1;
    half.x = pinical.x + fraction1 * (pFinal.x - pinical.x);
    half.y = pinical.y + fraction1 * (pFinal.y - pinical.y);
    half.z = pinical.z + fraction1 * (pFinal.z - pinical.z);
    checkNode(node->mChildren[side], fractionInitial, halfFraction, pinical, half);
    halfFraction = fractionInitial + (fractionFinal - fractionInitial) * fraction2;
    half.x = pinical.x + fraction2 * (pFinal.x - pinical.x);
    half.y = pinical.y + fraction2 * (pFinal.y - pinical.y);
    half.z = pinical.z + fraction2 * (pFinal.z = pinical.z);
    if (side == 1) {
        checkNode(node->mChildren[0], halfFraction, fractionFinal, half, pFinal);
    } else {
        checkNode(node->mChildren[1], halfFraction, fractionFinal, half, pFinal);
    }
}

void Level::checkBrush(QBrush* brush, glm::vec3 vOrigin, glm::vec3 vFinal) {
    float fractionOrigin = -1.0f;
    float fractionEnd = 1.0f;
    bool notAtOrigin = false;
    float offset = 0;
    float originDist;
    float endDist;
    if (this->traceType == TRACE_TYPE_SPHERE) {
        offset = this->sphereRadius;
    } else if (this->traceType == TRACE_TYPE_RAY) {
        offset = 0.0f;
    }
    for (int i = 0; i < brush->mNbBrushSides; i++) {
        QBrushSide* brushSide = &this->map.mBrushSides[brush->mBrushSide + i];
        QPlane* plane = &this->map.mPlanes[brushSide->mPlaneIndex];

        originDist = (plane->mNormal[0] * vOrigin.x + plane->mNormal[1] * vOrigin.y + plane->mNormal[2] * vOrigin.z) - (plane->mDistance + offset);
        endDist = (plane->mNormal[0] * vFinal.x + plane->mNormal[1] * vFinal.y + plane->mNormal[2] * vFinal.z) - (plane->mDistance + offset);
        if (originDist > 0) {
            notAtOrigin = true;
        }
        if (originDist > endDist) {
            float fraction1 = (originDist - EPSILON) / (originDist - endDist);
            if (fraction1 > fractionOrigin) {
                fractionOrigin = fraction1;
                this->collision = true;
            }
        } else {
            float fraction1 = (originDist + EPSILON) / (originDist - endDist);
            if (fraction1 < fractionEnd) {
                fractionEnd = fraction1;
            }
        }
    }
    if (!notAtOrigin) {
        return;
    }
    if (fractionOrigin < fractionEnd && fractionOrigin > -1 && fractionOrigin < this->outputFraction) {
        if (fractionOrigin < 0) {
            fractionOrigin = 0;
        }
        this->outputFraction = fractionOrigin;
    }
}

glm::vec3 Level::trace(glm::vec3 start, glm::vec3 end) {
    this->collision = false;
    glm::vec3 temp;
    this->outputFraction = 1.0f;
    checkNode(0, 0.0f, 1.0f, start, end);
    if (this->outputFraction == 1.0f) {
        return end;
    } else {
        this->collision = true;
        temp.x = start.x + this->outputFraction * (end.x - start.x);
        temp.y = start.y + this->outputFraction * (end.y - start.y);
        temp.z = start.z + this->outputFraction * (end.z - start.z);
    }
    return temp;
}

glm::vec3 Level::traceRay(glm::vec3 start, glm::vec3 end) {
    this->traceType = TRACE_TYPE_RAY;
    return trace(start, end);
}

glm::vec3 Level::traceSphere(glm::vec3 start, glm::vec3 end, float radius) {
    this->sphereRadius = radius;
    this->traceType = TRACE_TYPE_SPHERE;
    return trace(start, end);
}