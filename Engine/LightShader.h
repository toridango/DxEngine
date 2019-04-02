#pragma once
#include "Shader.h"


class LightShader :
	public Shader
{
private:

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float cpadding;
	};

	struct VariableBufferType
	{
		float delta;
		XMFLOAT3 padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColour;
		XMFLOAT4 diffuseColour;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColour;
	};


public:
	LightShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~LightShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	///bool Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	///	XMMATRIX projectionMatrix, XMFLOAT3 lightDirection, XMFLOAT4 ambientColour, XMFLOAT4 diffuseColour,
	///	XMFLOAT3 cameraPosition, XMFLOAT4 specularColour, float specularPower, float deltavalue, ID3D11ShaderResourceView* texture);
	bool Render(GameObject* go, CameraClass* camera, LightClass* light, float deltavalue);

private:

	bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, XMFLOAT3 lightDirection, XMFLOAT4 ambientColour,
		XMFLOAT4 diffuseColour, XMFLOAT3 cameraPosition, XMFLOAT4 specularColour,
		float specularPower, float deltavalue, ID3D11ShaderResourceView* texture);

protected:

	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_variableBuffer;

};

