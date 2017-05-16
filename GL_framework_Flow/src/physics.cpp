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

struct ParticlesList
{
	//Mesh
	std::vector<glm::vec3> list;
	std::vector<glm::vec3> initPos;
	glm::vec3 kBlood = glm::vec3(1.0f,0.0f,1.0f);

	float time,t,w,A,kItalic;
	float initalDistancePoints = 0.75;

	//Sphere
	glm::vec3 centerSphere;
	glm::vec3 sphereVelocity;
	glm::vec3 sphereForce;
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
		//Sumatori del temps de simulacio
		t = 0.0f;

		//Freqüencia. Mantenim els valors de la W
		if(w<1.0f)w = 5.0f;

		//Amplitud de la onada
		A = 0.2f;

		//La longitud del vector
		kItalic = kBlood.length();

		//Mesh
		list.clear();
		initPos.clear();
		float x;
		float z = -5.5f;
		float y = 4.0f;

		for (int j = 0; j < ClothMesh::numRows; j++)
		{
			z += initalDistancePoints;
			x = -5.5f;
			for (int i = 0; i < ClothMesh::numCols; i++)
			{
				x += initalDistancePoints;
				glm::vec3 p(x, y, z);
				list.push_back(p);
				initPos.push_back(p);
			}
		}
		ClothMesh::updateClothMesh(ParticlesToFloatPointer());

		//Sphere
		r = (rand() % 1) + 1;
		centerSphere.x = (rand() % 2) - 1;
		centerSphere.y = (rand() % 4)+2;
		centerSphere.z = (rand() % 2) - 1;
		Sphere::updateSphere(centerSphere, r);
		
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
		//Temps de simulacio
		time += dt;
		if (time >= 20)
		{
			ResetValues();
			time = 0;
		}
		//Sumatori de la simulacio
		t += dt;
		
		for (int i = 0; i < list.size(); i++)
		{
			glm::vec3 v = (kBlood / kItalic)*A*sin(dot(kBlood, initPos[i]) - w*t);
			list[i].x = initPos[i].x - v.x;
			list[i].z = initPos[i].z - v.z;

			list[i].y = (glm::vec3(A*cos(dot(kBlood, initPos[i]) - w*t))).y;
		}

		//float x, y, z;
		////Velocitat
		//list[i].xV = list[i].xV + dt*list[i].xF;
		//list[i].yV = list[i].yV + dt*list[i].yF;
		//list[i].zV = list[i].zV + dt*list[i].zF;

		////Forces
		//list[i].xF = list[i].xV * list[i].xF;
		//list[i].yF = list[i].yV * list[i].yF;
		//list[i].zF = list[i].zV * list[i].zF;

		////Posicions
		//x = list[i].x;
		//y = list[i].y;
		//z = list[i].z;

		//list[i].x = list[i].x + dt * list[i].xV;
		//list[i].y = list[i].y + dt * list[i].yV;
		//list[i].z = list[i].z + dt * list[i].zV;

		////ParticleCollision(x, y, z, i);
	}
	/*bool DistancePointToPlane(float xAnterior, float yAnterior, float zAnterior, int i, float * normalPla, float dEquacioPla)
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
	}*/

};


ParticlesList particles;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		//TODO
		ImGui::Text("Using SemiImplicit Euler System");
		ImGui::Text("Time %.1f", particles.time);
		ImGui::DragFloat("Frequence", &particles.w, 0.1f, 1.0f, 10.0f, "%.1f");
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
	particles.Update(dt);
	ClothMesh::updateClothMesh(particles.ParticlesToFloatPointer());
}
void PhysicsCleanup() {
	//TODO
}