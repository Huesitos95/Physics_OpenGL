#pragma once
#include <iostream>
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
	void Verlet(float);
	float *partVerts;
	float *partVertsAnterior;
	float *partVelocity;
	float *partForces;
	float *partTime;
	float elastic;

	float gravity;

	
		

private:
	void ParticleCollision(float, float, float, int);
	void CollisionParticlePlane(float, float, float, int, float*, float);
	void RestartParticle(int);
	bool DistancePointToPlane(float xAnterior, float yAnterior, float zAnterior, int i, float *normalPla, float dEquacioPla);
	Solver();
	
};

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}

inline void Solver::ParticleCollision(float xAnterior, float yAnterior, float zAnterior,int i)
{
	float normalPla[3];
	float dEquacioPla;
	
	//PLA INFERIOR
	//normal del pla
	normalPla[0] = 0;
	normalPla[1] = 1;
	normalPla[2] = 0;

	//D de la equacio del pla
	dEquacioPla= (-normalPla[0]*-5) - (normalPla[1]*0) - (normalPla[2]*5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 1] *= -1;
	}

	partForces[i * 3 + 1] = -partForces[i * 3 + 1];

	////PLA SUPERIOR
	//normal del pla
	normalPla[0] = 0;
	normalPla[1] = -1;
	normalPla[2] = 0;

	//D de la equacio del pla
	dEquacioPla = (-normalPla[0] * -5) - (normalPla[1] * 10) - (normalPla[2] * -5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 1] *= -1;
	}

	////PLA ESQUERRA
	//normal del pla
	normalPla[0] = 1;
	normalPla[1] = 0;
	normalPla[2] = 0;

	//D de la equacio del pla
	dEquacioPla = (-normalPla[0] * -5) - (normalPla[1] * 0) - (normalPla[2] * 5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 0] *= -1;
	}


	////PLA DRETA
	//normal del pla
	normalPla[0] = -1;
	normalPla[1] = 0;
	normalPla[2] = 0;

	//D de la equacio del pla
	dEquacioPla = (-normalPla[0] * 5) - (normalPla[1] * 0) - (normalPla[2] * -5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 0] *= -1;
	}



	////PLA DAVANT
	//normal del pla
	normalPla[0] = 0;
	normalPla[1] = 0;
	normalPla[2] = -1;

	//D de la equacio del pla
	dEquacioPla = (-normalPla[0] * 5) - (normalPla[1] * 0) - (normalPla[2] * 5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 2] *= -1;
	}



	////PLA DAVANT
	//normal del pla
	normalPla[0] = 0;
	normalPla[1] = 0;
	normalPla[2] = 1;

	//D de la equacio del pla
	dEquacioPla = (-normalPla[0] * 5) - (normalPla[1] * 0) - (normalPla[2] * -5);

	if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
	{
		CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		partForces[i * 3 + 2] *= -1;
	}

	
}

inline void Solver::CollisionParticlePlane(float xAnterior, float yAnterior, float zAnterior, int i, float *normalPla, float dEquacioPla)
{
	float calcVelocity;
	float pimerTermaDistancia;
	float segonTermaDistancia;
	float distanciaPla;

	segonTermaDistancia = (normalPla[0] * partVerts[i * 3 + 0] + normalPla[1] * partVerts[i * 3 + 1] + normalPla[2] * partVerts[i * 3 + 2] + dEquacioPla);

		//Calculem la nova posicio del punt: pt+dt= pt'+dt -2(n * pt'+dt + d) * n
		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] - (1 + elastic) * segonTermaDistancia * normalPla[0];
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] - (1 + elastic) * segonTermaDistancia * normalPla[1];
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] - (1 + elastic) * segonTermaDistancia * normalPla[2];

		// (n * vt'+dt + d)
		calcVelocity = (normalPla[0] * partVelocity[i * 3 + 0] + normalPla[1] * partVelocity[i * 3 + 1] + normalPla[2] * partVelocity[i * 3 + 2] + dEquacioPla);

		//Calculem la nova velocitat del punt: vt+dt= vt'+dt -2(n * vt'+dt + d) * n
		partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] - (1 + elastic) * calcVelocity * normalPla[0];
		partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] - (1 + elastic) * calcVelocity * normalPla[1];
		partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] - (1 + elastic) * calcVelocity * normalPla[2];
}

