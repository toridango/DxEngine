#pragma once

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

class SwordRockScene
{
public:
	SwordRockScene(HWND hwnd, D3DClass* d3d);
	~SwordRockScene();
	void Shutdown();

	bool Initialize(CameraClass* camera);
	bool InitializeModels();
	bool InitializeShaders();

	bool Render(float deltavalue);

private:
	bool CheckPath(/*std::string item,*/ char* path);
	bool CheckPath(/*std::string item,*/ const char* path);
	bool CheckPath(/*std::string item,*/ WCHAR* path);
	bool CheckAllPaths();

private:
	// CLASS VARIABLES
	HWND m_hwnd;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightClass* m_Light;
	SoundClass* m_Sound;

	// GAME OBJECTS;
	GameObject* go_floor;
	GameObject* go_sky;
	GameObject* go_rock;
	GameObject* go_sword;

	// MODELS
	AssimpModelClass* m_Model;
	AssimpModelClass* m_ModelSky;
	AssimpModelClass* m_ModelSword;
	AssimpBumpedModelClass* m_ModelRock;

	// SHADERS
	LightShader* m_LightShader;
	LightShader* m_SkyShader;
	BumpShader* m_BumpMapShader;

	// VARIABLES
	//XMFLOAT3 m_camPos;

	// FILE PATHS
	char* path_soundLoop = "../Engine/Assets/sounds/magical_forest.wav";

	char* path_floorModel = "../Engine/Assets/cube.fbx";
	WCHAR* path_floorTex = L"../Engine/Assets/grassfloor.dds";
	
	char* path_skyModel = "../Engine/Assets/sphere.fbx";
	WCHAR* path_skyTex = L"../Engine/Assets/Skybox/skybox_texture.dds";
	
	const char* path_claymoreModel = "../Engine/Assets/claymore/Mini_claymore.obj";
	WCHAR* path_claymoreTex = L"../Engine/Assets/claymore/Mini_claymore_texture.png";
	
	const char* path_rockModel = "../Engine/Assets/rock/stone.obj";
	WCHAR* path_rockAlbedo = L"../Engine/Assets/rock/tex/stone_albedo.png";
	WCHAR* path_rockNormal = L"../Engine/Assets/rock/tex/stone_normal.png";
	WCHAR* path_rockDiffuse = L"../Engine/Assets/rock/tex/stone_diffuse.png";
	WCHAR* path_rockAmbientOcclusion = L"../Engine/Assets/rock/tex/stone_ao.png";
	WCHAR* path_rockSpecular = L"../Engine/Assets/rock/tex/stone_specular.png";


	WCHAR* path_lightVertexShader = L"../Engine/light.vs";
	WCHAR* path_lightPixelShader = L"../Engine/light.ps";
	WCHAR* path_skyVertexShader = L"../Engine/sky.vs";
	WCHAR* path_skyPixelShader = L"../Engine/sky.ps";
	WCHAR* path_bumpVertexShader = L"../Engine/bumpmap.vs";
	WCHAR* path_bumpPixelShader = L"../Engine/bumpmap.ps";
};

