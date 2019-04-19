#pragma once

#include <DirectXMath.h>

using namespace DirectX;


class Controller
{
public:
	enum KEYBINDS
	{
		W_KEY,
		A_KEY,
		S_KEY,
		D_KEY,
		LSHIFT_KEY,
		SPACE_KEY
	};
public:
	Controller();
	~Controller();

	//KeyPressed(KEYBINDS id);

	void SetControlled();


	void StrafeLeft();
	void StrafeRight();
	void MoveForward();
	void MoveBack();

	/*void Strafe(float amount);
	void Advance(float amount);

	void Rotate(XMFLOAT3 rot);
	*/

private:

	float m_movSpeed;
};

