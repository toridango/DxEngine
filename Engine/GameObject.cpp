#include "GameObject.h"



GameObject::GameObject()
{
	m_model = 0;
	m_assimpModel = 0;
	m_assimpBumpModel = 0;

	m_scaling = { 1.0f, 1.0f, 1.0f };
	m_baseRotation = XMMatrixIdentity();

	m_worldMatrix = XMMatrixIdentity();
}


GameObject::~GameObject()
{
	ModelShutdown();
}

void GameObject::ModelShutdown()
{
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}
	if (m_assimpModel)
	{
		m_assimpModel->Shutdown();
		delete m_assimpModel;
		m_assimpModel = 0;
	}
	if (m_assimpBumpModel)
	{
		m_assimpBumpModel->Shutdown();
		delete m_assimpBumpModel;
		m_assimpBumpModel = 0;
	}
}


void GameObject::SetModel(ModelClass* model)
{
	ModelShutdown();
	m_model = model;
	m_modelType = MODEL_RSTTEK;
}

void GameObject::SetModel(AssimpModelClass* model)
{
	ModelShutdown();
	m_assimpModel = model;
	m_modelType = MODEL_ASSIMP;
}

void GameObject::SetModel(AssimpBumpedModelClass* model)
{
	ModelShutdown();
	m_assimpBumpModel = model;
	m_modelType = MODEL_ASSIMPBUMP;
}


void GameObject::SetOffsetRotation(XMMATRIX rot)
{
	m_baseRotation = rot;
	RotateDegreesMatrix(rot);
}

void GameObject::SetOffsetRotation(XMFLOAT3 rot)
{
	SetOffsetRotation(rot.x, rot.y, rot.z);
}

void GameObject::SetOffsetRotation(float x, float y, float z)
{
	float pitch = x * (XM_PI / 180.0);
	float yaw = y * (XM_PI / 180.0);
	float roll = z * (XM_PI / 180.0);
	SetOffsetRotation(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
}




void GameObject::SetTranslation(XMVECTOR t)
{
	// Make sure 4th parameter (w) is 1 before setting
	t = XMVectorSetW(t, 1.0f);

	m_worldMatrix.r[3] = t;
}

void GameObject::SetTranslation(XMFLOAT3 t)
{
	SetTranslation(XMLoadFloat3(&t));
}

void GameObject::SetTranslation(float x, float y, float z)
{
	SetTranslation(XMVectorSet(x, y, z, 1.0f));
}

void GameObject::Translate(XMVECTOR t)
{
	// Make sure 4th parameter (w) is 0 before adding
	t = XMVectorSetW(t, 0.0f);

	m_worldMatrix.r[3] += t;
}

void GameObject::Translate(XMFLOAT3 t)
{
	Translate(XMLoadFloat3(&t));
}

void GameObject::Translate(float x, float y, float z)
{
	/*XMMATRIX transform = XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f);*/

	/*XMMATRIX transform = XMMatrixIdentity();
	XMVectorSetX(transform.r[3], x);
	XMVectorSetY(transform.r[3], y);
	XMVectorSetZ(transform.r[3], z);*/

	/*XMMATRIX transform = XMMatrixIdentity();
	transform.r[3] = XMVectorSet(x, y, z, 1.0f);*/

	Translate({ x, y, z, 0.0f });
}


void GameObject::SetRotationXYZ(XMFLOAT3 r)
{

	float pitch = r.x * (XM_PI / 180.0);
	float yaw = r.y * (XM_PI / 180.0);
	float roll = r.z * (XM_PI / 180.0);

	XMVECTOR trans = m_worldMatrix.r[3];
	m_worldMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&m_scaling)) *
					m_baseRotation *
					XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	m_worldMatrix.r[3] = trans;
}


void GameObject::SetRotationDegAroundAxis(XMVECTOR axis, float angle)
{
	XMVECTOR trans = m_worldMatrix.r[3];
	m_worldMatrix = m_baseRotation *
					XMMatrixScalingFromVector(XMLoadFloat3(&m_scaling)) *
					XMMatrixRotationAxis(axis, angle);
	m_worldMatrix.r[3] = trans;
}


void GameObject::RotateDegAroundAxis(XMVECTOR axis, float angle)
{
	m_worldMatrix *= XMMatrixRotationAxis(axis, angle);
}



void GameObject::RotateDegreesMatrix(XMMATRIX rot)
{
	m_worldMatrix *= rot;
}

void GameObject::RotateDegreesAroundX(float degrees)
{
	RotateDegreesMatrix(XMMatrixRotationX(degrees * (XM_PI / 180.0f)));
}

