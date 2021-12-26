#include "level.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include "../../utils/arrayUtils.hpp"

#define EPSILON 0.03125f
#define TRACE_TYPE_RAY 0
#define TRACE_TYPE_SPHERE 1

Level::Level(const char* mapFilePath) {
	if (readMap(mapFilePath, this->map)) {
		spdlog::info("Loaded map {0}", mapFilePath);
		convertEdges();
		generateLightmaps();
		generateAlbedos();
        spdlog::info("Initialised lightmaps and textures");
	} else {
		spdlog::error("Could not load map: {0}", mapFilePath);
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
	int nLeaves = this->map.mLeaves.size();

	glDisable(GL_COLOR_MATERIAL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, this->fogColor);
    glFogf(GL_FOG_START, this->fogStart);
    glFogf(GL_FOG_END, this->fogEnd);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glEnable(GL_FOG);

	int leafIndex = findLeaf(camera->position);
	int cluster = this->map.mLeaves[leafIndex].mCluster;

	this->frustum.calculateFrustum();
	this->triangles = 0;
	while (nLeaves--) {
		QLeaf* leaf = &this->map.mLeaves[nLeaves];
		if (!clusterVisible(cluster, leaf->mCluster)) continue;
		if (!this->frustum.quadInFrustum(
            (float) leaf->mMins[0],
            (float) leaf->mMins[1],
            (float) leaf->mMins[2],
            (float) leaf->mMaxs[0],
            (float) leaf->mMaxs[1],
            (float) leaf->mMaxs[2]
            )) continue;
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

inline void Level::bindLightmapAndTexture(int faceIndex) {
    if (this->map.mFaces[faceIndex].mTextureIndex >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mTexCoord[0]));
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->albedos[this->map.mFaces[faceIndex].mTextureIndex].id);
    }
// TODO: Fix this lightmapping
//    if (this->map.mFaces[faceIndex].mLightmapIndex >= 0) {
//        glActiveTexture(GL_TEXTURE1);
//        glClientActiveTexture(GL_TEXTURE1);
//        glTexCoordPointer(2, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mTexCoord[1]));
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, this->lightmaps[this->map.mFaces[faceIndex].mLightmapIndex].id);
//    }
}

void Level::drawTriangleVA(int faceIndex) {
    bindLightmapAndTexture(faceIndex);
    glVertexPointer(3, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mPosition));
    glNormalPointer(GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[0].mNormal));
    glDrawArrays(GL_TRIANGLE_FAN, this->map.mFaces[faceIndex].mVertex, this->map.mFaces[faceIndex].mNbVertices);
    glDisable(GL_TEXTURE_2D);
}

void Level::drawTriangleMesh(int faceIndex) {
    bindLightmapAndTexture(faceIndex);
    glVertexPointer(3, GL_FLOAT, sizeof(QVertex), &(this->map.mVertices[this->map.mFaces[faceIndex].mVertex].mPosition));
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
    return -index - 1;
}

