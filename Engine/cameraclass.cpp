////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}

bool CameraClass::Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{

	m_screenW = screenWidth;
	m_screenH = screenHeight;
	m_screenFar = screenDepth;
	m_screenNear = screenNear;

	float fieldOfView = (float)XM_PIDIV4;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);


	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);



	return true;
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
void CameraClass::SetPosition(XMFLOAT3 vec)
{
	m_position.x = vec.x;
	m_position.y = vec.y;
	m_position.z = vec.z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}
void CameraClass::SetRotation(XMFLOAT3 vec)
{
	m_rotation.x = vec.x;
	m_rotation.y = vec.y;
	m_rotation.z = vec.z;
}



void CameraClass::SetFOV(float fovLerp)
{
	XMVECTOR fov0 = {XM_PIDIV4, XM_PIDIV4, XM_PIDIV4};
	XMVECTOR fov1 = {XM_PIDIV2, XM_PIDIV2, XM_PIDIV2};

	XMVECTOR lerp = XMVectorLerp(fov0, fov1, fovLerp);

	float fieldOfView = (float)XMVectorGetX(lerp);
	float screenAspect = (float)m_screenW / (float)m_screenH;
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screenNear, m_screenFar);
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

XMVECTOR CameraClass::GetLookAtVector()
{
	// Copy view matrix
	XMMATRIX cameraMatrix(m_viewMatrix);
	// Invert it
	cameraMatrix = XMMatrixInverse(&XMMatrixDeterminant(cameraMatrix), cameraMatrix);
	// Forward vector (use 0, 0, -1 if you have a right-handed coordinate system)
	XMVECTOR lookAtInViewSpace = { 0.0, 0.0, 1.0 };
	// Transform into world space. Also: transform into normal 
	//XMVECTOR lookAtInWorldSpace = lookAtInViewSpace * viewMatrix; 
	return XMVector3TransformNormal(lookAtInViewSpace, cameraMatrix);
}

XMVECTOR CameraClass::GetWrongUpVector()
{
	return m_viewMatrix.r[1];
}


XMVECTOR CameraClass::GetUpVector()
{
	XMMATRIX cameraMatrix(m_viewMatrix);
	// Invert it
	cameraMatrix = XMMatrixInverse(&XMMatrixDeterminant(cameraMatrix), cameraMatrix);
	return cameraMatrix.r[1];
}



XMFLOAT3 CameraClass::Strafe(float sign) // + for right, - for left
{
	/*XMFLOAT3 up, lookAt, right*/;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR right;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	/*up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;*/

	// Setup where the camera is looking by default.
	/*lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;*/

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	// PI/180 = 0.0174532925f;
	pitch = m_rotation.x * (XM_PI / 180.0);
	yaw = m_rotation.y * (XM_PI / 180.0);
	roll = m_rotation.z * (XM_PI / 180.0);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// LEFT HANDED CROSS PRODUCT
	// https://softwareengineering.stackexchange.com/questions/17519/why-does-directx-use-a-left-handed-coordinate-system
	//D3DXVec3Cross(&right, &up, &lookAt);
	right = XMVector3Cross(up, lookAt);

	//D3DXVECTOR3 mov = D3DXVECTOR3(right.x, right.y, right.z);
	//D3DXVec3Normalize(&mov, &mov);

	XMVECTOR mov = XMVector3Normalize(right);
	XMFLOAT3 vmov;
	XMStoreFloat3(&vmov, mov);
	m_position.x += sign * vmov.x;
	m_position.y += sign * vmov.y;
	m_position.z += sign * vmov.z;

	return m_position;

}

XMFLOAT3 CameraClass::Advance(float sign) // + for forward, - for backward
{
	//D3DXVECTOR3 lookAt, move;
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup where the camera is looking by default.
	/*lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;*/

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	// PI/180 = 0.0174532925f;
	pitch = m_rotation.x * (XM_PI / 180.0);
	yaw = m_rotation.y * (XM_PI / 180.0);
	roll = m_rotation.z * (XM_PI / 180.0);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);

	//D3DXVECTOR3 mov = D3DXVECTOR3(lookAt.x, lookAt.y, lookAt.z);
	//D3DXVec3Normalize(&mov, &mov);
	XMVECTOR mov = XMVector3Normalize(lookAt);
	//XMFLOAT3 vmov;
	//XMStoreFloat3(&vmov, mov);

	m_position.x += sign * XMVectorGetX(mov); // vmov.x;
	m_position.y += sign * XMVectorGetY(mov); // vmov.y;
	m_position.z += sign * XMVectorGetZ(mov); // vmov.z;


	return m_position;

}


void CameraClass::Rotate(XMFLOAT3 rot)
{
	/*D3DXVec3Normalize(&rot, &rot);
	m_rotationPitch += rot.y;
	m_rotationYaw += rot.x;*/


	// Save mov as a normalise version of rot
	XMFLOAT3 mov;
	XMStoreFloat3(&mov, XMVector3Normalize(XMLoadFloat3(&rot)));

	// Apply normalised rotation
	m_rotation.x += ROTATION_SPEED * mov.y;
	m_rotation.y += ROTATION_SPEED * mov.x;
}


void CameraClass::Render()
{
	//D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);


	// Setup the vector that points upwards.
	/*up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;*/

	// Setup the position of the camera in the world.
	/*position.x = m_position.x;
	position.y = m_position.y;
	position.z = m_position.z;*/

	// Setup where the camera is looking by default.
	/*lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;*/

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * (XM_PI / 180.0);
	yaw = m_rotation.y * (XM_PI / 180.0);
	roll = m_rotation.z * (XM_PI / 180.0);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	//XMVECTOR q1 = XMQuaternionRotationAxis({ 0.0f, 1.0f, 0.0f }, pitch);
	//XMVECTOR q2 = XMQuaternionRotationAxis({ 1.0f, 0.0f, 0.0f }, yaw);
	////XMVECTOR q3 = XMQuaternionRotationAxis({ 0.0f, 0.0f, 1.0f }, roll);
	//rotationMatrix = XMMatrixRotationQuaternion(q1*q2);

	XMVECTOR q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	rotationMatrix = XMMatrixRotationQuaternion(q);
	//rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	XMVECTOR posv = XMLoadFloat3(&m_position);
	// Translate the rotated camera position to the location of the viewer.
	lookAt = posv + lookAt;

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	m_viewMatrix = XMMatrixLookAtLH(posv, lookAt, up);
	//m_viewMatrix = rotationMatrix * XMMatrixTranslationFromVector(-posv);
}


XMMATRIX CameraClass::GetTextureViewMatrix()
{
	return m_texViewMatrix;
}



void CameraClass::RenderTextureView()
{

	float yaw, pitch, roll;

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR position = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);

	XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);



	lookAt = XMVector3TransformCoord(lookAt, rot);
	up = XMVector3TransformCoord(up, rot);


	lookAt = position + lookAt;


	m_texViewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}




XMMATRIX CameraClass::GetViewMatrix()
{
	return m_viewMatrix;
}


XMMATRIX CameraClass::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

XMMATRIX CameraClass::GetOrthogonalMatrix()
{
	return m_orthoMatrix;
}