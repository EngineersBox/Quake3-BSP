#pragma once

#ifndef QLOADER_H
#define QLOADER_H

#pragma pack(push, QLOADER_H)
#include <vector>
#include <string>
#pragma pack(pop, QLOADER_H)

struct QLump {
	int	mOffset; // Offset to start of lump, relative to beginning of file.  
	int	mLength; // Length of lump. Always a multiple of 4.  
};

struct QHeader {
	char mMagicNumber[4]; // Magic number. Always "IBSP".  
	int	mVersion; // Version number 0x2e for the BSP files distributed with Quake 3.  
	QLump mLumps[17]; // Lump directory, seventeen entries.
};

struct QEntity {
	int	mSize; // Size of the description.
	char* mBuffer; // Entity descriptions, stored as a string. 
};

struct QTexture {
	char mName[64];
	int	mFlags;	// Surface flags.  
	int	mContents; // Content flags.  
};

struct QPlane {
	float mNormal[3];
	float mDistance; // Distance from origin to plane along normal.  
};

struct QNode {
	int	mPlane;	// Plane index.  
	int	mChildren[2]; // Children indices. Negative numbers are leaf indices: -(leaf+1).  
	int	mMins[3]; // Bounding box min coord.  
	int	mMaxs[3]; // Bounding box max coord.  
};

struct QLeaf {
	int	mCluster; // Visdata cluster index.  
	int	mArea;	// Areaportal area.  
	int	mMins[3]; // Bounding box min coord.  
	int	mMaxs[3]; // Bounding box max coord.  
	int	mLeafFace;	// First leafface for leaf.  
	int	mNbLeafFaces; // Number of leaffaces for leaf.  
	int	mLeafBrush;	// First leafbrush for leaf.  
	int	mNbLeafBrushes;	// Number of leafbrushes for leaf.  

};

struct QLeafFace{
	int	mFaceIndex;
};

struct QLeafBrush {
	int	mBrushIndex;
};

struct QModel {
	float mMins[3];	// Bounding box min coord.  
	float mMaxs[3];	// Bounding box max coord.  
	int	mFace; // First face for model.  
	int	mNbFaces; // Number of faces for model.  
	int	mBrush;	// First brush for model.  
	int	mNBrushes; // Number of brushes for model.  

};

struct QBrush{
	int	mBrushSide;
	int	mNbBrushSides;
	int	mTextureIndex;
};

struct QBrushSide {
	int	mPlaneIndex;
	int	mTextureIndex;
};

struct QVertex {
	float mPosition[3];
	float mTexCoord[2][2]; // Vertex texture coordinates. 0 = Surface, 1 = Lightmap.  
	float mNormal[3];
	unsigned char mColor[4];

	QVertex operator+(const QVertex& a) const {
		QVertex resultado;
		resultado.mPosition[0] += a.mPosition[0];
		resultado.mPosition[1] += a.mPosition[1];
		resultado.mPosition[2] += a.mPosition[2];

		resultado.mTexCoord[0][0] += a.mTexCoord[0][0];
		resultado.mTexCoord[0][1] += a.mTexCoord[0][1];
		resultado.mTexCoord[1][0] += a.mTexCoord[1][0];
		resultado.mTexCoord[1][1] += a.mTexCoord[1][1];

		return resultado;
	}

	QVertex operator*(const float a) const {
		QVertex resultado;

		resultado.mPosition[0] *= a;
		resultado.mPosition[1] *= a;
		resultado.mPosition[2] *= a;

		resultado.mTexCoord[0][0] *= a;
		resultado.mTexCoord[0][1] *= a;

		resultado.mTexCoord[1][0] *= a;
		resultado.mTexCoord[1][1] *= a;

		resultado.mNormal[0] = mNormal[0];
		resultado.mNormal[1] = mNormal[2];
		resultado.mNormal[2] = mNormal[2];

		resultado.mColor[0] = mColor[0];
		resultado.mColor[1] = mColor[1];
		resultado.mColor[2] = mColor[2];
		resultado.mColor[3] = mColor[3];

		return resultado;
	}
};

struct QMeshVert {
	int	mMeshVert;// Vertex index offset, relative to first vertex of corresponding face.
};

struct QEffect{
	char mName[64];	// Effect shader.  
	int	mBrush;	// Brush that generated this effect.  
	int	mUnknown; // Always 5, except in q3dm8, which has one effect with -1.  
};

