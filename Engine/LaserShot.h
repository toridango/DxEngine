#pragma once

#include <chrono>

#include "GameObject.h"

// in seconds
const float LASER_LIFESPAN = 5.0f;
// in units/second
const float LASER_SPEED = 50.0f;
// in seconds
const float LASER_COOLDOWN = 0.5f;


class LaserShot :
	public GameObject
{
public:
	LaserShot(XMFLOAT3 m_direction);
	~LaserShot();

	static float cooldown;
	static void UpdateCooldown(float deltaTime);
	static bool IsOnCooldown();
	static void ResetCooldown();

	void Update(float deltaTime);
	bool HasExpired();
	XMFLOAT3 GetDirectionFloat3();
	XMVECTOR GetDirectionVector();


private:


	std::chrono::high_resolution_clock::time_point m_birthTime;
	
	XMFLOAT3 m_direction;
	
};

