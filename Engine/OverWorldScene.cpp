#include "OverWorldScene.h"



OverWorldScene::OverWorldScene(HWND hwnd, D3DClass* d3d) :
	m_hwnd(hwnd),
	m_D3D(d3d)
{
	m_k = 0;
}


OverWorldScene::~OverWorldScene()
{
	Shutdown();
}

void OverWorldScene::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		//m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the light shader object.
	if (m_SkyShader)
	{
		//m_SkyShader->Shutdown();
		delete m_SkyShader;
		m_SkyShader = 0;
	}

	// Release the bump map shader object.
	if (m_BumpMapShader)
	{
		//m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}


	if (m_TerrainShader)
	{
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}


	if (m_WaterShader)
	{
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the model object.
	if (m_ModelSky)
	{
		m_ModelSky->Shutdown();
		delete m_ModelSky;
		m_ModelSky = 0;
	}

	// Release the model object.
	if (m_ModelRock)
	{
		m_ModelRock->Shutdown();
		delete m_ModelRock;
		m_ModelRock = 0;
	}


	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	/*if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}*/



	return;
}





bool OverWorldScene::Initialize(CameraClass* camera)
{
	bool result;


	m_cpPaths.push_back(path_soundLoop);
	m_cpPaths.push_back(path_skyModel);
	m_cpPaths.push_back(/*(char*) */path_rockModel);
	m_cpPaths.push_back(path_xwModel);
	m_cpPaths.push_back(path_cubeModel);

	m_wcpPaths.push_back(path_skyTex);
	m_wcpPaths.push_back(path_rockAlbedo);
	m_wcpPaths.push_back(path_rockNormal);
	m_wcpPaths.push_back(path_rockDiffuse);
	m_wcpPaths.push_back(path_rockAmbientOcclusion);
	m_wcpPaths.push_back(path_rockSpecular);
	m_wcpPaths.push_back(path_lightVertexShader);
	m_wcpPaths.push_back(path_lightPixelShader);
	m_wcpPaths.push_back(path_skyVertexShader);
	m_wcpPaths.push_back(path_skyPixelShader);
	m_wcpPaths.push_back(path_bumpVertexShader);
	m_wcpPaths.push_back(path_bumpPixelShader);
	m_wcpPaths.push_back(path_terrainVertexShader);
	m_wcpPaths.push_back(path_terrainPixelShader);
	m_wcpPaths.push_back(path_xwTex);
	m_wcpPaths.push_back(path_waterVertexShader);
	m_wcpPaths.push_back(path_waterPixelShader);

	CheckAllPaths(m_hwnd);

	m_Camera = camera;

	// Set the initial position of the camera.
	XMFLOAT3 camPos = XMFLOAT3(-50.0f, 30.0f, -50.0f);
	m_Camera->SetPosition(camPos.x, camPos.y, camPos.z);
	m_Camera->SetRotation(20.0f, 45.0f, 0.0f);
	//m_Camera->SetRotation(0.0f, 0.0f, 0.0f);


	if (!InitializeModels()) { return false; }
	if (!InitializeShaders()) { return false; }


	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light) { return false; }

	// Initialize the light object.
	//m_Light->SetAmbientColour(1.f, 1.0f, 1.f, 1.0f);
	//m_Light->SetDiffuseColour(.6f, .0f, .3f, 1.0f);
	m_Light->SetAmbientColour(0.8f, 0.8f, 0.8f, 1.0f);
	m_Light->SetDiffuseColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -0.3f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Was in render
	m_Light->SetDirection(cos(30), sin(30), 0.0f);


	// Create the sound object
	/*m_Sound = new SoundClass;
	if (!m_Sound) { return false; }

	// Initialise the sound object
	// NOTE: This won't play the .wav file
	result = m_Sound->Initialize(m_hwnd, path_soundLoop);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialise Direct Sound.", L"Error", MB_OK);
		return false;
	}*/



	return true;
}


