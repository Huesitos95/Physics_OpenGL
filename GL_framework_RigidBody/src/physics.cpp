#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <string>

bool show_test_window = false;

struct CubeRender
{
	std::vector<glm::vec3> cubVerts;
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
		}
		return r;
	}
	glm::vec3& CenterPostion(float size)
	{
		glm::vec3 centerPoint;

		centerPoint.x = (rand()%10)-5;
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
		float size = 0.5;
		glm::vec3 center = CenterPostion(size);
		//Front
		glm::vec3 v = glm::vec3(center.x - size, center.y - size, center.z + size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x + size, center.y - size, center.z + size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x + size, center.y + size, center.z + size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x - size, center.y + size, center.z + size);
		cubVerts.push_back(v);

		//Back
		v = glm::vec3(center.x - size, center.y - size, center.z - size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x + size, center.y - size, center.z - size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x + size, center.y + size, center.z - size);
		cubVerts.push_back(v);

		v = glm::vec3(center.x - size, center.y + size, center.z - size);
		cubVerts.push_back(v);
	}
	
};

CubeRender c;

namespace Sphere
{
	extern void updateSphere(glm::vec3 pos, float radius);
}

namespace Cube
{
	extern void updateCube(float* array_data);
}

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
	
	Cube::updateCube(c.CubeRenderToFloatPointer());
}
void PhysicsUpdate(float dt) {
	//TODO
	Cube::updateCube(c.CubeRenderToFloatPointer());
}
void PhysicsCleanup() {
	//TODO
}