#include "level.hpp"

#define EPSILON 0.03125f
#define TT_RAY 0
#define TT_SPHERE 1
#define TT_BOX 2

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

	for (unsigned int i = 0; i < this->map.mVertices.size(); i++) {
		fTemp = this->map.mVertices[i].mPosition[1];
		this->map.mVertices[i].mPosition[1] = this->map.mVertices[i].mPosition[2];
		this->map.mVertices[i].mPosition[2] = -fTemp;

		fTemp = this->map.mVertices[i].mNormal[1];
		this->map.mVertices[i].mNormal[1] = this->map.mVertices[i].mNormal[2];
		this->map.mVertices[i].mNormal[2] = -fTemp;

		fTemp = this->map.mVertices[i].mTexCoord[1][0];
		this->map.mVertices[i].mTexCoord[1][0] = this->map.mVertices[i].mTexCoord[1][1];
		this->map.mVertices[i].mTexCoord[1][1] = fTemp;
	}

	for (unsigned int i = 0; i < this->map.mPlanes.size(); i++) {
		fTemp = this->map.mPlanes[i].mNormal[1];
		this->map.mPlanes[i].mNormal[1] = this->map.mPlanes[i].mNormal[2];
		this->map.mPlanes[i].mNormal[2] = -fTemp;
	}

	for (unsigned int i = 0; i < this->map.mFaces.size(); i++) {
		fTemp = this->map.mFaces[i].mNormal[1];
		this->map.mFaces[i].mNormal[1] = this->map.mFaces[i].mNormal[2];
		this->map.mFaces[i].mNormal[2] = -fTemp;
	}

	int iTemp;
	for (unsigned int i = 0; i < this->map.mLeaves.size(); i++) {
		iTemp = this->map.mLeaves[i].mMins[1];
		this->map.mLeaves[i].mMins[1] = this->map.mLeaves[i].mMins[2];
		this->map.mLeaves[i].mMins[2] = -iTemp;
		iTemp = this->map.mLeaves[i].mMaxs[1];
		this->map.mLeaves[i].mMaxs[1] = this->map.mLeaves[i].mMaxs[2];
		this->map.mLeaves[i].mMaxs[2] = -iTemp;
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
