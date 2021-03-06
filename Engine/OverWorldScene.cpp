#include "OverWorldScene.h"



OverWorldScene::OverWorldScene(HWND hwnd, D3DClass* d3d) :
	m_hwnd(hwnd),
	m_D3D(d3d)
{
	m_k = 0;
	m_shootingWingIdx = 0;
	m_fovLerp = 0.0f;
	m_fov = (float)XM_PIDIV4;
	m_aimAssist = true;
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

	if (m_volLaserShader)
	{
		delete m_volLaserShader;
		m_volLaserShader = 0;
	}

	if (m_volBalloonShader)
	{
		delete m_volBalloonShader;
		m_volBalloonShader = 0;
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

	if (m_ModelXW)
	{
		m_ModelXW->Shutdown();
		delete m_ModelXW;
		m_ModelXW = 0;
	}

	if (m_ModelCube)
	{
		m_ModelCube->Shutdown();
		delete m_ModelCube;
		m_ModelCube = 0;
	}

	if (m_ModelSphere)
	{
		m_ModelSphere->Shutdown();
		delete m_ModelSphere;
		m_ModelSphere = 0;
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
	m_cpPaths.push_back(path_sphereModel);

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
	m_wcpPaths.push_back(path_volLaserVertexShader);
	m_wcpPaths.push_back(path_volLaserPixelShader);
	m_wcpPaths.push_back(path_volBalloonVertexShader);
	m_wcpPaths.push_back(path_volBalloonPixelShader);

	CheckAllPaths(m_hwnd);

	m_Camera = camera;

	// Set the initial position of the camera.
	XMFLOAT3 camPos = XMFLOAT3(-50.0f, 30.0f, -50.0f);
	m_Camera->SetPosition(camPos.x, camPos.y, camPos.z);
	m_Camera->SetRotation(0.0f, 45.0f, 0.0f);
	//m_Camera->SetRotation(20.0f, 45.0f, 0.0f);
	m_Camera->Render();



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
	go_rock->SetTranslation(0.0f, -10.0f, 0.0f);


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
	XMFLOAT3 cpF3 = m_Camera->GetPosition();
	XMVECTOR camPos = { cpF3.x, cpF3.y, cpF3.z };
	// Position for scale 1.0
	//XMVECTOR XWPos = camPos - 5.0f * m_Camera->GetUpVector() + 9.0f * m_Camera->GetLookAtVector();
	// Position for scale 0.2 // -0.85, 1.55
	m_XW_upOffset = -0.85f;
	m_XW_lookAtOffset = 1.55f;
	XMVECTOR XWPos = camPos +
		m_XW_upOffset * m_Camera->GetUpVector() +
		m_XW_lookAtOffset * m_Camera->GetLookAtVector();

	//go_xw->Scale(1.0f, 1.0f, 1.0f);
	float xw_scaling = 0.2f;
	go_xw->Scale(xw_scaling, xw_scaling, xw_scaling);
	//go_xw->RotateDegreesAroundX(90.0f);
	go_xw->SetOffsetRotation(90.0f, 180.0f, 0.0f);
	go_xw->SetTranslation(XWPos);


	float h = 4.6f * xw_scaling;
	float v1 = 2.3f * xw_scaling;
	float v2 = 1.1f * xw_scaling;

	m_laserOffsets[0] = {  h, v1 };
	m_laserOffsets[1] = { -h, v1 };
	m_laserOffsets[2] = {  h, v2 };
	m_laserOffsets[3] = { -h, v2 };






	// ------------------ Terrain ------------------

	go_procTerrain = new Terrain();
	go_procTerrain->Initialize(m_D3D->GetDevice(), TERRAINWIDTH, TERRAINHEIGHT,
		L"../Engine/Assets/sand.dds", L"../Engine/Assets/grass.dds",
		L"../Engine/Assets/tundra.dds", L"../Engine/Assets/snow.dds");
	double scaling = 12.0;
	double zoom = 12.0;
	go_procTerrain->GenerateHeightMap(scaling, zoom);
	//OutputDebugStringA(("Scaling: " + std::to_string(scaling) + "\t\tZoom: " + std::to_string(zoom) + "\n").c_str());
	go_procTerrain->SetTranslation(-(float)TERRAINWIDTH / 2.0f, -0.5f, -(float)TERRAINHEIGHT / 2.0f);




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

	//m_laserCubeScale = XMFLOAT3(4.2f, 0.2f, 0.2f);
	m_laserCubeScale = XMFLOAT3(4.5f, 4.5f, 4.5f);
	go_laser->Scale(m_laserCubeScale);
	// the laser capsule (pos set every frame) will need to be in the same position as the bounding volume to be seen
	
	go_laser->SetTranslation(-10.0f, -10.0f, -10.0f);







	// ------------------ Balloon targets (Volumetric Cube) ------------------
	
	m_ModelSphere = new AssimpModelClass;
	if (!m_ModelSphere) { return false; }

	result = m_ModelSphere->Initialize(m_D3D->GetDevice(), path_cubeModel, path_rockAlbedo);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the balloon model object.", L"Error", MB_OK);
		return false;
	}

	float balloonRowCount = std::pow((double)min(TERRAINWIDTH, TERRAINHEIGHT), 1.0 / 4.0);
	go_targets.reserve(balloonRowCount * balloonRowCount);

	float step = min(TERRAINWIDTH, TERRAINHEIGHT) / (balloonRowCount + 1);
	float ix = -(float)TERRAINWIDTH / 2.0f + step;
	float iz = -(float)TERRAINHEIGHT / 2.0f + step;
	float x = ix;
	float z = iz;
	XMFLOAT3 scale = XMFLOAT3(4.5f, 4.5f, 4.5f);

	for (int i = 0; i < balloonRowCount; ++i)
	{
		for (int j = 0; j < balloonRowCount; ++j)
		{
			x = i * step + ix;
			z = j * step + iz;
			// Add noise to the positions using a function from the terrain
			float rx = (go_procTerrain->Rand01() - 0.5f) * 25.0f;
			float ry = (go_procTerrain->Rand01() - 0.5f) * 25.0f;
			float rz = (go_procTerrain->Rand01() - 0.5f) * 25.0f;

			GameObject* balloon = new GameObject();
			balloon->SetModel(m_ModelSphere);

			balloon->Scale(scale);
			balloon->SetTranslation(x + rx, 30.0f + ry, z + rz);

			go_targets.push_back(balloon);
			m_targetCooldowns.push_back(0.0f);
		}
	}

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


	m_volBalloonShader = new VolumetricBalloonShader(m_hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_volBalloonShader) { return false; }

	result = m_volBalloonShader->InitializeShader(path_volBalloonVertexShader, path_volBalloonPixelShader);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the volumetric balloon shader object.", L"Error", MB_OK);
		return false;
	}




	return true;
}




bool OverWorldScene::KeyPressed(KEYBINDS id)
{
	switch (id)
	{
	case SPACE_KEY:
		if (!SpawnLaserShot())
			return false;
		break;
	case SHIFT_KEY:
		SetSprint(true);
		break;
	case T_KEY:
		m_aimAssist = !m_aimAssist;
	default:
		SetSprint(false);
		break;
	}
	return true;
}

bool OverWorldScene::SpawnLaserShot()
{
	bool result;

	if (!LaserShot::IsOnCooldown())
	{
		XMFLOAT3 lookAt;
		XMVECTOR lookAtV = XMVector3Normalize(m_Camera->GetLookAtVector());
		XMVECTOR up = m_Camera->GetUpVector();
		int way = (m_shootingWingIdx % 2 == 0) ? -1.0f : 1.0f;
		//XMMATRIX m = XMMatrixRotationAxis(up, 15.0f);
		// Rotate direction vector 
		//XMVECTOR q = XMQuaternionRotationAxis(up, way * 3.0f * (XM_PI / 180.0f));
		//lookAtV = XMVector3Rotate(lookAtV, q);

		XMVECTOR v = go_xw->GetWorldMatrix().r[3] + 2.0f*lookAtV;
		// LEFT HANDED: right = cross(up, lookAT)
		v += m_laserOffsets[m_shootingWingIdx].x * XMVector3Cross(up, lookAtV);
		v += m_laserOffsets[m_shootingWingIdx].y * up;
		
		XMVECTOR* aimAt = TraceFirstTargetInRadius(XMLoadFloat3(&(m_Camera->GetPosition())), lookAtV);

		if (aimAt && m_aimAssist)
			XMStoreFloat3(&lookAt, XMVector3Normalize(*aimAt));
		else
			XMStoreFloat3(&lookAt, lookAtV);



		LaserShot* shot = new LaserShot(lookAt);
		shot->SetModel(m_ModelCube);
		shot->Scale(m_laserCubeScale);

		shot->SetTranslation(v);

		go_laserQ.push_back(shot);



		// In order to shoot one from each wing every 4
		m_shootingWingIdx = (m_shootingWingIdx + 1) % 4;
		LaserShot::ResetCooldown();
	}

	return true;
}