void GameObject::RotateDegreesAroundY(float degrees)
{
	RotateDegreesMatrix(XMMatrixRotationY(degrees * (XM_PI / 180.0f)));
}

void GameObject::RotateDegreesAroundZ(float degrees)
{
	RotateDegreesMatrix(XMMatrixRotationZ(degrees * (XM_PI / 180.0f)));
}

void GameObject::Scale(XMVECTOR s)
{
	m_worldMatrix *= XMMatrixScalingFromVector(s);
	XMStoreFloat3(&m_scaling, (XMLoadFloat3(&m_scaling) * s));
}

void GameObject::Scale(XMFLOAT3 s)
{
	/*XMMATRIX transform = XMMATRIX(
		 s.x,  0.0f, 0.0f, 0.0f,
		0.0f,  s.y, 0.0f, 0.0f,
		0.0f, 0.0f,  s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

	m_worldMatrix *= XMMatrixScalingFromVector(XMLoadFloat3(&s));
	XMStoreFloat3(&m_scaling, { m_scaling.x * s.x, m_scaling.y * s.y, m_scaling.z * s.z });
}

void GameObject::Scale(float sx, float sy, float sz)
{
	//ScaleAtOrigin(XMFLOAT3(sx, sy, sz));
	m_worldMatrix *= XMMatrixScaling(sx, sy, sz);
	XMStoreFloat3(&m_scaling, { m_scaling.x * sx, m_scaling.y * sy, m_scaling.z * sz });
}


XMFLOAT3 GameObject::GetScaling()
{
	return m_scaling;
}


XMMATRIX GameObject::GetWorldMatrix()
{
	/*m_worldMatrix = XMMatrixIdentity();

	m_worldMatrix *= XMMatrixScalingFromVector(XMLoadFloat3(&m_scaling));

	float pitch = m_rotationXYZ.x * (XM_PI / 180.0);
	float yaw = m_rotationXYZ.y * (XM_PI / 180.0);
	float roll = m_rotationXYZ.z * (XM_PI / 180.0);
	RotateDegreesMatrix(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));


	XMVECTOR t = XMLoadFloat3(&m_translation);
	t = XMVectorSetW(t, 1.0f);
	m_worldMatrix *= XMMatrixTranslationFromVector(t);*/

	return m_worldMatrix;
}

GameObject::MODELTYPE GameObject::GetModelType()
{
	return m_modelType;
}

int GameObject::GetIndexCount()
{
	switch (m_modelType)
	{
	case MODEL_ASSIMPBUMP:
		return m_assimpBumpModel->GetIndexCount();

	case MODEL_RSTTEK:
		return m_model->GetIndexCount();

	case MODEL_ASSIMP:
	default:
		return m_assimpModel->GetIndexCount();
	}
}


ID3D11ShaderResourceView* GameObject::GetTexture()
{
	switch (m_modelType)
	{
	case MODEL_ASSIMPBUMP:
		// This mode has a texture array not a single texture
		return NULL;

	case MODEL_RSTTEK:
		return m_model->GetTexture();

	case MODEL_ASSIMP:
	default:
		return m_assimpModel->GetTexture();
	}
}


ID3D11ShaderResourceView** GameObject::GetTextureArray()
{
	switch (m_modelType)
	{
	case MODEL_ASSIMPBUMP:
		return m_assimpBumpModel->GetTextureArray();

	case MODEL_RSTTEK:
	case MODEL_ASSIMP:
	default:
		// These modes have a single texture, not texture arrays
		return NULL;
	}
}


void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	switch (m_modelType)
	{
	case MODEL_ASSIMPBUMP:
	{
		m_assimpBumpModel->Render(deviceContext);
		break;
	}
	case MODEL_RSTTEK:
	{
		m_model->Render(deviceContext);
		break;
	}
	case MODEL_ASSIMP:
	default:
	{
		m_assimpModel->Render(deviceContext);
		break;
	}
	}
}


void GameObject::Store(std::string keyname, XMFLOAT4 value)
{
	m_miscStorage[keyname] = value;
}

float GameObject::StorageGetFloat(std::string keyname)
{
	return m_miscStorage[keyname].x;
}

XMFLOAT2 GameObject::StorageGetFloat2(std::string keyname)
{
	return { m_miscStorage[keyname].x,
			m_miscStorage[keyname].y };
}

XMFLOAT3 GameObject::StorageGetFloat3(std::string keyname)
{
	return { m_miscStorage[keyname].x,
			m_miscStorage[keyname].y ,
			m_miscStorage[keyname].z };
}

XMFLOAT4 GameObject::StorageGetFloat4(std::string keyname)
{
	return m_miscStorage[keyname];
}