bool OverWorldScene::InitializeModels()
{
	bool result;


	// ------------------ Sky ------------------

	// Create the model object.
	// WARNING: Before adapting this to only assimp models, you need to 
	// adapt the cubemap options from modelclass into assimpmodelclass
	m_ModelSky = new AssimpModelClass;
	if (!m_ModelSky) { return false; }

	// Initialize the model object.
	result = m_ModelSky->Initialize(m_D3D->GetDevice(), path_skyModel, path_skyTex, true);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the sky model object.", L"Error", MB_OK);
		return false;
	}

	go_sky = new GameObject();
	go_sky->SetModel(m_ModelSky);

	go_sky->SetTranslation(m_Camera->GetPosition());




	// ------------------ New Rock ------------------

	m_ModelRock = new AssimpBumpedModelClass;
	if (!m_ModelRock) { return false; }

	result = m_ModelRock->Initialize(m_D3D->GetDevice(), path_rockModel,
		path_rockAlbedo,
		path_rockNormal,
		path_rockDiffuse,
		path_rockAmbientOcclusion,
		path_rockSpecular);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the rock model object.", L"Error", MB_OK);
		return false;
	}

	go_rock = new GameObject();
	go_rock->SetModel(m_ModelRock);

	go_rock->Scale(3.0f, 3.0f, 3.0f);
	go_rock->SetTranslation(0.0f, 1.6f, 0.0f);


	// ------------------ XW ------------------

	m_ModelXW = new AssimpModelClass;
	if (!m_ModelXW) { return false; }

	result = m_ModelXW->Initialize(m_D3D->GetDevice(), path_xwModel, path_xwTex);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the rock model object.", L"Error", MB_OK);
		return false;
	}

	go_xw = new GameObject();
	go_xw->SetModel(m_ModelXW);

	//go_xw->Scale(1.0f, 1.0f, 1.0f);
	//go_xw->RotateDegreesAroundX(90.0f);
	go_xw->SetOffsetRotation(90.0f, 180.0f, 0.0f);
	//go_xw->SetTranslation(0.0f, 18.6f, 0.0f);

	/*XMFLOAT3 cpF3 = m_Camera->GetPosition();
	XMVECTOR camPos = { cpF3.x, cpF3.y, cpF3.z };
	XMVECTOR XWPos = XMVectorAdd(camPos, m_Camera->GetUpVector());
	XMVECTOR haha = m_Camera->GetLookAtVector();
	XWPos = XMVectorAdd(XWPos, haha);*/

	//go_xw->SetTranslation(XWPos);




	// ------------------ Terrain ------------------

	go_procTerrain = new Terrain();
	go_procTerrain->Initialize(m_D3D->GetDevice(), TERRAINWIDTH, TERRAINHEIGHT,
		L"../Engine/Assets/sand.dds", L"../Engine/Assets/grass.dds",
		L"../Engine/Assets/tundra.dds", L"../Engine/Assets/snow.dds");
	double scaling = 12.0;
	double zoom = 12.0;
	go_procTerrain->GenerateHeightMap(scaling, zoom);
	//OutputDebugStringA(("Scaling: " + std::to_string(scaling) + "\t\tZoom: " + std::to_string(zoom) + "\n").c_str());
	go_procTerrain->SetTranslation(-(float)TERRAINWIDTH/2.0f, -0.5f, -(float)TERRAINHEIGHT/2.0f);




	// ------------------ Water ------------------
	float terrainMaxH = (float)go_procTerrain->GetMaxHeight();
	float terrainMinH = (float)go_procTerrain->GetMinHeight();

	go_waterSurface = new Terrain();
	go_waterSurface->Initialize(m_D3D->GetDevice(), TERRAINWIDTH, TERRAINHEIGHT, 0.0f);
	//OutputDebugStringA(("Scaling: " + std::to_string(scaling) + "\t\tZoom: " + std::to_string(zoom) + "\n").c_str());
	float elevation = 0.19*(terrainMaxH - terrainMinH) + terrainMinH;
	go_waterSurface->SetTranslation(-(float)TERRAINWIDTH / 2.0f, elevation, -(float)TERRAINHEIGHT / 2.0f);




	// ------------------ Laser (Volumetric Cube) ------------------

	m_ModelCube = new AssimpModelClass;
	if (!m_ModelCube) { return false; }

	result = m_ModelCube->Initialize(m_D3D->GetDevice(), path_cubeModel, path_rockAlbedo);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the laser model object.", L"Error", MB_OK);
		return false;
	}

	go_laser = new GameObject();
	go_laser->SetModel(m_ModelCube);

	go_laser->Scale(3.0f, 3.0f, 3.0f);
	go_laser->SetTranslation(0.0f, 30.0f, 0.0f);



	return true;
}


bool OverWorldScene::InitializeShaders()
{
	bool result;

	// Create the light shader object.
	m_LightShader = new LightShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_LightShader) { return false; }

	// Initialize the light shader object.
	result = m_LightShader->InitializeShader(path_lightVertexShader, path_lightPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}



	// Create the sky shader object.
	m_SkyShader = new LightShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_SkyShader) { return false; }

	result = m_SkyShader->InitializeShader(path_skyVertexShader, path_skyPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the sky light shader object.", L"Error", MB_OK);
		return false;
	}



	// Create the bump map shader object.
	m_BumpMapShader = new BumpShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_BumpMapShader) { return false; }

	// Initialize the bump map shader object.
	result = m_BumpMapShader->InitializeShader(path_bumpVertexShader, path_bumpPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the bump map shader object.", L"Error", MB_OK);
		return false;
	}


	m_TerrainShader = new TerrainShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_TerrainShader) { return false; }

	result = m_TerrainShader->InitializeShader(path_terrainVertexShader, path_terrainPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}



	m_WaterShader = new WaterShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_WaterShader) { return false; }

	result = m_WaterShader->InitializeShader(path_waterVertexShader, path_waterPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}


	m_volLaserShader = new VolumetricLaserShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_volLaserShader) { return false; }

	result = m_volLaserShader->InitializeShader(path_volLaserVertexShader, path_volLaserPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the volumetric laser shader object.", L"Error", MB_OK);
		return false;
	}



	return true;
}


