#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "Cloth.h"
#include <vector>

bool show_test_window = false;

float* v;
struct Particle
{
	float x, y, z;
	Particle() {}
	Particle(float i, float j, float k) { x = i; y = j; z = k; }
};

struct ParticlesList
{
	std::vector<Particle> list;
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

namespace ClothMesh
{
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);
}

void PhysicsInit() {
	//TODO
	float x = 4.5f;
	float z = 6.5f;
	float y = 6.0f;

	v = new float[ClothMesh::numVerts * 3];
	
	for (int j = 0; j < ClothMesh::numRows; j++)
	{
		z -= 0.5f;
		x = 4.5f;
		for (int i = 0; i < ClothMesh::numCols; i++)
		{
			x -= 0.5;
			Particle p(x, y, z);
			v[particles.list.size() * 3 + 0] = p.x;
			v[particles.list.size() * 3 + 1] = p.y;
			v[particles.list.size() * 3 + 2] = p.z;
			particles.list.push_back(p);
		}
	}	
	ClothMesh::updateClothMesh(v);
}
void PhysicsUpdate(float dt) {
	//TODO
	ClothMesh::updateClothMesh(v);
}
void PhysicsCleanup() {
	//TODO
}