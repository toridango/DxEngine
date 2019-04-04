////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Scene = 0;
	m_Camera = 0;
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


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D) { return false; }

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN);// , SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera) { return false; }
	m_Camera->Initialize(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);

	//m_Scene = new SwordRockScene(hwnd, m_D3D);
	m_Scene = new OverWorldScene(hwnd, m_D3D);
	m_Scene->Initialize(m_Camera);



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

}




bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;
	static float delta = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Update the delta variable each frame. (keep this between 0 and 1)
	delta += 0.001;
	if (delta > 100000000.0f)
	{
		delta -= 100000000.0f;
	}

	// Render the graphics scene.
	result = Render(rotation, delta);
	if (!result) { return false; }

	return true;
}

/////////////////////////
//     INPUT CODE      //

void GraphicsClass::StrafeLeft()
{
	Strafe(-1.0 * MOV_SPEED);
}
void GraphicsClass::StrafeRight()
{
	Strafe(MOV_SPEED);
}
void GraphicsClass::MoveForward()
{
	Advance(MOV_SPEED);
}
void GraphicsClass::MoveBack()
{
	Advance(-1.0 * MOV_SPEED);
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

//                     //
/////////////////////////


bool GraphicsClass::Render(float rotation, float deltavalue)
{
	//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	result = m_Scene->Render(deltavalue);
	if (!result)
	{
		return false;
	}


	return true;
}
