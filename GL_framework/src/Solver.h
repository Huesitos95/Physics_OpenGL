#pragma once
#include <glm\gtc\type_ptr.hpp>

//Classe Singletone
class Solver
{
public:
	static Solver getInstance()
	{
		static Solver instance;
		return instance;
	}

	Solver &operator=(Solver &s) = delete;
	~Solver();
	void EulerSemiImplicit(float);
	float *partVerts;

private:
	Solver();
	
};

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}

Solver::Solver()
{
	partVerts = new float[LilSpheres::maxParticles * 3];
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
		partVerts[i * 3 + 1] = ((float)rand() / RAND_MAX) * 10.f;
		partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
	}
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}

Solver::~Solver()
{
}

void Solver::EulerSemiImplicit(float dt)
{
	for (int i = 0; i < LilSpheres::maxParticles; i++)
	{
		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * 1;
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * 1;
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * 1;
	}
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}
