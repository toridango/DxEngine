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


	struct ScreenSizeBufferType
	{
		float screenWidth;
		XMFLOAT3 padding;
	};

public:
	PostProcessShader(HWND hwnd, ID3D11Device*, ID3D11DeviceContext*);
	~PostProcessShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	void Shutdown();
	bool Render(int indexCount, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float screenWidth);

private:

	void ShutdownShader();

	bool SetShaderParameters(XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float);


private:
	//ID3D11VertexShader* m_vertexShader;
	//ID3D11PixelShader* m_pixelShader;
	//ID3D11InputLayout* m_layout;
	//ID3D11SamplerState* m_sampleState;
	//ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_screenSizeBuffer;
};
