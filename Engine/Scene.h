#pragma once

#include <vector>
#include <string>
#include <codecvt>

#include "d3dclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "soundclass.h"
#include "Utilities.h"
#include "GameObject.h"
#include "Terrain.h"
#include "LaserShot.h"

#include "modelclass.h"
#include "AssimpModelClass.h"
#include "AssimpBumpedModelClass.h"

#include "LightShader.h"
#include "BumpShader.h"
#include "TerrainShader.h"
#include "WaterShader.h"
#include "VolumetricLaserShader.h"
#include "VolumetricBalloonShader.h"


// Abstract Scene
class Scene
{
public:
	enum KEYBINDS
	{
		NONE,
		SPACE_KEY,
		SHIFT_KEY,
		A_KEY,
		D_KEY,
		S_KEY,
		T_KEY,
		W_KEY
	};

public:
	Scene();
	~Scene();

	virtual void Shutdown() = 0;
	virtual bool Initialize(CameraClass* camera) = 0;
	virtual bool Update(float deltaTime) = 0;
	virtual bool Render(float deltavalue) = 0;

	virtual bool KeyPressed(KEYBINDS id) = 0;

	float GetFOV();

	
protected:
	bool CheckPath(char* path);
	bool CheckPath(const char* path);
	bool CheckPath(WCHAR* path);
	bool CheckAllPaths(HWND hwnd);

	float DistanceToCamera(GameObject* go);
	

protected:
	XMFLOAT4 m_sceneBaseColour = {0.6f, 0.1f, 0.5f, 1.0f};

	CameraClass* m_Camera;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;

	std::vector<char*> m_cpPaths;
	std::vector<WCHAR*> m_wcpPaths;
};

