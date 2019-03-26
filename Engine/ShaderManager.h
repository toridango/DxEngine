#pragma once

#include <vector>

#include "LightShader.h"
#include "BumpShader.h"

enum SHADERID 
{
	LIGHTSHADER,
	SKYSHADER,
	BUMPSHADER
	
};

class ShaderManager
{
private:

	const std::vector<ShaderFiles> m_filenames 
	{
		{ L"../Engine/light.vs", L"../Engine/light.ps" },
		{ L"../Engine/bumpmap.vs", L"../Engine/bumpmap.ps" },
		{ L"../Engine/sky.vs", L"../Engine/sky.ps" }
	};

public:
	ShaderManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~ShaderManager();
	bool InitializeShaders();

	bool RenderModel();

private:
	bool Initialize(SHADERID id);

private:
	LightShader* m_lightShader;
	LightShader* m_skyShader;
	BumpShader* m_bumpShader;

};

