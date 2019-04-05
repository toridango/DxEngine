#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
//#include <d3d11.h>
//#include <string>


// From https://github.com/sol-prog/Perlin_Noise
// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

#ifndef IMPROVEDNOISE_H
#define IMPROVEDNOISE_H

class ImprovedNoise
{
	// The permutation vector
	std::vector<int> p;


public:
	ImprovedNoise(float min = 0.0f, float max = 1.0f);

	// Initialize with the reference values for the permutation vector
	void InitialisePerlin();
	// Generate a new permutation vector based on the value of seed
	void InitialisePerlin(unsigned int seed);

	void SetRealDistributionRange(float min, float max);
	void SetIntDistributionRange(unsigned int min, unsigned int max);
	float RollFloat();
	unsigned int RollUInt();

	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);

private:

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

private:

	// Been initialised flag
	bool m_perlinInit;
	// Random number generator (based on stochastic processes to generate sequence of uniformily distributed random numbers)
	static std::random_device m_randomDevice;
	// Mersenne Twister pseudo-random generator of 64-bit numbers with a state size of 19937 bits
	static std::mt19937_64 m_mtGenerator;

	std::uniform_int_distribution<unsigned int> m_UIntDistribution;
	std::uniform_real_distribution<float> m_floatDistribution;

};

#endif