void OverWorldScene::SetSprint(bool sprint)
{
	float min = 0.00f;
	float max = 0.85f;

	if (sprint)
		m_fovLerp += (max - m_fovLerp) / 8.0f;
	else
		m_fovLerp -= (m_fovLerp - min) / 8.0f;

	m_Camera->SetFOV(m_fovLerp);

}

XMVECTOR* OverWorldScene::TraceFirstTargetInRadius(XMVECTOR pos, XMVECTOR lookAt)
{
	XMVECTOR* aimAt = nullptr;

	// linepoint2 is (pos + lookAt), linepoint1 is pos
	XMVECTOR lp2 = pos + lookAt;

	int i = 0;
	bool found = false;
	//std::wstring ws = L"";

	// using d to store the distance
	std::vector<DepthGO> foundTargets;
	for (int i = 0; i < go_targets.size(); ++i)
	{
		if (m_targetCooldowns[i] <= 0.0f)
		{
			XMVECTOR targetPos = go_targets[i]->GetWorldMatrix().r[3];
			float angle = (180.0f / XM_PI) * XMVectorGetX(XMVector3AngleBetweenVectors(targetPos - pos, lookAt));

			if (angle <= 3.69f)
			{ 
				// aimAt = &(targetPos - pos);
				foundTargets.push_back({
					XMVectorGetX(XMVector3Length(targetPos - pos)), // use distance to sort
					//angle, // use angle to sort
					go_targets[i]
					});
				found = true;
				//ws += std::to_wstring(angle) + L", ";
			}
		}
	}
	if (found)
	{
		// std::wstringstream s;
		// s << ws << std::endl;
		// std::wstring ws = s.str();
		// OutputDebugString(ws.c_str());

		
		std::sort(foundTargets.begin(), foundTargets.end());
		// get closest (DepthGO gets ordered by farthest first)
		aimAt = &(foundTargets.back().go->GetWorldMatrix().r[3] - pos);
	}


	return aimAt;
}




