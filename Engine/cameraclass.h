////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
// #include <d3dx10math.h>
#include <DirectXMath.h>

//const float ROTATION_SPEED = 1.65f;
const float ROTATION_SPEED = 1.25f;

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	bool Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear);

	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT3 vec);
	void SetRotation(float, float, float);
	void SetRotation(XMFLOAT3 vec);

	void SetFOV(float fov);
	XMVECTOR ProjectVector(XMVECTOR v);

	//D3DXVECTOR3 GetPosition();
	//D3DXVECTOR3 GetRotation();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	XMVECTOR GetLookAtVector();
	XMVECTOR GetWrongUpVector();
	XMVECTOR GetUpVector();

	float GetFOV();

	//D3DXVECTOR3 Strafe(float);
	//D3DXVECTOR3 Advance(float);
	XMFLOAT3 Strafe(float);
	XMFLOAT3 Advance(float);
	void Rotate(XMFLOAT3);

	void Render();
	void RenderTextureView();

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthogonalMatrix();

	XMMATRIX GetTextureViewMatrix();

private:

	int m_screenW;
	int m_screenH;
	float m_screenFar;
	float m_screenNear;

	float m_fov;

	// X, Y, Z
	XMFLOAT3 m_position;

	// Pitch, Yaw, Roll
	XMFLOAT3 m_rotation;
	//float m_rotationPitch, m_rotationYaw, m_rotationRoll;
	
	XMMATRIX m_viewMatrix;
	XMMATRIX m_texViewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif