#pragma once

#include "Shader.h"


class VolumetricLaserShader :
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

	struct CapsuleBufferType
	{
		XMFLOAT3 cPos;
		float cLen;
		XMFLOAT3 cDir;
		float boundingCubeSide;
	};


public:
	VolumetricLaserShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~VolumetricLaserShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	bool Render(GameObject* go, CameraClass* camera, XMFLOAT3 cDir,
		float capsuleLen, float boundingCubeSide, float deltavalue);

private:

	bool SetShaderParameters(XMMATRIX worldMatrix,
		XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		XMFLOAT3 cameraPosition, XMFLOAT3 cPos, XMFLOAT3 cDir,
		float capsuleLen, float boundingCubeSide, float deltavalue);

protected:

	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_capsuleBuffer;
	ID3D11Buffer* m_variableBuffer;

};