bool OverWorldScene::Update(float deltaTime)
{


	// ------------------ Sky ------------------

	go_sky->SetTranslation(m_Camera->GetPosition());


	// ------------------ XW ------------------

	XMFLOAT3 cpF3 = m_Camera->GetPosition();
	XMVECTOR camPos = { cpF3.x, cpF3.y, cpF3.z };
	XMFLOAT3 camRot = m_Camera->GetRotation();

	XMVECTOR XWPos = camPos +
		m_XW_upOffset * m_Camera->GetUpVector() +
		m_XW_lookAtOffset * m_Camera->GetLookAtVector();

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

	


	// ------------------ Laser Queue ------------------


	LaserShot::UpdateCooldown(deltaTime);
	int expiredCount = 0;
	for (auto it = go_laserQ.cbegin(); it != go_laserQ.cend(); ++it)
	{
		if (!(*it)->HasExpired())
			(*it)->Update(deltaTime);
		else
			++expiredCount;
	}
	for (; expiredCount > 0; --expiredCount)
		go_laserQ.pop_front();


	// ------------------ Collision Check ------------------

	for (int i = 0; i < go_targets.size(); ++i)
	{
		if (m_targetCooldowns[i] > 0.0f)
		{
			m_targetCooldowns[i] -= (deltaTime / 1000.0f);
		}
	}


	for (int i = 0; i < go_targets.size(); ++i)
	{
		for (auto it = go_laserQ.cbegin(); it != go_laserQ.cend(); ++it)
		{
			XMVECTOR len = XMVector4Length((*it)->GetWorldMatrix().r[3] - go_targets[i]->GetWorldMatrix().r[3]);
			XMFLOAT4 x;
			XMStoreFloat4(&x, len);
			if (x.x < 1.75f)
			{
				// seconds
				m_targetCooldowns[i] = 60.0f;
			}

		}
	}




	return true;
}


std::vector<OverWorldScene::DepthGO> OverWorldScene::ArrangeDepthRenderOrder()
{
	std::vector<OverWorldScene::DepthGO> v;

	int i = 0;
	for (auto go : go_targets)
	{
		if (m_targetCooldowns[i] <= 0.0f)
		{
			v.push_back({ DistanceToCamera(go), go });
		}
		++i;
	}


	for (auto it = go_laserQ.cbegin(); it != go_laserQ.cend(); ++it)
	{
		v.push_back({ DistanceToCamera((*it)), (*it) });
	}

	std::sort(v.begin(), v.end());
	return v;
}


bool OverWorldScene::Render(float deltaTime)
{

	bool result;

	m_Camera->Render();




	// ------------------ XW ------------------


	go_xw->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(go_xw, m_Camera, m_Light, deltaTime);

	if (!result) { return false; }





	// ------------------ New Rock ------------------


	//m_ModelRock->Render(m_D3D->GetDeviceContext());
	go_rock->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_BumpMapShader->Render(go_rock, m_Camera, m_Light);

	if (!result) { return false; }




	// ------------------ Sky ------------------


	m_D3D->setSkyMode(true);

	//m_ModelSky->Render(m_D3D->GetDeviceContext());
	go_sky->Render(m_D3D->GetDeviceContext());


	// Render the model using the light shader.
	result = m_SkyShader->Render(go_sky, m_Camera, m_Light, deltaTime);

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

	result = m_WaterShader->Render(go_waterSurface, m_Camera, m_Light, deltaTime);

	if (!result) { return false; }






	// ------------------ Laser ------------------



	float capsuleLen = 4.0f;


	go_laser->Render(m_D3D->GetDeviceContext());

	// Assuming uniform scaling
	result = m_volLaserShader->Render(go_laser, m_Camera, XMFLOAT3(1.0, 0.0, 0.0),
		capsuleLen, go_laser->GetScaling().x, deltaTime);

	if (!result) { return false; }





	// ------------------            Laser Queue            ------------------
	// ------------------                 &                 ------------------
	// ------------------ Balloon targets (Volumetric Cube) ------------------



	std::vector<OverWorldScene::DepthGO> v = ArrangeDepthRenderOrder();

	m_ModelCube->Render(m_D3D->GetDeviceContext());

	for (auto dgo : v)
	{
		LaserShot* ls = dynamic_cast<LaserShot*>(dgo.go);
		if (ls != NULL)
		{
			ls->Render(m_D3D->GetDeviceContext());
			result = m_volLaserShader->Render(ls, m_Camera, ls->GetDirectionFloat3(),
				capsuleLen, ls->GetScaling().x, deltaTime);
		}
		else
		{
			result = m_volBalloonShader->Render(dgo.go, m_Camera, deltaTime);
		}

		if (!result) { return false; }

	}




	m_D3D->TurnOffAlphaBlending();
	// -----------------------------------------------------------------------------------





	// Present the rendered scene to the screen.
	//m_D3D->EndScene();
	return true;
}