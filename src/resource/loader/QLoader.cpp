#include "QLoader.hpp"

#pragma pack(push, QLOADER_H)
#include <cstdio>
#include <iostream>
#pragma pack(pop, QLOADER_H)

bool isValid(const QMapQ3& pMap) {
	return strncmp(pMap.mHeader.mMagicNumber, cMagicNumber.c_str(), 4) == 0
		&& pMap.mHeader.mVersion == cVersion;
}

bool readHeader(FILE* pFile, QMapQ3& pMap) {
	fread(&pMap.mHeader, 1, sizeof(QHeader), pFile);
	return isValid(pMap);
}

template <typename C>
void readElement(FILE* pFile,
				 QMapQ3& pMap,
				 const int lumpIdx,
				 std::vector<C> &elemVector) {
	const int lNbElements = pMap.mHeader.mLumps[lumpIdx].mLength / sizeof(C);
	fseek(pFile, pMap.mHeader.mLumps[lumpIdx].mOffset, SEEK_SET);
	for (int lElementCounter = 0; lElementCounter < lNbElements; ++lElementCounter) {
		C lElement;
		fread(&lElement, 1, sizeof(C), pFile);
		elemVector.push_back(lElement);
	}
}

void readTexture(FILE* pFile, QMapQ3& pMap) {
	readElement<QTexture>(pFile,pMap, cTextureLump, pMap.mTextures);
}

void readEntity(FILE* pFile, QMapQ3& pMap) {
	pMap.mEntity.mSize = pMap.mHeader.mLumps[cEntityLump].mLength;
	pMap.mEntity.mBuffer = new char[pMap.mEntity.mSize];
	fseek(pFile, pMap.mHeader.mLumps[cEntityLump].mOffset, SEEK_SET);
	fread(pMap.mEntity.mBuffer, pMap.mEntity.mSize, sizeof(char), pFile);
}

void readPlane(FILE* pFile, QMapQ3& pMap) {
	readElement<QPlane>(pFile, pMap, cPlaneLump, pMap.mPlanes);
}

void readNode(FILE* pFile, QMapQ3& pMap) {
	readElement<QNode>(pFile, pMap, cNodeLump, pMap.mNodes);
}

void readLeaf(FILE* pFile, QMapQ3& pMap) {
	readElement<QLeaf>(pFile, pMap, cLeafLump, pMap.mLeaves);
}

void readLeafFace(FILE* pFile, QMapQ3& pMap) {
	readElement<QLeafFace>(pFile, pMap, cLeafFaceLump, pMap.mLeafFaces);
}

void readLeafBrush(FILE* pFile, QMapQ3& pMap) {
	readElement<QLeafBrush>(pFile, pMap, cLeafBrushLump, pMap.mLeafBrushes);
}

void readModel(FILE* pFile, QMapQ3& pMap){
	readElement<QModel>(pFile, pMap, cModelLump, pMap.mModels);
}

void readBrush(FILE* pFile, QMapQ3& pMap) {
	readElement<QBrush>(pFile, pMap, cBrushLump, pMap.mBrushes);
}

void readBrushSide(FILE* pFile, QMapQ3& pMap) {
	readElement<QBrushSide>(pFile, pMap, cBrushSideLump, pMap.mBrushSides);
}

void readVertex(FILE* pFile, QMapQ3& pMap) {
	readElement<QVertex>(pFile, pMap, cVertexLump, pMap.mVertices);
}

void readMeshVert(FILE* pFile, QMapQ3& pMap) {
	readElement<QMeshVert>(pFile, pMap, cMeshVertLump, pMap.mMeshVertices);
}

void readEffect(FILE* pFile, QMapQ3& pMap) {
	readElement<QEffect>(pFile, pMap, cEffectLump, pMap.mEffects);
}

void readFace(FILE* pFile, QMapQ3& pMap) {
	readElement<QFace>(pFile, pMap, cFaceLump, pMap.mFaces);
}

void readLightMap(FILE* pFile, QMapQ3& pMap) {
	readElement<QLightMap>(pFile, pMap, cLightMapLump, pMap.mLightMaps);
}

void readLightVol(FILE* pFile, QMapQ3& pMap) {
	readElement<QLightVol>(pFile, pMap, cLightVolLump, pMap.mLightVols);
}

