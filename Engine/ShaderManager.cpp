#include "ShaderManager.h"



ShaderManager::ShaderManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	m_lightShader = new LightShader(hwnd, device, immediateContext);
	m_skyShader = new LightShader(hwnd, device, immediateContext);
	m_bumpShader = new BumpShader(hwnd, device, immediateContext);
}


ShaderManager::~ShaderManager()
{
	if (m_lightShader)
	{
		delete m_lightShader;
		m_lightShader = 0;
	}
	if (m_skyShader)
	{
		delete m_skyShader;
		m_skyShader = 0;
	}
	if (m_bumpShader)
	{
		delete m_bumpShader;
		m_bumpShader = 0;
	}
}

bool ShaderManager::Initialize(SHADERID id)
{
	HRESULT result;

	switch (id)
	{
	case SKYSHADER:
	{
		result = m_skyShader->InitializeShader(m_filenames[id]);
		if (FAILED(result)) { return result; }
	}
	case BUMPSHADER:
	{
		result = m_bumpShader->InitializeShader(m_filenames[id]);
		if (FAILED(result)) { return result; }
	}
	case LIGHTSHADER:
	default:
	{
		result = m_lightShader->InitializeShader(m_filenames[id]);
		if (FAILED(result)) { return result; }
	}

	}
}

bool ShaderManager::InitializeShaders()
{
	HRESULT result;

	result = Initialize(LIGHTSHADER);
	if (FAILED(result)) { return result; }

	result = Initialize(SKYSHADER);
	if (FAILED(result)) { return result; }

	result = Initialize(BUMPSHADER);
	if (FAILED(result)) { return result; }


}


//bool ShaderManager::RenderModel(ModelClass* m, CameraClass* c, LightClass* l)
//{
//
//	return m_lightShader->Render();
//}