bool Level::clusterVisible(int actual, int leafCluster) const {
    if (!this->map.mVisData.mBuffer || actual < 0) return true;
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
            for (auto &x : this->map.mLightMaps[i].mMapData) {
                scale = 1.0f;
                r = ((float) x[y][0]) * brightness / BRIGHTNESS_SCALAR;
                g = ((float) x[y][1]) * brightness / BRIGHTNESS_SCALAR;
                b = ((float) x[y][2]) * brightness / BRIGHTNESS_SCALAR;
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
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                this->lightmaps[i].bitDepth == 24 ? GL_RGB : GL_RGBA,
                this->lightmaps[i].width,
                this->lightmaps[i].height,
                0,
                this->lightmaps[i].bitDepth == 24 ? GL_RGB : GL_RGBA,
                GL_UNSIGNED_BYTE,
                this->lightmaps[i].data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
}



void Level::generateAlbedos() {
    spdlog::debug(
        "Using supported image file extensions: [{0}]",
        ArrayUtils::join(
            std::vector<std::string_view>(
                Material::SUPPORTED_FILE_EXTENSIONS.begin(),
                Material::SUPPORTED_FILE_EXTENSIONS.end()
            ),
            ", "
        )
    );
    std::string fileName,fileAndExt;
    char *path;

    int c;
    this->albedos.resize(this->map.mTextures.size());
    for(int i = 0; i < this->albedos.size(); i++) {
        c=0;

        while (this->map.mTextures[i].mName[c] != 0){
            if (this->map.mTextures[i].mName[c]=='/') {
                fileName.push_back('/');
                fileName.push_back('/');
            } else {
                fileName.push_back(this->map.mTextures[i].mName[c]);
            }
            c++;
        }
        bool opened = false;
        for (auto &ext : Material::SUPPORTED_FILE_EXTENSIONS) {
            fileAndExt = fileName + std::string(ext);
            if (fopen(fileAndExt.data(), "r")) {
                spdlog::info("Loading texture: {}", fileAndExt);
                path = new char[fileAndExt.size() + 1];
                strcpy(path, fileAndExt.c_str());
                this->albedos[i].load(path);
                delete[] path;
                opened = true;
            }
        }
        if (!opened) {
            spdlog::warn("Could not find \"{0}\" in any supported format, defaulting to NOT_FOUND texture", fileName);
            this->albedos[i].loadNotFound();
        }
        fileName.clear();

        glGenTextures(1, &this->albedos[i].id);
        glBindTexture(GL_TEXTURE_2D, this->albedos[i].id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            this->albedos[i].bitDepth == 24 ? GL_RGB : GL_RGBA,
            this->albedos[i].width,
            this->albedos[i].height,
            0,
            this->albedos[i].bitDepth == 24 ? GL_RGB : GL_RGBA,
            GL_UNSIGNED_BYTE,
            this->albedos[i].data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
}

void Level::checkNode(int nodeIndex, float deltaInitial, float deltaFinal, glm::vec3 pStart, glm::vec3 pFinal) {
    if (nodeIndex < 0) {
        QLeaf* leaf = &this->map.mLeaves[-(nodeIndex + 1)];
        for (int i = 0; i < leaf->mNbLeafBrushes; i++) {
            QBrush* brush = &this->map.mBrushes[this->map.mLeafBrushes[leaf->mLeafBrush + i].mBrushIndex];
            if (brush->mNbBrushSides > 0 && this->map.mTextures[brush->mTextureIndex].mContents & 1) {
                checkBrush(brush, pStart, pFinal);
            }
        }
        return;
    }
    const QNode* node = &this->map.mNodes[nodeIndex];
    const QPlane* plane = &this->map.mPlanes[node->mPlane];
    float startDist = (plane->mNormal[0] * pStart.x + plane->mNormal[1] * pStart.y + plane->mNormal[2] * pStart.z) - plane->mDistance;
    float endDist = (plane->mNormal[0] * pFinal.x + plane->mNormal[1] * pFinal.y + plane->mNormal[2] * pFinal.z) - plane->mDistance;
    float offset = 0;
    if (this->traceType == TRACE_TYPE_SPHERE) {
        offset = this->sphereRadius;
    }
    if (this->traceType == TRACE_TYPE_RAY) {
        offset = 0.0;
    }
    if (startDist >= offset && endDist >= offset) {
        checkNode(node->mChildren[0], deltaInitial, deltaFinal, pStart, pFinal);
        return;
    } else if (startDist < -offset && endDist < -offset) {
        checkNode(node->mChildren[1], deltaInitial, deltaFinal, pStart, pFinal);
        return;
    }
    int side = 0;
    float delta1 = 1.0f;
    float delta2 = 0.0f;
    float deltaMid;
    glm::vec3 mid;
    if (startDist < endDist) {
        side = 1;
        float inverseDistance = 1.0f / (startDist - endDist);
        delta1 = (startDist - offset - EPSILON) * inverseDistance;
        delta2 = (startDist + offset + EPSILON) * inverseDistance;
    } else if (endDist < startDist) {
        side = 0;
        float inverseDistance = 1.0f / (startDist - endDist);
        delta1 = (startDist + offset + EPSILON) * inverseDistance;
        delta2 = (startDist - offset - EPSILON) * inverseDistance;
    }
    delta1 = std::clamp(delta1, 0.0f, 1.0f);
    delta2 = std::clamp(delta2, 0.0f, 1.0f);
    deltaMid = deltaInitial + (deltaFinal - deltaInitial) * delta1;
    mid.x = pStart.x + delta1 * (pFinal.x - pStart.x);
    mid.y = pStart.y + delta1 * (pFinal.y - pStart.y);
    mid.z = pStart.z + delta1 * (pFinal.z - pStart.z);
    checkNode(node->mChildren[side], deltaInitial, deltaMid, pStart, mid);
    deltaMid = deltaInitial + (deltaFinal - deltaInitial) * delta2;
    mid.x = pStart.x + delta2 * (pFinal.x - pStart.x);
    mid.y = pStart.y + delta2 * (pFinal.y - pStart.y);
    mid.z = pStart.z + delta2 * (pFinal.z = pStart.z);
    checkNode(node->mChildren[side != 1], deltaMid, deltaFinal, mid, pFinal);
}

void Level::checkBrush(QBrush* brush, glm::vec3 vOrigin, glm::vec3 vFinal) {
    float deltaOrigin = -1.0f;
    float deltaEnd = 1.0f;
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
        if (originDist > 0 && endDist > 0) {
            return;
        }
        if (originDist <= 0 && endDist <= 0) {
            continue;
        }
        if (originDist > endDist) {
            float delta1 = (originDist - EPSILON) / (originDist - endDist);
            if (delta1 > deltaOrigin) {
                deltaOrigin = delta1;
                this->collision = true;
            }
        } else {
            float delta1 = (originDist + EPSILON) / (originDist - endDist);
            if (delta1 < deltaEnd) {
                deltaEnd = delta1;
            }
        }
    }
    if (!notAtOrigin) {
        return;
    }
    if (deltaOrigin < deltaEnd && deltaOrigin > -1 && deltaOrigin < this->outputDelta) {
        if (deltaOrigin < 0) {
            deltaOrigin = 0;
        }
        this->outputDelta = deltaOrigin;
    }
}

glm::vec3 Level::trace(glm::vec3 start, glm::vec3 end) {
    this->collision = false;
    glm::vec3 temp;
    this->outputDelta = 1.0f;
    checkNode(0, 0.0f, 1.0f, start, end);
    if (this->outputDelta == 1.0f) {
        return end;
    } else {
        this->collision = true;
        temp.x = start.x + this->outputDelta * (end.x - start.x);
        temp.y = start.y + this->outputDelta * (end.y - start.y);
        temp.z = start.z + this->outputDelta * (end.z - start.z);
    }
    return temp;
}

[[maybe_unused]] glm::vec3 Level::traceRay(glm::vec3 start, glm::vec3 end) {
    this->traceType = TRACE_TYPE_RAY;
    return trace(start, end);
}

glm::vec3 Level::traceSphere(glm::vec3 start, glm::vec3 end, float radius) {
    this->sphereRadius = radius;
    this->traceType = TRACE_TYPE_SPHERE;
    return trace(start, end);
}