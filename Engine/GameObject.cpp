#include "GameObject.h"



GameObject::GameObject()
{
	m_model = 0;
	m_assimpModel = 0;
	m_assimpBumpModel = 0;

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
	//m_worldMatrix *= XMMatrixTranslationFromVector(t);
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

	Translate(XMVectorSet(x, y, z, 0.0f));
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

void GameObject::ScaleAtOrigin(XMVECTOR s)
{
	m_worldMatrix *= XMMatrixScalingFromVector(s);
}

void GameObject::ScaleAtOrigin(XMFLOAT3 s)
{
	/*XMMATRIX transform = XMMATRIX(
		 s.x,  0.0f, 0.0f, 0.0f,
		0.0f,  s.y, 0.0f, 0.0f,
		0.0f, 0.0f,  s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

	m_worldMatrix *= XMMatrixScalingFromVector(XMLoadFloat3(&s));
}

void GameObject::ScaleAtOrigin(float sx, float sy, float sz)
{
	//ScaleAtOrigin(XMFLOAT3(sx, sy, sz));
	m_worldMatrix *= XMMatrixScaling(sx, sy, sz);
}


XMMATRIX GameObject::GetWorldMatrix()
{
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