inline void Solver::RestartParticle(int i)
{
	//Introducir Posicion Particulas.
	partVerts[i * 3 + 0] = 1;//((float)rand() / RAND_MAX) * 10.f - 5.0f;
	partVerts[i * 3 + 1] = 4;//((float)rand() / RAND_MAX) * 10.f;
	partVerts[i * 3 + 2] = 1;//((float)rand() / RAND_MAX) * 10.f - 5.0f;
	
	//Introduir Posicions Particules anteriors.
	partVertsAnterior[i * 3 + 0]=partVerts[i * 3 + 0];
	partVertsAnterior[i * 3 + 1]=partVerts[i * 3 + 1];
	partVertsAnterior[i * 3 + 2]=partVerts[i * 3 + 2];

	//Introducir Velocidad Particulas
	partVelocity[i * 3 + 0] = 0;
	partVelocity[i * 3 + 1] = 0;
	partVelocity[i * 3 + 2] = 0;


	//Introduir Fuerzas Particulas

	//Todo random, menos la gravedad
	partForces[i * 3 + 0] = 0;
	partForces[i * 3 + 1] = rand()*2-1;
	partForces[i * 3 + 2] = 0;

	//Vida Particula
	partTime[i] = ((float)rand()*5);
}

inline bool Solver::DistancePointToPlane(float xAnterior, float yAnterior, float zAnterior, int i, float * normalPla, float dEquacioPla)
{

	float pimerTermaDistancia;
	float segonTermaDistancia;
	float distanciaPla;
	//Primer terma per calcular la distancia
	pimerTermaDistancia = (normalPla[0] * xAnterior + normalPla[1] * yAnterior + normalPla[2] * zAnterior + dEquacioPla);

	//Segon terma per calcular la distancia
	segonTermaDistancia = (normalPla[0] * partVerts[i * 3 + 0] + normalPla[1] * partVerts[i * 3 + 1] + normalPla[2] * partVerts[i * 3 + 2] + dEquacioPla);

	//Calculem la distancia.
	distanciaPla = pimerTermaDistancia*segonTermaDistancia;

	return distanciaPla <= 0;
}

Solver::Solver()
{
	gravity = -9.8;
	partVerts = new float[LilSpheres::maxParticles * 3];
	partVertsAnterior = new float[LilSpheres::maxParticles * 3];
	partVelocity= new float[LilSpheres::maxParticles * 3];
	partForces = new float[LilSpheres::maxParticles * 3];
	partTime = new float[LilSpheres::maxParticles];

	elastic = 0.5;

	for (int i = 0; i < LilSpheres::maxParticles; ++i)
	{
		RestartParticle(i);
	}

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}

Solver::~Solver()
{
}

void Solver::EulerSemiImplicit(float dt)
{
	float x, y, z;
	for (int i = 0; i < LilSpheres::maxParticles; i++)
	{
		partTime[i] += dt;

		if (partTime[i] < 5)
		{
			//Velocitat
			partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] + dt*partForces[i * 3 + 0];
			partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] + dt*partForces[i * 3 + 1];
			partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] + dt*partForces[i * 3 + 2];

			//Forces
			partForces[i * 3 + 0] = partVelocity[i * 3 + 0] * partForces[i * 3 + 0];
			partForces[i * 3 + 1] = partVelocity[i * 3 + 1] * partForces[i * 3 + 1];// +gravity;
			partForces[i * 3 + 2] = partVelocity[i * 3 + 2] * partForces[i * 3 + 2];

			//std::cout << partForces[0 * 3 + 1] << std::endl;


			//Posicions
			x = partVerts[i * 3 + 0];
			y = partVerts[i * 3 + 1];
			z = partVerts[i * 3 + 2];

			partVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVelocity[i * 3 + 0];
			partVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVelocity[i * 3 + 1];
			partVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVelocity[i * 3 + 2];

			ParticleCollision(x, y, z, i);
		}
		else
		{
			RestartParticle(i);
		}		
	}

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}

inline void Solver::Verlet(float dt)
{
	float x, y, z;
	for (int i = 0; i < LilSpheres::maxParticles; i++)
	{
		partTime[i] += dt;

		if (partTime[i] < 5)
		{
		//Forces
		partForces[i * 3 + 0] = partForces[i * 3 + 0] * glm::pow(dt,2);
		partForces[i * 3 + 1] = glm::pow(dt, 2) * partForces[i * 3 + 1];// +gravity;
		partForces[i * 3 + 2] = glm::pow(dt, 2) * partForces[i * 3 + 2];


		//Posicions
		x = partVerts[i * 3 + 0];
		y = partVerts[i * 3 + 1];
		z = partVerts[i * 3 + 2];

		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] +(partVerts[i * 3 + 0]- partVertsAnterior[i * 3 + 0])+ partForces[i * 3 + 0];
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] + (partVerts[i * 3 + 1] - partVertsAnterior[i * 3 + 1]) + partForces[i * 3 + 1];
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] + (partVerts[i * 3 + 2] - partVertsAnterior[i * 3 + 2]) + partForces[i * 3 + 2];

		ParticleCollision(x, y, z, i);

		partVertsAnterior[i * 3 + 0] = x;
		partVertsAnterior[i * 3 + 1] = y;
		partVertsAnterior[i * 3 + 2] = z;
		}
		else
		{
			RestartParticle(i);
		}
	}

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
	
}
