#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "Cloth.h"
#include <vector>

bool show_test_window = false;

namespace ClothMesh
{
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);
}

struct Particle
{
	float x, y, z, xV, yV, zV, xF, yF, zF;
	Particle() {}
	Particle(float i, float j, float k) { x = i; y = j; z = k; }
};

struct ParticlesList
{
	std::vector<Particle> list;
	float elastic = 0.5f;
	float* ParticlesToFloatPointer()
	{
		float* v= new float[list.size()*3];
		for (int i = 0; i < list.size();i++)
		{
			v[i * 3 + 0] = list[i].x;
			v[i * 3 + 1] = list[i].y;
			v[i * 3 + 2] = list[i].z;
		}
		return v;
	}

	//Extret de la practica de rebot de les particules
	void Update(float dt)
	{
		float x, y, z;
		for (int i = 1; i < list.size(); i++)
		{
			//S'ha de repassar aquesta part.
			if (i + 1 != ClothMesh::numCols)
			{
				//Velocitat
				list[i].xV = list[i].xV + dt*list[i].xF;
				list[i].yV = list[i].yV + dt*list[i].yF;
				list[i].zV = list[i].zV + dt*list[i].zF;

				//Forces
				list[i].xF = list[i].xV * list[i].xF;
				list[i].yF = list[i].yV * list[i].yF;
				list[i].zF = list[i].zV * list[i].zF;

				//Posicions
				x = list[i].x;
				y = list[i].y;
				z = list[i].z;

				list[i].x = list[i].x + dt * list[i].xV;
				list[i].y = list[i].y + dt * list[i].yV;
				list[i].z = list[i].z + dt * list[i].zV;

				ParticleCollision(x, y, z, i);
			}
		}
	}
	bool DistancePointToPlane(float xAnterior, float yAnterior, float zAnterior, int i, float * normalPla, float dEquacioPla)
	{

		float pimerTermaDistancia;
		float segonTermaDistancia;
		float distanciaPla;
		//Primer terma per calcular la distancia
		pimerTermaDistancia = (normalPla[0] * xAnterior + normalPla[1] * yAnterior + normalPla[2] * zAnterior + dEquacioPla);

		//Segon terma per calcular la distancia
		segonTermaDistancia = (normalPla[0] * list[i].x + normalPla[1] * list[i].y + normalPla[2] * list[i].z + dEquacioPla);

		//Calculem la distancia.
		distanciaPla = pimerTermaDistancia*segonTermaDistancia;

		return distanciaPla <= 0;
	}
	void ParticleCollision(float xAnterior, float yAnterior, float zAnterior, int i)
	{
		float normalPla[3];
		float dEquacioPla;

		//PLA INFERIOR
		//normal del pla
		normalPla[0] = 0;
		normalPla[1] = 1;
		normalPla[2] = 0;

		//D de la equacio del pla
		dEquacioPla = (-normalPla[0] * -5) - (normalPla[1] * 0) - (normalPla[2] * 5);

		if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
		{
			CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		}

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
		}



		////PLA DARRERA
		//normal del pla
		normalPla[0] = 0;
		normalPla[1] = 0;
		normalPla[2] = 1;

		//D de la equacio del pla
		dEquacioPla = (-normalPla[0] * 5) - (normalPla[1] * 0) - (normalPla[2] * -5);

		if (DistancePointToPlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla))
		{
			CollisionParticlePlane(xAnterior, yAnterior, zAnterior, i, normalPla, dEquacioPla);
		}


	}
	void CollisionParticlePlane(float xAnterior, float yAnterior, float zAnterior, int i, float *normalPla, float dEquacioPla)
	{
		float calcVelocity;
		float pimerTermaDistancia;
		float segonTermaDistancia;
		float distanciaPla;

		segonTermaDistancia = (normalPla[0] * list[i].x + normalPla[1] * list[i].y + normalPla[2] * list[i].z + dEquacioPla);

		//Calculem la nova posicio del punt: pt+dt= pt'+dt -2(n * pt'+dt + d) * n
		list[i].x = list[i].x - (1 + elastic) * segonTermaDistancia * normalPla[0];
		list[i].y = list[i].y - (1 + elastic) * segonTermaDistancia * normalPla[0];
		list[i].z = list[i].z - (1 + elastic) * segonTermaDistancia * normalPla[0];

		// (n * vt'+dt)
		calcVelocity = (normalPla[0] * list[i].xV+ normalPla[1] * list[i].yV + normalPla[2] * list[i].zV);

		//Calculem la nova velocitat del punt: vt+dt= vt'+dt -2(n * vt'+dt + d) * n
		list[i].xV = list[i].xV - (1 + elastic) * calcVelocity * normalPla[0];
		list[i].yV = list[i].yV - (1 + elastic) * calcVelocity * normalPla[1];
		list[i].zV = list[i].zV - (1 + elastic) * calcVelocity * normalPla[2];
	}
};


ParticlesList particles;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	//TODO
	float x = -4.5f;
	float z = -3.5f;
	float y = 6.0f;
	
	for (int j = 0; j < ClothMesh::numRows; j++)
	{
		z += 0.5f;
		x = -4.5f;
		for (int i = 0; i < ClothMesh::numCols; i++)
		{
			x += 0.5;
			Particle p(x, y, z);
			p.xV = 0;
			p.yV = 0;
			p.zV = 0;
			p.xF = 0;
			p.yF = -9.8f;
			p.zF = 0;
			particles.list.push_back(p);
		}
	}	
	ClothMesh::updateClothMesh(particles.ParticlesToFloatPointer());
}
void PhysicsUpdate(float dt) {
	//TODO
	particles.Update(dt);
	ClothMesh::updateClothMesh(particles.ParticlesToFloatPointer());
}
void PhysicsCleanup() {
	//TODO
}