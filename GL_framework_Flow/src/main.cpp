#include <windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <cstdio>

#include "GL_framework.h"

static GLFWwindow *window;

extern void PhysicsInit();
extern void PhysicsUpdate(float dt);
extern void PhysicsCleanup();
extern void GUI();

extern void GLmousecb(MouseEvent ev);
extern void GLResize(int width, int height);
extern void GLinit(int width, int height);
extern void GLcleanup();
extern void GLrender();

namespace {
	const int expected_fps = 30;
	const double expected_frametime = 1.0 / expected_fps;
	double prev_frametimestamp = 0;
	double curr_frametimestamp = 0;
	double wait_time_ms = 1e3 * expected_frametime;

	void waitforFrameEnd() {
		curr_frametimestamp = glfwGetTime();
		if((curr_frametimestamp - prev_frametimestamp) < 5e-3)
			wait_time_ms = 1e3 * (expected_frametime - (curr_frametimestamp - prev_frametimestamp));
		DWORD wait = (DWORD)wait_time_ms;
		if(wait > 0) {
			Sleep(wait);
		}
		prev_frametimestamp = curr_frametimestamp;
	}

	void GLFWwindowresize(GLFWwindow *, int w, int h) {
		GLResize(w, h);
	}
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
int main(int argc, char** argv){
	//Init GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Couldn't initialize GLFW\n");
		return -1;
	}
	//Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "GL_framework", NULL, NULL);
	if(!window) {
		glfwTerminate();
		fprintf(stderr, "Couldn't create GL window\n");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, GLFWwindowresize);

	//Init GLEW
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	//Init scene
	GLinit(display_w, display_h);
	PhysicsInit();
	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	prev_frametimestamp = glfwGetTime();
	while(!glfwWindowShouldClose(window)) { // Loop until the user closes the window
		glfwPollEvents(); // Poll for events
		ImGui_ImplGlfwGL3_NewFrame();
		
		ImGuiIO& io = ImGui::GetIO();
		GUI();
		PhysicsUpdate((float)expected_frametime);
		if(!io.WantCaptureMouse) {
			MouseEvent ev = {io.MousePos.x, io.MousePos.y, 
				(io.MouseDown[0] ? MouseEvent::Button::Left : 
				(io.MouseDown[1] ? MouseEvent::Button::Right :
				(io.MouseDown[2] ? MouseEvent::Button::Middle :
				MouseEvent::Button::None)))};
			GLmousecb(ev);
		}
		GLrender();
	
		glfwSwapBuffers(window);//Swap front and back buffers
		waitforFrameEnd();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	PhysicsCleanup();
	GLcleanup();

	glfwTerminate();
	return 0;
}