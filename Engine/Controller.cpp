#include "Controller.h"



Controller::Controller()
{
}


Controller::~Controller()
{
}

void Controller::StrafeLeft()
{
	//Strafe(-m_movSpeed);
}
void Controller::StrafeRight()
{
	//Strafe(m_movSpeed);
}
void Controller::MoveForward()
{
	//Advance(m_movSpeed);
}
void Controller::MoveBack()
{
	//Advance(-1.0f * m_movSpeed);
}

/*void Controller::Strafe(float amount)
{
	m_camPos = m_Camera->Strafe(amount);
}

void Controller::Advance(float amount)
{
	m_camPos = m_Camera->Advance(amount);
}

void Controller::Rotate(XMFLOAT3 rot)
{
	m_Camera->Rotate(rot);
}*/
