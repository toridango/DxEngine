#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dx11async.h>
#include <fstream>

#include "Shader.h"

using namespace DirectX;


class PostProcessShader :
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
		bool sprinting;
		XMFLOAT3 padding;
	};

public:
	PostProcessShader(HWND hwnd, ID3D11Device*, ID3D11DeviceContext*);
	~PostProcessShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	void Shutdown();
	bool Render(int indexCount, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, bool sprinting);

private:

	void ShutdownShader();

	bool SetShaderParameters(XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, bool sprinting);


//private:
	ID3D11Buffer* m_variableBuffer;
};