bool OverWorldScene::Render(float deltavalue)
{

	bool result;

	// Base colour
	//float R, G, B, A;
	//R = m_sceneBaseColour.x;
	//G = m_sceneBaseColour.y;
	//B = m_sceneBaseColour.z;
	//A = m_sceneBaseColour.w;

	// Clear the buffers to begin the scene.
	// m_D3D->BeginScene(R, G, B, A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();


	// ------------------ New Rock ------------------


	//m_ModelRock->Render(m_D3D->GetDeviceContext());
	go_rock->Render(m_D3D->GetDeviceContext());
	
	// Render the model using the light shader.
	result = m_BumpMapShader->Render(go_rock, m_Camera, m_Light);

	if (!result) { return false; }



	// ------------------ XW ------------------


	XMFLOAT3 cpF3 = m_Camera->GetPosition();
	XMVECTOR camPos = { cpF3.x, cpF3.y, cpF3.z };
	XMVECTOR XWPos = XMVectorAdd(camPos, -5.0f*m_Camera->GetUpVector());
	XMFLOAT3 camRot = m_Camera->GetRotation();
	XWPos = XMVectorAdd(XWPos, 9.0f * m_Camera->GetLookAtVector());

	// OUTPUTTING STUFF TO THE DEBUGGER // FOR FUTURE REFERENCE
	/*XMStoreFloat3(&cpF3, haha);
	std::wstringstream s;
	s << cpF3.x << ", " << cpF3.y << ", " << cpF3.z << std::endl;
	std::wstring ws = s.str();
	OutputDebugString(ws.c_str());*/


	//XMVECTOR r = XMVector3Cross(m_Camera->GetLookAtVector(), m_Camera->GetUpVector());
	//go_xw->SetRotationDegAroundAxis(r, 10.0f);
	go_xw->SetRotationXYZ(camRot);

	go_xw->SetTranslation(XWPos);

	go_xw->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(go_xw, m_Camera, m_Light, deltavalue);

	if (!result) { return false; }



	// ------------------ Sky ------------------


	m_D3D->setSkyMode(true);

	//m_ModelSky->Render(m_D3D->GetDeviceContext());
	go_sky->Render(m_D3D->GetDeviceContext());

	go_sky->SetTranslation(m_Camera->GetPosition());

	// Render the model using the light shader.
	result = m_SkyShader->Render(go_sky, m_Camera, m_Light, deltavalue);

	if (!result) { return false; }

	m_D3D->setSkyMode(false);



	// ------------------ Terrain ------------------
	
	// Stupid but fun animation to see the parameters increasing
	/*go_procTerrain->ModelShutdown();
	go_procTerrain->Initialize(m_D3D->GetDevice(), TERRAINWIDTH, TERRAINHEIGHT);
	go_procTerrain->GenerateHeightMap(m_k, m_k*2);
	go_procTerrain->Smooth();
	OutputDebugStringA(("Scaling: " + std::to_string(m_k) + "\t\tZoom: " + std::to_string(m_k*2) + "\n").c_str());
	m_k += 0.05;*/
	

	go_procTerrain->Render(m_D3D->GetDeviceContext());

	result = m_TerrainShader->Render(go_procTerrain, m_Camera, m_Light);

	if (!result) { return false; }



	// REGION: ALPHA BLENDING ON ---------------------------------------------------------

	m_D3D->TurnOnAlphaBlending();

	// ------------------ Water ------------------
	

	go_waterSurface->Render(m_D3D->GetDeviceContext());

	result = m_WaterShader->Render(go_waterSurface, m_Camera, m_Light, deltavalue);

	if (!result) { return false; }

	//m_D3D->TurnOffAlphaBlending();


	

	// ------------------ Laser ------------------




	go_laser->Render(m_D3D->GetDeviceContext());

	// Render the model using its shader.
	//result = m_LightShader->Render(go_laser, m_Camera, m_Light, deltavalue);
	result = m_volLaserShader->Render(go_laser, m_Camera, m_Light, deltavalue);

	if (!result) { return false; }


	m_D3D->TurnOffAlphaBlending();
	// -----------------------------------------------------------------------------------





	// Present the rendered scene to the screen.
	//m_D3D->EndScene();
	return true;
}