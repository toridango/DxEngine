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

const float ROTATION_SPEED = 1.65f;

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

	//D3DXVECTOR3 GetPosition();
	//D3DXVECTOR3 GetRotation();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	//D3DXVECTOR3 Strafe(float);
	//D3DXVECTOR3 Advance(float);
	XMFLOAT3 Strafe(float);
	XMFLOAT3 Advance(float);
	void Rotate(XMFLOAT3);

	void Render();
	//void GetViewMatrix(D3DXMATRIX&);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthogonalMatrix();

private:
	// X, Y, Z
	XMFLOAT3 m_position;

	// Pitch, Yaw, Roll
	XMFLOAT3 m_rotation;
	//float m_rotationPitch, m_rotationYaw, m_rotationRoll;
	
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif