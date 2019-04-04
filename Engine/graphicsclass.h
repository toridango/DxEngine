////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "SwordRockScene.h"
#include "OverWorldScene.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float MOV_SPEED = 0.166f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void StrafeLeft();
	void StrafeRight();
	void MoveForward();
	void MoveBack();
	void Strafe(float);
	void Advance(float);
	void Rotate(XMFLOAT3);

private:
	bool Render(float, float);


private:

	D3DClass* m_D3D;
	//SwordRockScene* m_Scene;
	Scene* m_Scene;
	CameraClass* m_Camera;
	XMFLOAT3 m_camPos;
};

#endif