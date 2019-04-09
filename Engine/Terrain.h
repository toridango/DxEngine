#pragma once


#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_



#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <limits>

#include "ImprovedNoise.h"
#include "GameObject.h"

// Use https://github.com/nothings/stb instead

using namespace DirectX;

const int TERRAINWIDTH = 256;
const int TERRAINHEIGHT = 256;
const int TEXTURE_REPEAT = 32;

class Terrain :
	public GameObject
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		XMFLOAT4 blending;
	};

	struct HeightMapType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		XMFLOAT4 blending;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	enum TEXID
	{
		LOWEST,
		LOW,
		HIGH,
		HIGHEST
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();


	bool Initialize(ID3D11Device*, int terrainWidth, int terrainHeight, WCHAR* lowestTexFile, WCHAR* lowTexFile, WCHAR* highTexFile, WCHAR* highestTexFile);
	void ModelShutdown();

	ID3D11ShaderResourceView* GetTexture(Terrain::TEXID id);
	bool LoadTexture(ID3D11Device* device, WCHAR* filename, Terrain::TEXID id);

	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(double scaling = 1.0, double zoom = 1.0);
	int  GetIndexCount();

	bool Smooth();
	XMFLOAT4 TextureBlendingByHeight(HeightMapType h);
	XMFLOAT4 TextureBlendingByHeight(float h);
	void CalculateBlendings();

private:
	void NormalizeHeightMap();
	bool CalculateNormals();
	void CalculateTextureCoordinates();
	void ShutdownHeightMap();
	void ReleaseTextures();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	int RandBetween(int min, int max);
	float Rand01();

private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	double m_maxHeight;
	double m_minHeight;

	ID3D11Device* m_device;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	HeightMapType* m_pastMap;
	ImprovedNoise m_perlinImpNoise;

	std::vector<TextureClass*> m_Textures;
};

#endif