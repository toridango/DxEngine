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
#include "OrthoWindow.h"
#include "RenderTexture.h"
#include "PostProcessShader.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float MOV_SPEED_NORMAL = 0.166f;
const float MOV_SPEED_SPRINT = 1.0f; // *6 normal


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
	void SetSprint(bool sprint);

private:
	bool Render(float, float);
	bool RenderTextureToScreen();


private:

	bool m_sprint;
	float m_movSpeed;
	int m_screenH;
	int m_screenW;

	D3DClass* m_D3D;
	Scene* m_Scene;
	CameraClass* m_Camera;
	XMFLOAT3 m_camPos;

	OrthoWindow* m_orthoW;
	RenderTexture* m_RenderTexture;
	PostProcessShader* m_ppShader;
	XMMATRIX m_orthoMatrix;
	XMMATRIX m_orthoView;

};

#endif