#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dx11async.h>
#include <fstream>

#include "Shader.h"

using namespace DirectX;


class PPSEdgeDetection :
	public Shader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};


	struct VariableBufferType
	{
		XMFLOAT2 screenDimensions;
		bool onlyEdges;
		float padding;
	};

public:
	PPSEdgeDetection(HWND hwnd, ID3D11Device*, ID3D11DeviceContext*);
	~PPSEdgeDetection();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	void Shutdown();
	bool Render(int indexCount, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
		XMFLOAT2 screenDimensions, bool onlyEdges);

private:

	void ShutdownShader();

	bool SetShaderParameters(XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, 
		XMFLOAT2 screenDimensions, bool onlyEdges);


	ID3D11Buffer* m_variableBuffer;
};