void readVisData(FILE* pFile, QMapQ3& pMap) {
	fseek(pFile, pMap.mHeader.mLumps[cVisDataLump].mOffset, SEEK_SET);

	fread(&pMap.mVisData.mNbClusters, 1, sizeof(int), pFile);
	fread(&pMap.mVisData.mBytesPerCluster, 1, sizeof(int), pFile);

	int lBufferSize = pMap.mVisData.mNbClusters * pMap.mVisData.mBytesPerCluster;
	pMap.mVisData.mBuffer = new unsigned char[lBufferSize];

	fread(pMap.mVisData.mBuffer, lBufferSize, sizeof(unsigned char), pFile);
}

bool readMap(const std::string& pFilename, QMapQ3& pMap) {
	// Yeah... this is a mad vulnerability here lol. Will fix, as well as the other stuff.
	FILE* lFile = fopen(pFilename.c_str(), "r+b");

	if (!lFile) {
		std::cout << "readMap :: Invalid stream handle" << std::endl;
		return false;
	}

	if (readHeader(lFile, pMap) == false) {
		std::cout << "readMap :: Invalid Q3 map header" << std::endl;
		return false;
	}

	readEntity(lFile, pMap);
	readTexture(lFile, pMap);
	readPlane(lFile, pMap);
	readNode(lFile, pMap);
	readLeaf(lFile, pMap);
	readLeafFace(lFile, pMap);
	readLeafBrush(lFile, pMap);
	readModel(lFile, pMap);
	readBrush(lFile, pMap);
	readBrushSide(lFile, pMap);
	readVertex(lFile, pMap);
	readMeshVert(lFile, pMap);
	readEffect(lFile, pMap);
	readFace(lFile, pMap);
	readLightMap(lFile, pMap);
	readLightVol(lFile, pMap);
	readVisData(lFile, pMap);
	fclose(lFile);
	return true;
};

void freeMap(QMapQ3& pMap) {
	if (pMap.mEntity.mBuffer) {
		delete[] pMap.mEntity.mBuffer;
		pMap.mEntity.mBuffer = NULL;
	}

	if (pMap.mVisData.mBuffer) {
		delete[] pMap.mVisData.mBuffer;
		pMap.mVisData.mBuffer = NULL;
	}

	pMap.mTextures.clear();
	pMap.mPlanes.clear();
	pMap.mNodes.clear();
	pMap.mLeaves.clear();
	pMap.mLeafFaces.clear();
	pMap.mLeafBrushes.clear();
	pMap.mModels.clear();
	pMap.mBrushes.clear();
	pMap.mBrushSides.clear();
	pMap.mVertices.clear();
	pMap.mMeshVertices.clear();
	pMap.mEffects.clear();
	pMap.mFaces.clear();
	pMap.mLightMaps.clear();
	pMap.mLightVols.clear();
}

