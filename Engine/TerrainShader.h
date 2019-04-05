
#ifndef _TERRAINSHADERCLASS_H_
#define _TERRAINSHADERCLASS_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dx11async.h>
#include <fstream>

#include "Shader.h"

using namespace DirectX;


class TerrainShader :
	public Shader
{
private:

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

public:
	TerrainShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~TerrainShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	void Shutdown();
	///bool Render(ID3D11DeviceContext*, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection);
	bool Render(GameObject* go, CameraClass* camera, LightClass* light);

private:
	//bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	//void ShutdownShader();
	//void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, XMFLOAT4 ambientColour, XMFLOAT4 diffuseColour, XMFLOAT3 lightDirection);
	//void RenderShader(ID3D11DeviceContext*, int indexCount);

private:
	//ID3D11VertexShader* m_vertexShader;
	//ID3D11PixelShader* m_pixelShader;
	//ID3D11InputLayout* m_layout;
	//ID3D11SamplerState* m_sampleState;
	//ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};

#endif