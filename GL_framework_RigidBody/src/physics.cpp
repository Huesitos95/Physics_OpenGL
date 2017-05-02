#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <string>
#include <glm\gtc\matrix_transform.hpp>

bool show_test_window = false;



namespace Sphere
{
	extern void updateSphere(glm::vec3 pos, float radius);
}

namespace Cube
{
	extern void updateCube(glm::mat4 model);
}

struct CubeRender
{
	std::vector<glm::vec3> cubVerts;
	float size = 0.5;
	float maxTime = 20.0f;
	float currenTime = 0.0f;
	bool numVertexCollision;

	glm::vec3 center;
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 previousPosition;
	CubeRender()
	{
		Restart();
	}

	float* CubeRenderToFloatPointer()
	{
		float*r = new float[24];
		int i = 0;
		for (glm::vec3 v : cubVerts)
		{
			r[0 + i * 3] = v.x;
			r[1 + i * 3] = v.y;
			r[2 + i * 3] = v.z;
			i++;
		}
		return r;
	}
	glm::vec3& CenterPostion(float size)
	{
		glm::vec3 centerPoint;

		centerPoint.x = (rand() % 10) - 5;
		if (centerPoint.x - size < -5)centerPoint.x + size;
		else if (centerPoint.x + size > 5)centerPoint.x - size;

		centerPoint.y = (rand() % 10);
		if (centerPoint.y - size < 0)centerPoint.y + size;
		else if (centerPoint.y + size > 10)centerPoint.y - size;

		centerPoint.z = (rand() % 10) - 5;
		if (centerPoint.z - size < -5)centerPoint.z + size;
		else if (centerPoint.z + size > 5)centerPoint.z - size;


		return centerPoint;
	}
	void Restart()
	{
		cubVerts.clear();
		numVertexCollision = false;
		currenTime = 0.0f;
		center =  CenterPostion(size);
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		force = glm::vec3(0, -9.8f, 0.0f);
		//Front
		glm::vec3 v = glm::vec3(0.0f - size, 0.0f - size, 0.0f + size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f + size, 0.0f - size, 0.0f + size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f + size, 0.0f + size, 0.0f + size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f - size, 0.0f + size, 0.0f + size);
		cubVerts.push_back(v);

		//Back
		v = glm::vec3(0.0f - size, 0.0f - size, 0.0f - size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f + size, 0.0f - size, 0.0f - size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f + size, 0.0f + size, 0.0f - size);
		cubVerts.push_back(v);

		v = glm::vec3(0.0f - size, 0.0f + size, 0.0f - size);
		cubVerts.push_back(v);
	}
	void CalculateNewVertexPosition()
	{
		glm::vec3 pointPrevious;

		//model = glm::translate(model, center);
		/*if (!numVertexCollision)
		{
			pointPrevious = cubVerts[0];
			cubVerts[0] = glm::vec3(center.x - size, center.y - size, center.z + size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 0);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[1];
			cubVerts[1] = glm::vec3(center.x + size, center.y - size, center.z + size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 1);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[2];
			cubVerts[2] = glm::vec3(center.x + size, center.y + size, center.z + size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 2);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[3];
			cubVerts[3] = glm::vec3(center.x - size, center.y + size, center.z + size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 3);
		}
		

		//Back
		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[4];
			cubVerts[4] = glm::vec3(center.x - size, center.y - size, center.z - size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 4);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[5];
			cubVerts[5] = glm::vec3(center.x + size, center.y - size, center.z - size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 5);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[6];
			cubVerts[6] = glm::vec3(center.x + size, center.y + size, center.z - size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 6);
		}
		

		if (!numVertexCollision)
		{
			pointPrevious = cubVerts[7];
			cubVerts[7] = glm::vec3(center.x - size, center.y + size, center.z - size);
			ParticleCollision(pointPrevious.x, pointPrevious.y, pointPrevious.z, 7);
		}*/
		
	}

	void Update(float dt)
	{
		glm::mat4 model;

		currenTime += dt;
		if (currenTime >= maxTime)
		{
			Restart();
		}
		else
		{
			velocity = velocity + dt*force;
			//force = velocity*force;
			
			//previousPosition = center;

			center += dt * velocity;
			model = glm::translate(model, center);
		}
		Cube::updateCube(model);
	}

	bool DistancePointToPlane(float xAnterior, float yAnterior, float zAnterior, int i, float * normalPla, float dEquacioPla)
	{

		float pimerTermaDistancia;
		float segonTermaDistancia;
		float distanciaPla;
		//Primer terma per calcular la distancia
		pimerTermaDistancia = (normalPla[0] * xAnterior + normalPla[1] * yAnterior + normalPla[2] * zAnterior + dEquacioPla);

		//Segon terma per calcular la distancia
		segonTermaDistancia = (normalPla[0] * cubVerts[i].x + normalPla[1] * cubVerts[i].y + normalPla[2] * cubVerts[i].z + dEquacioPla);

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
		cubVerts[i].x = xAnterior;
		cubVerts[i].y = yAnterior;
		cubVerts[i].z = zAnterior;
		numVertexCollision=true;

		/*float calcVelocity;
		float pimerTermaDistancia;
		float segonTermaDistancia;
		float distanciaPla;

		segonTermaDistancia = (normalPla[0] * cubVerts[i].x + normalPla[1] * cubVerts[i].y + normalPla[2] * cubVerts[i].z + dEquacioPla);

		//Calculem la nova posicio del punt: pt+dt= pt'+dt -2(n * pt'+dt + d) * n
		cubVerts[i].x = cubVerts[i].x - (1 + elastic) * segonTermaDistancia * normalPla[0];
		cubVerts[i].y = cubVerts[i].y - (1 + elastic) * segonTermaDistancia * normalPla[0];
		cubVerts[i].z = cubVerts[i].z - (1 + elastic) * segonTermaDistancia * normalPla[0];

		// (n * vt'+dt)
		calcVelocity = (normalPla[0] * list[i].xV + normalPla[1] * list[i].yV + normalPla[2] * list[i].zV);

		//Calculem la nova velocitat del punt: vt+dt= vt'+dt -2(n * vt'+dt + d) * n
		list[i].xV = list[i].xV - (1 + elastic) * calcVelocity * normalPla[0];
		list[i].yV = list[i].yV - (1 + elastic) * calcVelocity * normalPla[1];
		list[i].zV = list[i].zV - (1 + elastic) * calcVelocity * normalPla[2];*/
	}
};

CubeRender c;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Time Life: %.3f", c.currenTime);
		
		//TODO
		if (ImGui::Button("Restart", ImVec2(150.0f, 50.0f)))
		{
			c.Restart();
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
	
}
void PhysicsUpdate(float dt) {
	//TODO
	//Cube::updateCube(c.CubeRenderToFloatPointer());
	c.Update(dt);
}
void PhysicsCleanup() {
	//TODO
}