void debugInfo(FILE* pFile, const QMapQ3& pMap) {
	// Originally from alexkid77/3dEngineOpenGL
	// Super dodgy debug with super bad fprintf. I'll create proper debug stuff later that isn't garbage
	if (!pFile) {
		std::cout << "debugInfo :: Invalid stream handle" << std::endl;
		return;
	}

	if (!isValid(pMap)) {
		std::cout << "debugInfo :: Invalid Q3 map header" << std::endl;
		return;
	}

	fprintf(pFile, "********* Header *********\n");
	fprintf(pFile, "Magic Number : %s\n", pMap.mHeader.mMagicNumber);
	fprintf(pFile, "Version : %d\n", pMap.mHeader.mVersion);
	for (int lLumpCounter = 0; lLumpCounter < 17; ++lLumpCounter) {
		fprintf(pFile, "Lump %d\n", lLumpCounter);
		fprintf(pFile, "\tOffset : %d\n", pMap.mHeader.mLumps[lLumpCounter].mOffset);
		fprintf(pFile, "\tLength : %d\n", pMap.mHeader.mLumps[lLumpCounter].mLength);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Entity Lump *********\n");
	fprintf(pFile, "Size : %d\n", pMap.mEntity.mSize);
	if (pMap.mEntity.mSize != 0) {
		fprintf(pFile, "Buffer : %s\n", pMap.mEntity.mBuffer);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Texture Lump *********\n");
	for (int lTextureCounter = 0; lTextureCounter < (int)pMap.mTextures.size(); ++lTextureCounter) {
		fprintf(pFile, "Texture %d\n", lTextureCounter);
		fprintf(pFile, "\tName : %s\n", pMap.mTextures[lTextureCounter].mName);
		fprintf(pFile, "\tFlags : %d\n", pMap.mTextures[lTextureCounter].mFlags);
		fprintf(pFile, "\tContents : %d\n", pMap.mTextures[lTextureCounter].mContents);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Plane Lump *********\n");
	for (int lPlaneCounter = 0; lPlaneCounter < (int)pMap.mPlanes.size(); ++lPlaneCounter) {
		fprintf(pFile, "Plane %d\n", lPlaneCounter);
		fprintf(pFile, "\tNormal : %f %f %f\n", pMap.mPlanes[lPlaneCounter].mNormal[0], pMap.mPlanes[lPlaneCounter].mNormal[1], pMap.mPlanes[lPlaneCounter].mNormal[2]);
		fprintf(pFile, "\tDistance : %f\n", pMap.mPlanes[lPlaneCounter].mDistance);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Node Lump *********\n");
	for (int lNodeCounter = 0; lNodeCounter < (int)pMap.mNodes.size(); ++lNodeCounter) {
		fprintf(pFile, "Node %d\n", lNodeCounter);
		fprintf(pFile, "\tPlane index : %d\n", pMap.mNodes[lNodeCounter].mPlane);
		fprintf(pFile, "\tChildren index : %d %d\n", pMap.mNodes[lNodeCounter].mChildren[0], pMap.mNodes[lNodeCounter].mChildren[1]);
		fprintf(pFile, "\tMin Bounding Box : %d %d %d\n", pMap.mNodes[lNodeCounter].mMins[0], pMap.mNodes[lNodeCounter].mMins[1], pMap.mNodes[lNodeCounter].mMins[2]);
		fprintf(pFile, "\tMax Bounding Box : %d %d %d\n", pMap.mNodes[lNodeCounter].mMaxs[0], pMap.mNodes[lNodeCounter].mMaxs[1], pMap.mNodes[lNodeCounter].mMaxs[2]);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Leaf Lump *********\n");
	for (int lLeafCounter = 0; lLeafCounter < (int)pMap.mLeaves.size(); ++lLeafCounter) {
		fprintf(pFile, "Leaf %d\n", lLeafCounter);
		fprintf(pFile, "\tCluster %d\n", pMap.mLeaves[lLeafCounter].mCluster);
		fprintf(pFile, "\tMin Bounding Box : %d %d %d\n", pMap.mLeaves[lLeafCounter].mMins[0], pMap.mLeaves[lLeafCounter].mMins[1], pMap.mLeaves[lLeafCounter].mMins[2]);
		fprintf(pFile, "\tMax Bounding Box : %d %d %d\n", pMap.mLeaves[lLeafCounter].mMaxs[0], pMap.mLeaves[lLeafCounter].mMaxs[1], pMap.mLeaves[lLeafCounter].mMaxs[2]);
		fprintf(pFile, "\tLeafFace %d\n", pMap.mLeaves[lLeafCounter].mLeafFace);
		fprintf(pFile, "\tNb LeafFace %d\n", pMap.mLeaves[lLeafCounter].mNbLeafFaces);
		fprintf(pFile, "\tLeafBrush %d\n", pMap.mLeaves[lLeafCounter].mLeafBrush);
		fprintf(pFile, "\tNb LeafBrushes %d\n", pMap.mLeaves[lLeafCounter].mNbLeafBrushes);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* LeafFace Lump *********\n");
	for (int lLeafFaceCounter = 0; lLeafFaceCounter < (int)pMap.mLeafFaces.size(); ++lLeafFaceCounter) {
		fprintf(pFile, "LeafFace %d\n", lLeafFaceCounter);
		fprintf(pFile, "\tFaceIndex %d\n", pMap.mLeafFaces[lLeafFaceCounter].mFaceIndex);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* LeafBrush Lump *********\n");
	for (int lLeafBrushCounter = 0; lLeafBrushCounter < (int)pMap.mLeafBrushes.size(); ++lLeafBrushCounter) {
		fprintf(pFile, "LeafBrush %d\n", lLeafBrushCounter);
		fprintf(pFile, "\tBrushIndex %d\n", pMap.mLeafBrushes[lLeafBrushCounter].mBrushIndex);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Model Lump *********\n");
	for (int lModelCounter = 0; lModelCounter < (int)pMap.mModels.size(); ++lModelCounter) {
		fprintf(pFile, "Model %d\n", lModelCounter);
		fprintf(pFile, "\tMin Bounding Box : %p %p %p\n", pMap.mModels[lModelCounter].mMins[0], pMap.mModels[lModelCounter].mMins[1], pMap.mModels[lModelCounter].mMins[2]);
		fprintf(pFile, "\tMax Bounding Box : %p %p %p\n", pMap.mModels[lModelCounter].mMaxs[0], pMap.mModels[lModelCounter].mMaxs[1], pMap.mModels[lModelCounter].mMaxs[2]);
		fprintf(pFile, "\tFace %f\n", pMap.mModels[lModelCounter].mFace);
		fprintf(pFile, "\tNbFaces %f\n", pMap.mModels[lModelCounter].mNbFaces);
		fprintf(pFile, "\tBrush %f\n", pMap.mModels[lModelCounter].mBrush);
		fprintf(pFile, "\tNbBrushes %f\n", pMap.mModels[lModelCounter].mNBrushes);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Brush Lump *********\n");
	for (int lBrushCounter = 0; lBrushCounter < (int)pMap.mBrushes.size(); ++lBrushCounter) {
		//	fprintf(pFile, "Brush %d\n", lModelCounter);
		fprintf(pFile, "\tBrushSide %d\n", pMap.mBrushes[lBrushCounter].mBrushSide);
		fprintf(pFile, "\tNbBrushSides %d\n", pMap.mBrushes[lBrushCounter].mNbBrushSides);
		fprintf(pFile, "\tTextureIndex %d\n", pMap.mBrushes[lBrushCounter].mTextureIndex);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* BrushSide Lump *********\n");
	for (int lBrushSideCounter = 0; lBrushSideCounter < (int)pMap.mBrushSides.size(); ++lBrushSideCounter) {
		fprintf(pFile, "BrushSide %d\n", lBrushSideCounter);
		fprintf(pFile, "\tPlaneIndex %d\n", pMap.mBrushSides[lBrushSideCounter].mPlaneIndex);
		fprintf(pFile, "\tTextureIndex %d\n", pMap.mBrushSides[lBrushSideCounter].mTextureIndex);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Vertex Lump *********\n");
	for (int lVertexCounter = 0; lVertexCounter < (int)pMap.mVertices.size(); ++lVertexCounter) {
		fprintf(pFile, "Vertex %d\n", lVertexCounter);
		fprintf(pFile, "\tPosition : %f %f %f\n", pMap.mVertices[lVertexCounter].mPosition[0], pMap.mVertices[lVertexCounter].mPosition[1], pMap.mVertices[lVertexCounter].mPosition[2]);
		fprintf(pFile, "\tTexCoord0 : %f %f\n", pMap.mVertices[lVertexCounter].mTexCoord[0], pMap.mVertices[lVertexCounter].mTexCoord[1]);
		fprintf(pFile, "\tTexCoord0 : %f %f\n", pMap.mVertices[lVertexCounter].mTexCoord[2], pMap.mVertices[lVertexCounter].mTexCoord[3]);
		fprintf(pFile, "\tNormal : %f %f %f\n", pMap.mVertices[lVertexCounter].mNormal[0], pMap.mVertices[lVertexCounter].mNormal[1], pMap.mVertices[lVertexCounter].mNormal[2]);
		fprintf(pFile, "\tColor : %d %d %d %d\n", pMap.mVertices[lVertexCounter].mColor[0], pMap.mVertices[lVertexCounter].mColor[1], pMap.mVertices[lVertexCounter].mColor[2], pMap.mVertices[lVertexCounter].mColor[3]);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* MeshVert Lump *********\n");
	for (int lMeshVertCounter = 0; lMeshVertCounter < (int)pMap.mMeshVertices.size(); ++lMeshVertCounter) {
		fprintf(pFile, "MeshVert %d\n", lMeshVertCounter);
		fprintf(pFile, "\tVertex Index : %d\n", pMap.mMeshVertices[lMeshVertCounter].mMeshVert);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Effect Lump *********\n");
	for (int lEffectCounter = 0; lEffectCounter < (int)pMap.mEffects.size(); ++lEffectCounter) {
		fprintf(pFile, "Effect %d\n", lEffectCounter);
		fprintf(pFile, "\tName : %s\n", pMap.mEffects[lEffectCounter].mName);
		fprintf(pFile, "\tBrush : %d\n", pMap.mEffects[lEffectCounter].mBrush);
		fprintf(pFile, "\tUnknown : %d\n", pMap.mEffects[lEffectCounter].mUnknown);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* Face Lump *********\n");
	for (int lFaceCounter = 0; lFaceCounter < (int)pMap.mFaces.size(); ++lFaceCounter) {
		fprintf(pFile, "Face %d\n", lFaceCounter);
		fprintf(pFile, "\tTextureIndex : %d\n", pMap.mFaces[lFaceCounter].mTextureIndex);
		fprintf(pFile, "\tEffectIndex : %d\n", pMap.mFaces[lFaceCounter].mEffectIndex);
		fprintf(pFile, "\tType : %d\n", pMap.mFaces[lFaceCounter].mType);
		fprintf(pFile, "\tVertex : %d\n", pMap.mFaces[lFaceCounter].mVertex);
		fprintf(pFile, "\tNbVertices : %d\n", pMap.mFaces[lFaceCounter].mNbVertices);
		fprintf(pFile, "\tMeshVertex : %d\n", pMap.mFaces[lFaceCounter].mMeshVertex);
		fprintf(pFile, "\tNbMeshVertices : %d\n", pMap.mFaces[lFaceCounter].mNbMeshVertices);
		fprintf(pFile, "\tLightMapIndex : %d\n", pMap.mFaces[lFaceCounter].mLightmapIndex);
		fprintf(pFile, "\tLightMapCorner : %d %d\n", pMap.mFaces[lFaceCounter].mLightmapCorner[0], pMap.mFaces[lFaceCounter].mLightmapCorner[1]);
		fprintf(pFile, "\tLightmapSize : %d %d\n", pMap.mFaces[lFaceCounter].mLightmapSize[0], pMap.mFaces[lFaceCounter].mLightmapSize[1]);
		fprintf(pFile, "\tLightmapOrigin : %f %f %f\n", pMap.mFaces[lFaceCounter].mLightmapOrigin[0], pMap.mFaces[lFaceCounter].mLightmapOrigin[1], pMap.mFaces[lFaceCounter].mLightmapOrigin[2]);
		fprintf(pFile, "\tLightmapVecs S : %f %f %f\n", pMap.mFaces[lFaceCounter].mLightmapVecs[0][0], pMap.mFaces[lFaceCounter].mLightmapVecs[0][1], pMap.mFaces[lFaceCounter].mLightmapVecs[0][2]);
		fprintf(pFile, "\tLightmapVecs T : %f %f %f\n", pMap.mFaces[lFaceCounter].mLightmapVecs[1][0], pMap.mFaces[lFaceCounter].mLightmapVecs[1][1], pMap.mFaces[lFaceCounter].mLightmapVecs[1][2]);
		fprintf(pFile, "\tNormal : %f %f %f\n", pMap.mFaces[lFaceCounter].mNormal[0], pMap.mFaces[lFaceCounter].mNormal[1], pMap.mFaces[lFaceCounter].mNormal[2]);
		fprintf(pFile, "\tPatchSize : %d %d\n", pMap.mFaces[lFaceCounter].mPatchSize[0], pMap.mFaces[lFaceCounter].mPatchSize[1]);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* LightMap Lump *********\n");
	fprintf(pFile, "NbLightMaps %d\n", pMap.mLightMaps.size());
	fprintf(pFile, "\n");

	fprintf(pFile, "********* LightVol Lump *********\n");
	for (int lLightVolCounter = 0; lLightVolCounter < (int)pMap.mLightVols.size(); ++lLightVolCounter) {
		fprintf(pFile, "LightVol %d\n", lLightVolCounter);
		fprintf(pFile, "\tAmbient : %d %d %d\n", pMap.mLightVols[lLightVolCounter].mAmbient[0], pMap.mLightVols[lLightVolCounter].mAmbient[1], pMap.mLightVols[lLightVolCounter].mAmbient[2]);
		fprintf(pFile, "\tDirectional : %d %d %d\n", pMap.mLightVols[lLightVolCounter].mDirectional[0], pMap.mLightVols[lLightVolCounter].mDirectional[1], pMap.mLightVols[lLightVolCounter].mDirectional[2]);
		fprintf(pFile, "\tDir : %d %d\n", pMap.mLightVols[lLightVolCounter].mDir[0], pMap.mLightVols[lLightVolCounter].mDir[1]);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "********* VisData Lump *********\n");
	fprintf(pFile, "\tNbCluster %d\n", pMap.mVisData.mNbClusters);
	fprintf(pFile, "\tBytePerCluster %d\n", pMap.mVisData.mBytesPerCluster);
	fprintf(pFile, "\n");
}