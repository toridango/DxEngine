#pragma once


#include "Shader.h"
#include "Terrain.h"

using namespace DirectX;

class WaterShader :
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
		XMFLOAT3 surfaceDimensions;
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
	WaterShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~WaterShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	bool Render(Terrain* go, CameraClass* camera, LightClass* light, float deltavalue);

private:
	//void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, XMFLOAT4 ambientColour, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour,
		float specularPower, XMFLOAT3 lightDirection, XMFLOAT3 cameraPosition, XMFLOAT3 surfaceDimensions,
		float deltavalue);
	
private:
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_variableBuffer;
	ID3D11Buffer* m_lightBuffer;
};

