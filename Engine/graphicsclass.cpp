////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Scene = 0;
	m_Camera = 0;
	m_orthoW = 0;
	m_RenderTexture = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	m_sprint = false;
	m_movSpeed = MOV_SPEED_NORMAL;
	m_screenH = screenHeight;
	m_screenW = screenWidth;

	m_D3D = new D3DClass;
	if (!m_D3D) { return false; }
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN);// , SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}


	m_Camera = new CameraClass;
	if (!m_Camera) { return false; }
	result = m_Camera->Initialize(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the camera object.", L"Error", MB_OK);
		return false;
	}


	// -----------      Scene Selection      -------------

	//m_Scene = new SwordRockScene(hwnd, m_D3D);
	m_Scene = new OverWorldScene(hwnd, m_D3D);

	// ---------------------------------------------------


	if (!m_Scene) { return false; }
	m_Scene->Initialize(m_Camera);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the scene object.", L"Error", MB_OK);
		return false;
	}

	m_orthoW = new OrthoWindow();
	if (!m_orthoW) { return false; }
	result = m_orthoW->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the orthographic window object.", L"Error", MB_OK);
		return false;
	}
	m_Camera->RenderTextureView();
	// Get view matrix and save it
	m_orthoView = m_Camera->GetTextureViewMatrix();
	m_orthoMatrix = XMMatrixOrthographicLH(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	m_RenderTexture = new RenderTexture();
	if (!m_RenderTexture) { return false; }
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}


	m_ppShader = new PostProcessShader(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_ppShader) { return false; }
	result = m_ppShader->InitializeShader(L"../Engine/ppShader_vs.hlsl", L"../Engine/ppShader_ps.hlsl");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the post processing shader.", L"Error", MB_OK);
		return false;
	}



	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the Sword Rock Scene.
	if (m_Scene)
	{
		//m_srScene->Shutdown();
		delete m_Scene;
		m_Scene = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_orthoW)
	{
		delete m_orthoW;
		m_orthoW = 0;
	}

	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_ppShader)
	{
		delete m_ppShader;
		m_ppShader = 0;
	}
}




bool GraphicsClass::Frame(Timer* timer)
{
	bool result;
	/*static float rotation = 0.0f;
	static float delta = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Update the delta variable each frame. (keep this between 0 and 1)
	// delta += 0.001f; // original
	delta += 0.1f;
	if (delta > 100000000.0f)
	{
		delta -= 100000000.0f;
	}*/


	result = Update(timer->GetDeltaMilli());
	if (!result) { return false; }


	// Render the graphics scene.
	result = Render(timer->GetMillisecondsElapsed());
	if (!result) { return false; }

	return true;
}



bool GraphicsClass::Update(float deltaTime)
{
	bool result;

	result = m_Scene->Update(deltaTime);
	if (!result)
	{
		return false;
	}
}




/////////////////////////
//     INPUT CODE      //

void GraphicsClass::StrafeLeft()
{
	Strafe(-1.0f * m_movSpeed);
}
void GraphicsClass::StrafeRight()
{
	Strafe(m_movSpeed);
}
void GraphicsClass::MoveForward()
{
	Advance(m_movSpeed);
}
void GraphicsClass::MoveBack()
{
	Advance(-1.0f * m_movSpeed);
}

void GraphicsClass::Strafe(float amount)
{
	m_camPos = m_Camera->Strafe(amount);
}

void GraphicsClass::Advance(float amount)
{
	m_camPos = m_Camera->Advance(amount);
}

void GraphicsClass::Rotate(XMFLOAT3 rot)
{
	m_Camera->Rotate(rot);
}

void GraphicsClass::SetSprint(bool sprint)
{
	m_sprint = sprint;
	if (sprint) m_movSpeed = MOV_SPEED_SPRINT;
	else m_movSpeed = MOV_SPEED_NORMAL;

	if (sprint)
		m_Scene->KeyPressed(Scene::SHIFT_KEY);
	else
		m_Scene->KeyPressed(Scene::NONE);
}

bool GraphicsClass::SpacePressed()
{
	return m_Scene->KeyPressed(Scene::SPACE_KEY);
}

//                     //
/////////////////////////


bool GraphicsClass::Render(float deltaTime)
{
	bool result;



	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(),
										0.0f, 0.0f, 0.0f, 1.0f);


	result = m_Scene->Render(deltaTime);
	if (!result)
	{
		return false;
	}


	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	//m_D3D->ResetViewport();



	RenderTextureToScreen();


	return true;
}

// BLOOM ----------------------------------------------------------------------------
// Render scene into rendertexture with mask shader
// Render result into rendertexture with vertical blur shader
// Render result into rendertexture with horizontal blur shader
// Render result into rendertexture with final shader (addition of blur to scene)
// ----------------------------------------------------------------------------------


bool GraphicsClass::RenderTextureToScreen()
{
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(1.0f, 0.0f, 0.0f, 0.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_D3D->GetWorldMatrix(worldMatrix);
	//m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->SetZBuffer(false);

	//m_orthoView = m_Camera->GetTextureViewMatrix();
	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_orthoW->Render(m_D3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	result = m_ppShader->Render(m_orthoW->GetIndexCount(), m_orthoView, m_orthoMatrix, 
								m_RenderTexture->GetShaderResourceView(), m_sprint);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->SetZBuffer(true);

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
