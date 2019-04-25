#include "LaserShot.h"



float LaserShot::cooldown = LASER_COOLDOWN;

/*
// Offsets for 5, 9 settings
XMFLOAT2 LaserShot::offsets[] =	{
								{ 4.6f,  2.3f },
								{ -4.6f,  2.3f },
								{  4.6f,  1.1f },
								{ -4.6f,  1.1f } 
};
*/

// Offsets for 0.85, 1.55
/*
XMFLOAT2 LaserShot::offsets[] = {
								{  0.85f,  0.48f },
								{ -0.85f,  0.48f },
								{  0.85f,  0.18f },
								{ -0.85f,  0.18f }
};
*/

LaserShot::LaserShot(XMFLOAT3 direction) :
	m_direction(direction)
{
	m_birthTime = std::chrono::high_resolution_clock::now();

	/*
	float hOff = 4.6f;
	float vOff = 2.3f;
	m_offsets[0] = { hOff,  vOff };
	m_offsets[1] = { -hOff,  0.0f };
	m_offsets[2] = { -hOff,  vOff };
	m_offsets[3] = { hOff,  0.0f };
	*/
}


LaserShot::~LaserShot()
{
}


void LaserShot::UpdateCooldown(float deltaTime)
{
	if (cooldown > 0.0f)
		cooldown -= (deltaTime / 1000.0f);
}

bool LaserShot::IsOnCooldown()
{
	return cooldown >= 0.0f;
}

void LaserShot::ResetCooldown()
{
	cooldown = LASER_COOLDOWN;
}


void LaserShot::Update(float deltaTime)
{

	XMVECTOR trans = this->GameObject::GetWorldMatrix().r[3] + 
					LASER_SPEED * (deltaTime/1000.0) * XMLoadFloat3(&m_direction);

	this->GameObject::SetTranslation(trans);

}

bool LaserShot::HasExpired()
{
	auto elapsed = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - m_birthTime);
	// From millis to seconds
	float elapsedSeconds = (elapsed.count() / 1000.0f);
	return elapsedSeconds > LASER_LIFESPAN;
}

XMFLOAT3 LaserShot::GetDirectionFloat3()
{
	return m_direction;
}

XMVECTOR LaserShot::GetDirectionVector()
{
	return XMLoadFloat3(&m_direction);
}


/*XMFLOAT2* LaserShot::GetOffsets()
{
	return offsets;
}*/