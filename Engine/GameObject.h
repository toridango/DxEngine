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
		MODEL_ASSIMPBUMP,
		MODEL_TERRAIN
	};

public:
	GameObject();
	~GameObject();
	void ModelShutdown();

	void SetModel(ModelClass* model);
	void SetModel(AssimpModelClass* model);
	void SetModel(AssimpBumpedModelClass* model);

	void SetOffsetRotation(XMFLOAT3 rot);
	void SetOffsetRotation(float x, float y, float z);

	void SetTranslation(XMVECTOR t);
	void SetTranslation(XMFLOAT3 t);
	void SetTranslation(float x, float y, float z);

	void Translate(XMVECTOR t);
	void Translate(XMFLOAT3 t);
	void Translate(float x, float y, float z);

	void SetRotationXYZ(XMFLOAT3 r);
	void SetRotationDegAroundAxis(XMVECTOR axis, float angle);

	void RotateDegAroundAxis(XMVECTOR axis, float angle);
	void RotateDegreesAroundX(float degrees);
	void RotateDegreesAroundY(float degrees);
	void RotateDegreesAroundZ(float degrees);

	void Scale(XMFLOAT3 s);
	void Scale(XMVECTOR s);
	void Scale(float sx, float sy, float sz);

	XMMATRIX GetWorldMatrix();
	MODELTYPE GetModelType();
	virtual int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

	virtual void Render(ID3D11DeviceContext* deviceContext);


private:

	void RotateDegreesMatrix(XMMATRIX rot);
	void SetOffsetRotation(XMMATRIX rot);

private:

	XMFLOAT3 m_scaling;
	XMMATRIX m_baseRotation;

	XMMATRIX m_worldMatrix;
	MODELTYPE m_modelType;
	ModelClass* m_model;
	AssimpModelClass* m_assimpModel;
	AssimpBumpedModelClass* m_assimpBumpModel;
};

