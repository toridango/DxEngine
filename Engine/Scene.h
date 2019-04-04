#pragma once

#include <vector>
#include <string>
#include <codecvt>

#include "d3dclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "soundclass.h"
#include "modelclass.h"
#include "AssimpModelClass.h"
#include "AssimpBumpedModelClass.h"
#include "LightShader.h"
#include "BumpShader.h"
#include "GameObject.h"
#include "Utilities.h"

// Abstract Scene
class Scene
{
public:
	Scene();
	~Scene();

	virtual void Shutdown() = 0;
	virtual bool Initialize(CameraClass* camera) = 0;
	virtual bool Render(float deltavalue) = 0;

	
protected:
	bool CheckPath(char* path);
	bool CheckPath(const char* path);
	bool CheckPath(WCHAR* path);
	bool CheckAllPaths(HWND hwnd);
	

protected:
	XMFLOAT4 m_sceneBaseColour = {0.6f, 0.1f, 0.5f, 1.0f};

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;

	std::vector<char*> m_cpPaths;
	std::vector<WCHAR*> m_wcpPaths;
};

