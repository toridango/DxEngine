#pragma once


#include <d3d11.h>
#include <d3dx11async.h>
#include <fstream>
#include <DirectXMath.h>

#include "GameObject.h"
#include "lightclass.h"
#include "cameraclass.h"
#include "Utilities.h"
using namespace DirectX;

struct ShaderFiles
{
	WCHAR* vs;
	WCHAR* ps;
};

class Shader
{
protected:

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};


public:
	Shader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
	~Shader();

	bool CompileVertexShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, ID3D10Blob **ppShader);
	bool CompilePixelShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, ID3D10Blob **ppShader);
	
	// In children: Add custom elements to initialize
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename, 
		const D3D11_INPUT_ELEMENT_DESC* polygonLayout, unsigned int numElementsPolyLayout);
		
	void RenderShader(int indexCount);



protected:
	
	bool CompileShaderFromFile(WCHAR* filename, LPCSTR pFunctionName, LPCSTR pProfile, ID3D10Blob **ppShader);
	
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	// In children: Add custom elements to setParameters
	bool SetMatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix/*, XMFLOAT3 lightDirection*/);

protected:
	HWND m_hwnd;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	LPCSTR m_psFunctionName;
	LPCSTR m_vsFunctionName;
};

