#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "Solver.h"


bool show_test_window = false;
bool Euler = false;

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
	if (ImGui::Button("EulerSemiImplicit", ImVec2(150, 50)))
	{
		Euler = true;
		
	}
	if (ImGui::Button("Verlet", ImVec2(150, 50)))
	{
		Euler = false;
		
	}

}

void PhysicsInit() {
	//TODO
}
void PhysicsUpdate(float dt) {
	//TODO
	if (Euler)
	{
		Solver::getInstance().EulerSemiImplicit(dt);
		ImGui::Text("Current Solver: EulerSemiImplicit");
	}
	else
	{
		Solver::getInstance().Verlet(dt);
		ImGui::Text("Current Solver: Verlet");
	}
}
void PhysicsCleanup() {
	//TODO
}