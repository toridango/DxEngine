////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "AssimpModelClass.h"
#include "AssimpBumpedModelClass.h"
#include "bumpmapshaderclass.h"
#include "LightShader.h"
#include "BumpShader.h"


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
	bool RenderToTexture();
	bool RenderScene();

	//bool FileExists(char* filename);
	//bool FileExists(WCHAR* filename);

private:

	// D3DXVECTOR3 m_camPos;
	XMFLOAT3 m_camPos;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_ModelSky;
	AssimpModelClass* m_ModelSword;
	AssimpBumpedModelClass* m_ModelRock;
	LightShader* m_LightShader;
	LightShader* m_SkyShader;
	LightClass* m_Light;
	RenderTextureClass* m_RenderTexture;
	BumpShader* m_BumpMapShader;
};

#endif