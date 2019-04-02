#pragma once


#include "modelclass.h"
#include "AssimpModelClass.h"
#include "AssimpBumpedModelClass.h"


class GameObject
{
public:
	enum MODELTYPE
	{
		MODEL_RSTTEK,
		MODEL_ASSIMP,
		MODEL_ASSIMPBUMP
	};

public:
	GameObject();
	~GameObject();
	void ModelShutdown();

	void SetModel(ModelClass* model);
	void SetModel(AssimpModelClass* model);
	void SetModel(AssimpBumpedModelClass* model);

	void SetTranslation(XMVECTOR t);
	void SetTranslation(XMFLOAT3 t);
	void SetTranslation(float x, float y, float z);

	void Translate(XMVECTOR t);
	void Translate(XMFLOAT3 t);
	void Translate(float x, float y, float z);

	void RotateDegreesMatrix(XMMATRIX rot);
	void RotateDegreesAroundX(float degrees);
	void RotateDegreesAroundY(float degrees);
	void RotateDegreesAroundZ(float degrees);

	void ScaleAtOrigin(XMFLOAT3 s);
	void ScaleAtOrigin(XMVECTOR s);
	void ScaleAtOrigin(float sx, float sy, float sz);

	XMMATRIX GetWorldMatrix();
	MODELTYPE GetModelType();
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

	void Render(ID3D11DeviceContext* deviceContext);

private:

	XMMATRIX m_worldMatrix;
	MODELTYPE m_modelType;
	ModelClass* m_model;
	AssimpModelClass* m_assimpModel;
	AssimpBumpedModelClass* m_assimpBumpModel;
};

