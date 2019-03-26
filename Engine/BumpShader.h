#pragma once
#include "Shader.h"
class BumpShader :
	public Shader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		// D3DXVECTOR4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};


public:
	BumpShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~BumpShader();


	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	//bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	//	XMMATRIX projectionMatrix, XMFLOAT3 lightDirection, XMFLOAT4 ambientColour, XMFLOAT4 diffuseColour,
	//	XMFLOAT3 cameraPosition, XMFLOAT4 specularColour, float specularPower, float deltavalue, ID3D11ShaderResourceView* texture);
	
	bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection);

private:

	bool SetShaderParameters(XMMATRIX worldMatrix,
		XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView** textureArray, XMFLOAT3 lightDirection);

protected:

	// The bump map shader will require a constant 
	// buffer to interface with the light direction 
	// and light color.
	ID3D11Buffer* m_lightBuffer;

};

