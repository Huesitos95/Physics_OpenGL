#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "Solver.h"


bool show_test_window = false;
bool Euler = false;
bool Cascada = true;

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
		Solver::getInstance().actualSolver = TipusSolver::EULER;
		Solver::getInstance().RestartParticle();
	}
	if (ImGui::Button("Verlet", ImVec2(150, 50)))
	{
		Euler = false;
		Solver::getInstance().actualSolver = TipusSolver::VERLET;
		Solver::getInstance().RestartParticle();
	}

	if (ImGui::Button("Cascada", ImVec2(100, 50)))
	{
		Cascada = true;
		Solver::getInstance().actualSistema = CASCADA;
		Solver::getInstance().RestartParticle();
	}
	if (ImGui::Button("Font", ImVec2(100, 50)))
	{
		Cascada = false;
		Solver::getInstance().actualSistema = FONT;
		Solver::getInstance().RestartParticle();
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
	float v=Solver::getInstance().tempsVidaParticula;
	ImGui::DragFloat("TempsParticula", &Solver::getInstance().tempsVidaParticula, 0.1f, 0, 30, "%.3f", 1.f);
	
	if (Cascada) {
		ImGui::Text("Current System: Cascada");
	}
	else {
		ImGui::Text("Current System: Font");
	}
}
void PhysicsCleanup() {
	//TODO
}