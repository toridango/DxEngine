#pragma once

#include <vector>
#include <memory>
#include <d3d11.h>
//#include <d3dx10math.h>
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

using namespace DirectX;

#include "textureclass.h"

// TODO: Make inherit from ModelClass, FFS
class AssimpModelClass
{
public:
	/*struct Vertex
	{
		Vertex(const XMFLOAT3& pos, const XMFLOAT3& norm)
			: position(pos), padZero(0.0f), normal(norm), padOne(1.0f)
		{}

		XMFLOAT3 position;
		float padZero;
		XMFLOAT3 normal;
		float padOne;

	};
	struct Material
	{
		Material(XMFLOAT4 spec, XMFLOAT4 dif, XMFLOAT4 amb)
			: specular(spec), diffuse(dif), ambient(amb)
		{}

		XMFLOAT4 specular;
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;

	};

	class RenderCall
	{
	public:
		RenderCall(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
	
	public:
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int numIdx;

	};
	struct Mesh
	{
		RenderCall call;
		Material material;
	};*/
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	AssimpModelClass();
	AssimpModelClass(const AssimpModelClass&);
	~AssimpModelClass();

	bool Initialize(ID3D11Device* device, const char* modelFilename, WCHAR* textureFilename, bool iscubemap = false);
	bool InitializeBuffers(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();
	
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadModel(const char* modelFilename);
	void ReleaseModel();

	ID3D11ShaderResourceView* GetTexture();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename, bool iscubemap = false);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	std::vector<unsigned int> idxVector;
	ModelType* m_model;
	TextureClass* m_Texture;


};