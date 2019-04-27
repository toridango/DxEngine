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
#include "Timer.h"
#include "PPSEdgeDetection.h"

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
	bool Frame(Timer* timer);
	void StrafeLeft();
	void StrafeRight();
	void MoveForward();
	void MoveBack();
	void Strafe(float);
	void Advance(float);
	void Rotate(XMFLOAT3);
	void SetSprint(bool sprint);
	bool SpacePressed();
	void TPressed();

private:
	bool Update(float deltaTime);
	bool Render(float deltaTime);
	bool RenderTextureToScreen();


private:

	bool m_sprint;
	bool m_aimAssist;
	float m_T_cooldown;
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
	PPSEdgeDetection* m_ppsEdgeShader;
	XMMATRIX m_orthoMatrix;
	XMMATRIX m_orthoView;

};

#endif