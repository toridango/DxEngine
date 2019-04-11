#pragma once

#include <d3d11.h>
#include <string>
#include <string>
#include <sstream>
#include <iostream>


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
	double m_k; // iteration variable that was being used to watch the terrain change

	// GAME OBJECTS;
	GameObject* go_sky;
	GameObject* go_rock;
	GameObject* go_xw;
	Terrain* go_procTerrain;
	Terrain* go_waterSurface;

	// MODELS
	AssimpModelClass* m_ModelXW;
	AssimpModelClass* m_ModelSky;
	AssimpBumpedModelClass* m_ModelRock;

	// SHADERS
	LightShader* m_LightShader;
	LightShader* m_SkyShader;
	BumpShader* m_BumpMapShader;
	TerrainShader* m_TerrainShader;
	WaterShader* m_WaterShader;

	// FILE PATHS
	char* path_soundLoop = "../Engine/Assets/sounds/magical_forest.wav";

	char* path_skyModel = "../Engine/Assets/sphere.fbx";
	WCHAR* path_skyTex = L"../Engine/Assets/Skybox/skybox_texture.dds";
	
	char* path_xwModel = "../Engine/Assets/XW/xStarFighterCentered.fbx";
	WCHAR* path_xwTex = L"../Engine/Assets/xW/Texture.png";

	char* path_rockModel = "../Engine/Assets/rock/stone.obj";
	WCHAR* path_rockAlbedo = L"../Engine/Assets/rock/tex/stone_albedo.png";
	WCHAR* path_rockNormal = L"../Engine/Assets/rock/tex/stone_normal.png";
	WCHAR* path_rockDiffuse = L"../Engine/Assets/rock/tex/stone_diffuse.png";
	WCHAR* path_rockAmbientOcclusion = L"../Engine/Assets/rock/tex/stone_ao.png";
	WCHAR* path_rockSpecular = L"../Engine/Assets/rock/tex/stone_specular.png";


	WCHAR* path_lightVertexShader = L"../Engine/light_vs.hlsl";
	WCHAR* path_lightPixelShader = L"../Engine/light_ps.hlsl";
	WCHAR* path_skyVertexShader = L"../Engine/sky_vs.hlsl";
	WCHAR* path_skyPixelShader = L"../Engine/sky_ps.hlsl";
	WCHAR* path_bumpVertexShader = L"../Engine/bumpmap_vs.hlsl";
	WCHAR* path_bumpPixelShader = L"../Engine/bumpmap_ps.hlsl";
	WCHAR* path_terrainVertexShader = L"../Engine/terrain_vs.hlsl";
	WCHAR* path_terrainPixelShader = L"../Engine/terrain_ps.hlsl";
	WCHAR* path_waterVertexShader = L"../Engine/water_vs.hlsl";
	WCHAR* path_waterPixelShader = L"../Engine/water_ps.hlsl";
};

