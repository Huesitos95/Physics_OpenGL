#pragma once
#include <glm\gtc\type_ptr.hpp>

//Classe Singletone
class Solver
{
public:
	static Solver &getInstance()
	{
		static Solver instance;
		return instance;
	}

	Solver &operator=(Solver &s) = delete;
	~Solver();
	void EulerSemiImplicit(float);
	float *partVerts;
	float *partVelocity;
	float *partForces;

	float gravity;
		

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
	gravity = -9.8;
	partVerts = new float[LilSpheres::maxParticles * 3];
	partVelocity= new float[LilSpheres::maxParticles * 3];
	partForces = new float[LilSpheres::maxParticles * 3];
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {

		//Introducir Posicion Particulas.
		partVerts[i * 3 + 0] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
		partVerts[i * 3 + 1] = ((float)rand() / RAND_MAX) * 10.f;
		partVerts[i * 3 + 2] = ((float)rand() / RAND_MAX) * 10.f - 5.f;

		//Introducir Velocidad Particulas
		partVelocity[i * 3 + 0] = 0;
		partVelocity[i * 3 + 1] = 0;
		partVelocity[i * 3 + 2] = 0;


		//Introduir Fuerzas Particulas

		//Todo random, menos la gravedad
		partForces[i * 3 + 0] = (rand() % 30) - 15;
		partForces[i * 3 + 1] = gravity;
		partForces[i * 3 + 2] = (rand() % 30) - 15;

		//Cascada
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
		partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] + dt*partForces[i * 3 + 0];
		partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] + dt*partForces[i * 3 + 1];
		partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] + dt*partForces[i * 3 + 2];

		partForces[i * 3 + 0] = partVelocity[i * 3 + 0] * partForces[i * 3 + 0];
		partForces[i * 3 + 1] = partVelocity[i * 3 + 1] * partForces[i * 3 + 1];
		partForces[i * 3 + 2] = partVelocity[i * 3 + 2] * partForces[i * 3 + 2];

		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVelocity[i * 3 + 0];
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVelocity[i * 3 + 1];
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVelocity[i * 3 + 2];
	}
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}
