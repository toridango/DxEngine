////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <DirectXMath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);// , float screenDepth, float screenNear);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	/*XMMATRIX GetProjectionMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();*/

	void GetVideoCardInfo(char*, int&);

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	//void SetBackBufferRenderTarget(bool mode);

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetZBuffer(bool mode);
	void SetBackBufferRenderTarget();
	void ResetViewport();

	void setSkyMode(bool how);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthStencilStateSky;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateSky;
	ID3D11BlendState* m_blendStates[2];
	/*XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;*/

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	//ID3D11BlendState* m_alphaEnableBlendingState;
	//ID3D11BlendState* m_alphaDisableBlendingState;

	D3D11_VIEWPORT m_viewport;
};

#endif