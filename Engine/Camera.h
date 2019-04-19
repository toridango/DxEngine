#pragma once
#include "GameObject.h"
class Camera :
	public GameObject
{
public:
	Camera();
	~Camera();


	bool Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear);


	XMFLOAT3 GetPosition();
	XMMATRIX GetRotation();

	XMVECTOR GetLookAtVector();
	XMVECTOR GetUpVector();

	XMFLOAT3 Strafe(float);
	XMFLOAT3 Advance(float);
	void Rotate(float pitch, float yaw, float roll);

	void Render();
	void RenderTextureView();

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthogonalMatrix();

	XMMATRIX GetTextureViewMatrix();


private:
	XMFLOAT3 m_position;

	XMMATRIX m_viewMatrix;
	XMMATRIX m_texViewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};

