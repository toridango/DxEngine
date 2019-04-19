#pragma once

#include "Shader.h"


class VolumetricBalloonShader :
	public Shader
{
private:

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float cpadding;
	};


	struct CapsuleBufferType
	{
		XMFLOAT3 cPos;
		float delta;
	};


public:
	VolumetricBalloonShader(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~VolumetricBalloonShader();

	bool InitializeShader(ShaderFiles sf);
	bool InitializeShader(WCHAR* vsFilename, WCHAR* psFilename);

	bool Render(GameObject* go, CameraClass* camera, float deltavalue);

private:

	bool SetShaderParameters(XMMATRIX worldMatrix,
		XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		XMFLOAT3 cameraPosition, XMFLOAT3 cPos, float deltavalue);

protected:

	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_capsuleBuffer;
	ID3D11Buffer* m_variableBuffer;

};
