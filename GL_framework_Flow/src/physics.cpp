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
	glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);

	float time,t,w,A,kItalic,density,gravity,vSub;
	float initalDistancePoints = 0.75;

	//Sphere
	glm::vec3 centerSphere;
	glm::vec3 sphereVelocity;
	glm::vec3 sphereForce;
	float r = 1.0f;
	float m = 20.0f;

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

		//Gravetat
		gravity = -9.8f;

		//DensitatFluit
		density = 15.0f;

		//Volum Desplasat
		vSub = 0.0f;

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
		centerSphere.y = (rand() % 3)+2;
		centerSphere.z = (rand() % 2) - 1;
		Sphere::updateSphere(centerSphere, r);
		sphereForce.y = -9.8f;
		
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
		else
		{
			//Sumatori de la simulacio
			t += dt;
			float yPoints=0.0f;
			int countPoints=0;
			glm::vec3 bouyancy;

			for (int i = 0; i < list.size(); i++)
			{
				glm::vec3 v = (kBlood / kItalic)*A*sin(dot(kBlood, initPos[i]) - w*t);
				list[i].x = initPos[i].x - v.x;
				list[i].z = initPos[i].z - v.z;

				list[i].y = (glm::vec3(A*cos(dot(kBlood, initPos[i]) - w*t))).y;
				if (DistancePointToPoint(list[i]))
				{
					yPoints += list[i].y;
					countPoints++;
				}
			}

			//SPHERE
			//Moure esfera Euler SemiImplicit
			if (countPoints > 0)
			{
				//centerSphere.y = (yPoints / countPoints);
				float d = (yPoints / countPoints) - (centerSphere.y - r);
				vSub = d*r*r;
				bouyancy = density*-gravity*vSub*y;
				
			}
			sphereForce = bouyancy + glm::vec3(0.0f, gravity, 0.0f)*m;
			sphereVelocity += dt * sphereForce/m;
			centerSphere = centerSphere + dt * sphereVelocity;

			//Actualitzar
			Sphere::updateSphere(centerSphere, r);
		}
	}
	bool DistancePointToPoint(glm::vec3 point)
	{
		float distanciaPla = (centerSphere.y - r) - point.y;
		return distanciaPla <= 0;
	}
};


ParticlesList particles;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		//TODO
		ImGui::Text("Using SemiImplicit Euler System");
		ImGui::Text("Time %.1f", particles.time);
		ImGui::DragFloat("Frequence", &particles.w, 0.1f, 1.0f, 10.0f, "%.1f");
		ImGui::DragFloat("Density", &particles.density, 0.1f, 15.0f, 20.0f, "%.1f");
		ImGui::DragFloat("Mass", &particles.m, 0.1f, 20.0f, 30.0f, "%.1f");
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