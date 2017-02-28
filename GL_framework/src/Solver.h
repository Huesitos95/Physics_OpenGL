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
	void ParticleCollision(float, float, float, int);
	Solver();
	
};

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}
namespace Box {
	extern float cubeVerts[];
}

inline void Solver::ParticleCollision(float xAnterior, float yAnterior, float zAnterior,int i)
{
	float normalPla[3];
	float calcVelocity;
	float dEquacioPla;
	float pimerTermaDistancia;
	float segonTermaDistancia;
	float distanciaPla;
	
	//PLA INFERIOR

	//normal del pla
	normalPla[0] = 0;
	normalPla[1] = 1;
	normalPla[2] = 0;

	//D de la equacio del pla
	dEquacioPla= -normalPla[0] - normalPla[1] - normalPla[2];

	//Primer terma per calcular la distancia
	pimerTermaDistancia= (normalPla[0] * xAnterior + normalPla[1] * yAnterior + normalPla[2] * zAnterior + dEquacioPla);
	
	//Segon terma per calcular la distancia
	segonTermaDistancia = (normalPla[0] * partVerts[i * 3 + 0] + normalPla[1] * partVerts[i * 3 + 1] + normalPla[2] * partVerts[i * 3 + 2] + dEquacioPla);
	
	//Calculem la distancia.
	distanciaPla = pimerTermaDistancia*segonTermaDistancia;

	//Si a passat el pla
	if (distanciaPla <= 0)
	{
		//Calculem la nova posicio del punt: pt+dt= pt'+dt -2(n * pt'+dt + d) * n
		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] - 2 * segonTermaDistancia * normalPla[0];
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] - 2 * segonTermaDistancia * normalPla[1];
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] - 2 * segonTermaDistancia * normalPla[2];

		// (n * vt'+dt + d)
		calcVelocity = (normalPla[0] * partVelocity[i * 3 + 0] + normalPla[1] * partVelocity[i * 3 + 1] + normalPla[2] * partVelocity[i * 3 + 2] + dEquacioPla);

		//Calculem la nova velocitat del punt: vt+dt= vt'+dt -2(n * vt'+dt + d) * n
		partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] - 2 * calcVelocity * normalPla[0];
		partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] - 2 * calcVelocity * normalPla[1];
		partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] - 2 * calcVelocity * normalPla[2];

	}

	////PLA SUPERIOR

	////normal del pla
	//normalPla[0] = 0;
	//normalPla[1] = 0;
	//normalPla[2] = 0;

	////D de la equacio del pla
	//dEquacioPla = -normalPla[0] - normalPla[1] - normalPla[2];

	////Primer terma per calcular la distancia
	//pimerTermaDistancia = (normalPla[0] * xAnterior + normalPla[1] * yAnterior + normalPla[2] * zAnterior + dEquacioPla);

	////Segon terma per calcular la distancia
	//segonTermaDistancia = (normalPla[0] * partVerts[i * 3 + 0] + normalPla[1] * partVerts[i * 3 + 1] + normalPla[2] * partVerts[i * 3 + 2] + dEquacioPla);

	////Calculem la distancia.
	//distanciaPla = pimerTermaDistancia*segonTermaDistancia;

	////Si a passat el pla
	//if (distanciaPla <= 0)
	//{
	//	//Calculem la nova posicio del punt: pt+dt= pt'+dt -2(n * pt'+dt + d) * n
	//	partVerts[i * 3 + 0] = partVerts[i * 3 + 0] - 2 * segonTermaDistancia * normalPla[0];
	//	partVerts[i * 3 + 1] = partVerts[i * 3 + 1] - 2 * segonTermaDistancia * normalPla[1];
	//	partVerts[i * 3 + 2] = partVerts[i * 3 + 2] - 2 * segonTermaDistancia * normalPla[2];

	//	// (n * vt'+dt + d)
	//	calcVelocity = (normalPla[0] * partVelocity[i * 3 + 0] + normalPla[1] * partVelocity[i * 3 + 1] + normalPla[2] * partVelocity[i * 3 + 2] + dEquacioPla);

	//	//Calculem la nova velocitat del punt: vt+dt= vt'+dt -2(n * vt'+dt + d) * n
	//	partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] - 2 * calcVelocity * normalPla[0];
	//	partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] - 2 * calcVelocity * normalPla[1];
	//	partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] - 2 * calcVelocity * normalPla[2];
	//}
	
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
		partVerts[i * 3 + 1] = ((float)rand() / RAND_MAX) * 10.f+1;
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
	float x, y, z;
	for (int i = 0; i < LilSpheres::maxParticles; i++)
	{
		partVelocity[i * 3 + 0] = partVelocity[i * 3 + 0] + dt*partForces[i * 3 + 0];
		partVelocity[i * 3 + 1] = partVelocity[i * 3 + 1] + dt*partForces[i * 3 + 1];
		partVelocity[i * 3 + 2] = partVelocity[i * 3 + 2] + dt*partForces[i * 3 + 2];

		partForces[i * 3 + 0] = partVelocity[i * 3 + 0] * partForces[i * 3 + 0];
		partForces[i * 3 + 1] = partVelocity[i * 3 + 1] * partForces[i * 3 + 1];
		partForces[i * 3 + 2] = partVelocity[i * 3 + 2] * partForces[i * 3 + 2];

		x = partVerts[i * 3 + 0];
		y = partVerts[i * 3 + 1];
		z = partVerts[i * 3 + 2];

		partVerts[i * 3 + 0] = partVerts[i * 3 + 0] + dt * partVelocity[i * 3 + 0];
		partVerts[i * 3 + 1] = partVerts[i * 3 + 1] + dt * partVelocity[i * 3 + 1];
		partVerts[i * 3 + 2] = partVerts[i * 3 + 2] + dt * partVelocity[i * 3 + 2];

		ParticleCollision(x, y, z, i);
	}
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, partVerts);
}
