#pragma once


#include "Scene.h"

class OverWorldScene : public Scene
{
public:
	OverWorldScene(HWND hwnd, D3DClass* d3d);
	~OverWorldScene();

	void Shutdown();

	bool Initialize(CameraClass* camera);
	bool InitializeModels();
	bool InitializeShaders();

	bool Render(float deltavalue);



private:
	// CLASS VARIABLES
	HWND m_hwnd;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightClass* m_Light;
	SoundClass* m_Sound;

	// GAME OBJECTS;
	GameObject* go_sky;
	GameObject* go_rock;

	// MODELS
	AssimpModelClass* m_Model;
	AssimpModelClass* m_ModelSky;
	AssimpBumpedModelClass* m_ModelRock;

	// SHADERS
	LightShader* m_LightShader;
	LightShader* m_SkyShader;
	BumpShader* m_BumpMapShader;


	// FILE PATHS
	char* path_soundLoop = "../Engine/Assets/sounds/magical_forest.wav";

	char* path_skyModel = "../Engine/Assets/sphere.fbx";
	WCHAR* path_skyTex = L"../Engine/Assets/Skybox/skybox_texture.dds";

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

