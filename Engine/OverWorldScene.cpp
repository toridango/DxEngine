#include "OverWorldScene.h"



OverWorldScene::OverWorldScene(HWND hwnd, D3DClass* d3d) :
	m_hwnd(hwnd),
	m_D3D(d3d)
{
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

	// Release the model object.
	if (m_ModelSky)
	{
		m_ModelSky->Shutdown();
		delete m_ModelSky;
		m_ModelSky = 0;
	}

	// Release the model object.
	/*if (m_ModelRock)
	{
		m_ModelRock->Shutdown();
		delete m_ModelRock;
		m_ModelRock = 0;
	}*/


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
	m_cpPaths.push_back((char*) path_rockModel);

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

	CheckAllPaths(m_hwnd);

	m_Camera = camera;

	// Set the initial position of the camera.
	XMFLOAT3 camPos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	m_Camera->SetPosition(camPos.x, camPos.y, camPos.z);


	if (!InitializeModels()) { return false; }
	if (!InitializeShaders()) { return false; }


	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light) { return false; }

	// Initialize the light object.
	m_Light->SetAmbientColour(1.f, 1.0f, 1.f, 1.0f);
	m_Light->SetDiffuseColour(.6f, .0f, .3f, 1.0f);
	m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
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


	return true;
}


bool OverWorldScene::Render(float deltavalue)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Base colour
	float R, G, B, A;
	R = m_sceneBaseColour.x;
	G = m_sceneBaseColour.y;
	B = m_sceneBaseColour.z;
	A = m_sceneBaseColour.w;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(R, G, B, A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	viewMatrix = m_Camera->GetViewMatrix();
	projectionMatrix = m_Camera->GetProjectionMatrix();




	/*XMFLOAT4 v = m_Light->GetSpecularColour();
	v.x = v.y = v.z = v.w = 0.0f;

	//m_ModelRock->Render(m_D3D->GetDeviceContext());
	go_rock->Render(m_D3D->GetDeviceContext());
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);

	worldMatrix = go_rock->GetWorldMatrix();

	// Render the model using the light shader.
	result = m_BumpMapShader->Render(go_rock, m_Camera, m_Light);

	if (!result) { return false; }*/



	m_D3D->setSkyMode(true);

	//m_ModelSky->Render(m_D3D->GetDeviceContext());
	go_sky->Render(m_D3D->GetDeviceContext());

	go_sky->SetTranslation(m_Camera->GetPosition());
	worldMatrix = go_sky->GetWorldMatrix();

	// Render the model using the light shader.
	result = m_SkyShader->Render(go_sky, m_Camera, m_Light, deltavalue);

	if (!result) { return false; }



	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}