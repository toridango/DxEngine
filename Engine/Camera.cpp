#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}




bool Camera::Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	float fieldOfView = (float)XM_PIDIV4;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);


	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);



	return true;
}




XMFLOAT3 Camera::GetPosition()
{
	return m_position;
}


XMMATRIX Camera::GetRotation()
{
	return m_viewMatrix;
}

XMVECTOR Camera::GetLookAtVector()
{
	// Copy view matrix
	XMMATRIX viewMatrix(m_viewMatrix);
	// Invert it
	viewMatrix = XMMatrixInverse(&XMMatrixDeterminant(viewMatrix), viewMatrix);
	// Forward vector (use 0, 0, -1 if you have a right-handed coordinate system)
	XMVECTOR lookAtInViewSpace = { 0.0, 0.0, 1.0 };
	// Transform into world space. Also: transform into normal 
	//XMVECTOR lookAtInWorldSpace = lookAtInViewSpace * viewMatrix; 
	return XMVector3TransformNormal(lookAtInViewSpace, viewMatrix);
}

XMVECTOR Camera::GetUpVector()
{
	return m_viewMatrix.r[1];
}



XMFLOAT3 Camera::Strafe(float sign) // + for right, - for left
{
	XMVECTOR up = GetUpVector();
	XMVECTOR lookAt = GetLookAtVector();
	XMVECTOR right;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// LEFT HANDED CROSS PRODUCT
	// https://softwareengineering.stackexchange.com/questions/17519/why-does-directx-use-a-left-handed-coordinate-system
	right = XMVector3Cross(up, lookAt);


	XMVECTOR mov = XMVector3Normalize(right);
	XMFLOAT3 vmov;
	XMStoreFloat3(&vmov, mov);
	m_position.x += sign * vmov.x;
	m_position.y += sign * vmov.y;
	m_position.z += sign * vmov.z;

	return m_position;

}

XMFLOAT3 Camera::Advance(float sign) // + for forward, - for backward
{
	XMVECTOR lookAt = GetLookAtVector();

	XMVECTOR mov = sign * XMVector3Normalize(lookAt);

	m_worldMatrix.r[3] += mov;


	return m_position;

}


void Camera::Rotate(float pitch, float yaw, float roll)
{

	// Save mov as a normalise version of rot
	XMFLOAT3 mov;
	//XMStoreFloat3(&mov, XMVector3Normalize(XMLoadFloat3(&rot)));
	//
	//// Apply normalised rotation
	//m_rotation.x += ROTATION_SPEED * mov.y;
	//m_rotation.y += ROTATION_SPEED * mov.x;
}


//void Camera::Render()
//{
//	//D3DXVECTOR3 up, position, lookAt;
//	float yaw, pitch, roll;
//	XMMATRIX rotationMatrix;
//
//	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//
//
//	// Setup the vector that points upwards.
//	/*up.x = 0.0f;
//	up.y = 1.0f;
//	up.z = 0.0f;*/
//
//	// Setup the position of the camera in the world.
//	/*position.x = m_position.x;
//	position.y = m_position.y;
//	position.z = m_position.z;*/
//
//	// Setup where the camera is looking by default.
//	/*lookAt.x = 0.0f;
//	lookAt.y = 0.0f;
//	lookAt.z = 1.0f;*/
//
//	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
//	pitch = m_rotation.x * (XM_PI / 180.0);
//	yaw = m_rotation.y * (XM_PI / 180.0);
//	roll = m_rotation.z * (XM_PI / 180.0);
//
//	// Create the rotation matrix from the yaw, pitch, and roll values.
//	//XMVECTOR q1 = XMQuaternionRotationAxis({ 0.0f, 1.0f, 0.0f }, pitch);
//	//XMVECTOR q2 = XMQuaternionRotationAxis({ 1.0f, 0.0f, 0.0f }, yaw);
//	//XMVECTOR q3 = XMQuaternionRotationAxis({ 0.0f, 0.0f, 1.0f }, roll);
//	//rotationMatrix = XMMatrixRotationQuaternion(q1*q2);
//
//	XMVECTOR q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
//	rotationMatrix = XMMatrixRotationQuaternion(q);
//	//rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
//	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
//	up = XMVector3TransformCoord(up, rotationMatrix);
//
//	XMVECTOR posv = XMLoadFloat3(&m_position);
//	// Translate the rotated camera position to the location of the viewer.
//	lookAt = posv + lookAt;
//
//	// Finally create the view matrix from the three updated vectors.
//	//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
//	m_viewMatrix = XMMatrixLookAtLH(posv, lookAt, up);
//	//m_viewMatrix = rotationMatrix * XMMatrixTranslationFromVector(-posv);
//}


XMMATRIX Camera::GetTextureViewMatrix()
{
	return m_texViewMatrix;
}



void Camera::RenderTextureView()
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




XMMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}


XMMATRIX Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

XMMATRIX Camera::GetOrthogonalMatrix()
{
	return m_orthoMatrix;
}