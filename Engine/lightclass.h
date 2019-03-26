////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
//#include <d3dx10math.h>
#include <DirectXMath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColour(float, float, float, float);
	void SetAmbientColour(float, float, float, float);
	void SetSpecularColour(float, float, float, float);
	void SetSpecularPower(float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColour();
	XMFLOAT4 GetAmbientColour();
	XMFLOAT4 GetSpecularColour();
	float GetSpecularPower();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 m_diffuseColour;
	XMFLOAT4 m_ambientColour;
	XMFLOAT4 m_specularColour;
	float m_specularPower;
	XMFLOAT3 m_direction;
};

#endif