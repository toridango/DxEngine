#pragma once


#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_



#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <cmath>

#include "ImprovedNoise.h"
#include "GameObject.h"

// Use https://github.com/nothings/stb instead

using namespace DirectX;

const int TERRAINWIDTH = 128;
const int TERRAINHEIGHT = 128;

class Terrain : 
	public GameObject
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();


	bool Initialize(ID3D11Device*, int terrainWidth, int terrainHeight);
	void ModelShutdown();

	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(double scaling = 1.0, double zoom = 1.0);
	int  GetIndexCount();

	bool Smooth();

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	//bool LoadColourMap(char* filename);
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	int RandBetween(int min, int max);
	float Rand01();

private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Device* m_device;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	HeightMapType* m_pastMap;
	ImprovedNoise m_perlinImpNoise;
};

#endif