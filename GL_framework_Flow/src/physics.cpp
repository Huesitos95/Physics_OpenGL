#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "Cloth.h"
#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <string>

bool show_test_window = false;

namespace ClothMesh
{
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);
}

namespace Sphere
{
	extern void updateSphere(glm::vec3 pos, float radius);
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
	float time;
	float dampingStretch = 0.5f;
	float dampingShear = 0.5f;
	float dampingBend = 0.5f;
	float longitud = 10;
	float initalDistancePoints = 0.5;
	glm::vec3 centerSphere;
	float r = 1.0f;

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
	void ResetValues()
	{
		list.clear();
		float x = -5.5f;
		float z = -5.5f;
		float y = 4.0f;

		for (int j = 0; j < ClothMesh::numRows; j++)
		{
			z += initalDistancePoints;
			x = -5.5f;
			for (int i = 0; i < ClothMesh::numCols; i++)
			{
				x += initalDistancePoints;
				Particle p(x, y, z);
				p.xV = 0;
				p.yV = 0;
				p.zV = 0;
				p.xF = 0;
				p.yF = -9.8f;
				p.zF = 0;
				list.push_back(p);
			}
		}
		r = (rand() % 1) + 1;
		centerSphere.x = (rand() % 2) - 1;
		centerSphere.y = (rand() % 2)+2;
		centerSphere.z = (rand() % 2) - 1;
		Sphere::updateSphere(centerSphere, r);

		ClothMesh::updateClothMesh(ParticlesToFloatPointer());
	}

	bool DistancePointSphere(int i)
	{
		glm::vec3 v (list[i].x - centerSphere.x, list[i].y - centerSphere.y, list[i].z - centerSphere.z);
		float d = glm::pow(v.x, 2) + glm::pow(v.y, 2) + glm::pow(v.z, 2);
		d = glm::sqrt(d);

		float x = list[i].x - centerSphere.x;
		float y = list[i].y - centerSphere.y;
		float z = list[i].z - centerSphere.z;

		return d-r<= 0;
	}

	//Extret de la practica de rebot de les particules
	void Update(float dt)
	{
		time += dt;
		if (time >= 20)
		{
			ResetValues();
			time = 0;
		}
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

		//ESFERA	
		if (DistancePointSphere(i))
		{
			glm::vec3 vectorDirector(list[i].x - centerSphere.x, list[i].y - centerSphere.y, list[i].z - centerSphere.z);
			vectorDirector = glm::normalize(vectorDirector);
			glm::vec3 point = centerSphere + (vectorDirector*r);
			
			list[i].x = xAnterior;
			list[i].y = yAnterior;
			list[i].z = zAnterior;
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
		ImGui::Text("Using SemiImplicit Euler System");
		ImGui::DragFloat("Stretch", &particles.dampingStretch, 0.1f,0, 30, "%.3f", 1.f);
		ImGui::DragFloat("Shear", &particles.dampingShear, 0.1f, 0, 30, "%.3f", 1.f);
		ImGui::DragFloat("Bend", &particles.dampingBend, 0.1f, 0, 30, "%.3f", 1.f);
		ImGui::DragFloat("Enllongation", &particles.longitud, 0.1f, 0, 30, "%.3f", 1.f);
		ImGui::DragFloat("Initial Rest Distance", &particles.initalDistancePoints, 0.1f, 0.1, 30, "%.3f", 1.f);
		ImGui::Text("Time %.1f", particles.time);
		if (ImGui::Button("Reset", ImVec2(50, 20)))
		{
			particles.ResetValues();
		}
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	//TODO
	particles.ResetValues();
}
void PhysicsUpdate(float dt) {
	//TODO
	//particles.Update(dt);
	ClothMesh::updateClothMesh(particles.ParticlesToFloatPointer());
}
void PhysicsCleanup() {
	//TODO
}