struct QFace {
	int	mTextureIndex; 
	int	mEffectIndex; // Index into lump 12 (Effects), or -1.  
	int	mType; // Face type. 1 = Polygon, 2 = Patch, 3 = Mesh, 4 = Billboard. 
	int	mVertex; // Index of first vertex.  
	int	mNbVertices; // Number of vertices.  
	int	mMeshVertex; // Index of first meshvert.  
	int	mNbMeshVertices; // Number of meshverts.  
	int	mLightmapIndex;
	int	mLightmapCorner[2];	// Corner of this face's lightmap image in lightmap.  
	int	mLightmapSize[2]; // Size of this face's lightmap image in lightmap.  
	float mLightmapOrigin[3]; // World space origin of lightmap.  
	float mLightmapVecs[2][3]; // World space lightmap s and t unit vectors.  
	float mNormal[3]; // Surface normal.  
	int	mPatchSize[2];
};

struct QLightMap {
	unsigned char mMapData[128][128][3]; // Lightmap color data. RGB. 
};


struct QLightVol {
	unsigned char mAmbient[3]; // Ambient color component. RGB.  
	unsigned char mDirectional[3]; // Directional color component. RGB.  
	unsigned char mDir[2]; // Direction to light. 0=phi, 1=theta.  
};

struct QVisData {
	int	mNbClusters; // The number of clusters
	int	mBytesPerCluster; // Bytes (8 bits) in the cluster's bitset
	unsigned char* mBuffer;	// Array of bytes holding the cluster vis.
};

struct QMapQ3 {
	QHeader	mHeader;
	QEntity	mEntity; // Leaves
	std::vector<QTexture> mTextures;
	std::vector<QPlane>	mPlanes;
	std::vector<QNode> mNodes;
	std::vector<QLeaf> mLeaves;
	std::vector<QLeafFace> mLeafFaces;
	std::vector<QLeafBrush>	mLeafBrushes;
	std::vector<QModel>	mModels;
	std::vector<QBrush>	mBrushes;
	std::vector<QBrushSide> mBrushSides;
	std::vector<QVertex> mVertices;
	std::vector<QMeshVert> mMeshVertices;
	std::vector<QEffect> mEffects;
	std::vector<QFace> mFaces;
	std::vector<QLightMap> mLightMaps;
	std::vector<QLightVol> mLightVols;
	QVisData mVisData;
};

/**
 * Id Software Magic number.
 */
const std::string cMagicNumber = "IBSP";
const int cVersion = 0x2E; // Quake 3 map version

const int cEntityLump = 0x00; // Entities : Game-related object descriptions.  
const int cTextureLump = 0x01; // Textures : Surface descriptions.  
const int cPlaneLump = 0x02; // Planes : Planes used by map geometry.  
const int cNodeLump = 0x03; // Nodes : BSP tree nodes.  
const int cLeafLump = 0x04; // Leafs : BSP tree leaves.  
const int cLeafFaceLump = 0x05; // LeafFaces : Lists of face indices, one list per leaf.  
const int cLeafBrushLump = 0x06; // LeafBrushes  Lists of brush indices, one list per leaf.  
const int cModelLump = 0x07; // Models  Descriptions of rigid world geometry in map.  
const int cBrushLump = 0x08; // Brushes  Convex polyhedra used to describe solid space.  
const int cBrushSideLump = 0x09; // Brushsides  Brush surfaces.  
const int cVertexLump = 0x0A; // Vertexes  Vertices used to describe faces.  
const int cMeshVertLump = 0x0B; // MeshVerts  Lists of offsets, one list per mesh.  
const int cEffectLump = 0x0C; // Effects  List of special map effects.  
const int cFaceLump = 0x0D; // Faces  Surface geometry.  
const int cLightMapLump = 0x0E; // LightMaps  Packed lightmap data.  
const int cLightVolLump = 0x0F; // LightVols  Local illumination data.  
const int cVisDataLump = 0x10; // Visdata  Cluster-cluster visibility data.

void debugInfo(FILE* pFile, const QMapQ3& pMap);
bool readMap(const std::string& pFilename, QMapQ3& pMap);
void freeMap(QMapQ3& pMap);
bool isValid(const QMapQ3& pMap);

#endif // QLOADER_H