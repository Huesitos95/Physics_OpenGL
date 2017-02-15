#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

#include "GL_framework.h"

/////////fw decl
namespace ImGui {
	void Render();
}
void setupCube();
void cleanupCube();
void drawCube();
void setupSphere();
void cleanupSphere();
void drawSphere();
////////////////

namespace {
	const float FOV = glm::radians(65.f);

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, -5.f, -15.f };
	float rota[2] = { 0.f, 0.f };
}

void GLResize(int width, int height) {
	glViewport(0, 0, width, height);
	if(height != 0) _projection = glm::perspective(FOV, (float)width / (float)height, 1.f, 50.f);
	else _projection = glm::perspective(FOV, 0.f, 1.f, 50.f);
}

void GLmousecb(MouseEvent ev) {
	if(prevMouse.waspressed && prevMouse.button == ev.button) {
		float diffx = ev.posx - prevMouse.lastx;
		float diffy = ev.posy - prevMouse.lasty;
		switch(ev.button) {
		case MouseEvent::Button::Left: //ROTATE
			rota[0] += diffx * 0.005f;
			rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: //MOVE XY
			panv[0] += diffx * 0.03f;
			panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: //MOVE Z
			panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	} else {
		prevMouse.button = ev.button;
		prevMouse.waspressed = true;
	}
	prevMouse.lastx = ev.posx;
	prevMouse.lasty = ev.posy;
}

void GLinit(int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	_projection = glm::perspective(FOV, (float)width/(float)height, 1.f, 50.f);

	//Setup shaders & geometry
	setupCube();
	setupSphere();
}

void GLcleanup() {
	cleanupCube();
	cleanupSphere();
}

void GLrender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_modelView = glm::mat4(1.f);
	_modelView = glm::translate(_modelView, glm::vec3(panv[0], panv[1], panv[2]));
	_modelView = glm::rotate(_modelView, rota[1], glm::vec3(1.f, 0.f, 0.f));
	_modelView = glm::rotate(_modelView, rota[0], glm::vec3(0.f, 1.f, 0.f));

	_inv_modelview = glm::inverse(_modelView);
	_cameraPoint = _inv_modelview * glm::vec4(0.f, 0.f, 0.f, 1.f);

	_MVP = _projection * _modelView;

	//render code
	drawCube();
	drawSphere();

	ImGui::Render();
}


//////////////////////////////////////////////////
GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name="") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
void linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

namespace {
	GLuint cubeVao;
	GLuint cubeVbo[2];
	GLuint cubeShaders[2];
	GLuint cubeProgram;

	float cubeVerts[] = {
		//-5,0,-5 -- 5, 10, 5
		-5.f,  0.f, -5.f,
		 5.f,  0.f, -5.f,
		 5.f,  0.f,  5.f,
		-5.f,  0.f,  5.f, 
		-5.f, 10.f, -5.f,
		 5.f, 10.f, -5.f,
		 5.f, 10.f,  5.f,
		-5.f, 10.f,  5.f,
	};
	GLubyte cubeIdx[] = { 
		1, 0, 2, 3, //Floor - TriangleStrip
		0, 1, 5, 4, //Wall - Lines
		1, 2, 6, 5, //Wall - Lines
		2, 3, 7, 6, //Wall - Lines
		3, 0, 4, 7  //Wall - Lines
	};

	const char* vertShader_xform = 
"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* fragShader_flatColor = 
"#version 330\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = color;\n\
}";
}
void setupCube() {
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);
	glGenBuffers(2, cubeVbo);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 20, cubeIdx, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cubeShaders[0] = compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
	cubeShaders[1] = compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, cubeShaders[0]);
	glAttachShader(cubeProgram, cubeShaders[1]);
	glBindAttribLocation(cubeProgram, 0, "in_Position");
	linkProgram(cubeProgram);
}
void cleanupCube() {
	glDeleteBuffers(2, cubeVbo);
	glDeleteVertexArrays(1, &cubeVao);

	glDeleteProgram(cubeProgram);
	glDeleteShader(cubeShaders[0]);
	glDeleteShader(cubeShaders[1]);
}
void drawCube() {
	glBindVertexArray(cubeVao);
	glUseProgram(cubeProgram);
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(_MVP));
	//FLOOR
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
	//WALLS
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte)*4));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte)*8));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte)*12));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte)*16));

	glUseProgram(0);
	glBindVertexArray(0);
}

namespace {
	GLuint sphereVao;
	GLuint sphereVbo;
	GLuint sphereShaders[3];
	GLuint sphereProgram;

	float sphereVerts[] = {
		0.f, 1.f, 0.f
	};

	const char* sphere_vertShader =
"#version 330\n\
in vec3 in_Position;\n\
void main() {\n\
	gl_Position = vec4(in_Position, 1.0);\n\
}";
	const char* sphere_geomShader =
"#version 330\n\
layout(points) in;\n\
layout(triangle_strip, max_vertices = 4) out;\n\
out vec4 eyePos;\n\
out vec4 centerEyePos;\n\
uniform mat4 mvpMat;\n\
uniform mat4 mv_Mat;\n\
uniform mat4 projMat;\n\
uniform vec4 camPoint;\n\
uniform float radius;\n\
vec4 nu_verts[4];\n\
void main() {\n\
	vec3 n = normalize((camPoint - gl_in[0].gl_Position).xyz);\n\
	vec3 up = vec3(0.0, 1.0, 0.0);\n\
	vec3 u = normalize(cross(up, n));\n\
	vec3 v = normalize(cross(n, u));\n\
	nu_verts[0] = vec4(-radius*u - radius*v, 0.0); \n\
	nu_verts[1] = vec4( radius*u - radius*v, 0.0); \n\
	nu_verts[2] = vec4(-radius*u + radius*v, 0.0); \n\
	nu_verts[3] = vec4( radius*u + radius*v, 0.0); \n\
	for (int i = 0; i < 4; ++i) {\n\
		centerEyePos = mv_Mat * gl_in[0].gl_Position;\n\
		eyePos = mv_Mat * (gl_in[0].gl_Position + nu_verts[i]);\n\
		gl_Position = projMat * eyePos;\n\
		EmitVertex();\n\
	}\n\
	EndPrimitive();\n\
}";
	const char* sphere_fragShader_flatColor =
"#version 330\n\
in vec4 eyePos;\n\
in vec4 centerEyePos;\n\
out vec4 out_Color;\n\
uniform mat4 projMat;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
uniform float radius;\n\
void main() {\n\
	vec4 diff = eyePos - centerEyePos;\n\
	float distSq2C = dot(diff, diff);\n\
	if (distSq2C > (radius*radius)) {\n\
		discard; \n\
	} else {\n\
		float h = sqrt(radius*radius - distSq2C);\n\
		vec4 nuEyePos = vec4(eyePos.xy, eyePos.z + h, 1.0);\n\
		vec4 nuPos = projMat * nuEyePos;\n\
		gl_FragDepth = ((nuPos.z / nuPos.w) + 1) * 0.5;\n\
		vec3 normal = normalize(nuEyePos - centerEyePos).xyz;\n\
		out_Color = vec4(color.xyz * dot(normal, (mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)).xyz) + color.xyz * 0.3, 1.0 );\n\
	}\n\
}";
}
void setupSphere() {
	glGenVertexArrays(1, &sphereVao);
	glBindVertexArray(sphereVao);
	glGenBuffers(1, &sphereVbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, sphereVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	sphereShaders[0] = compileShader(sphere_vertShader, GL_VERTEX_SHADER, "sphereVert");
	sphereShaders[1] = compileShader(sphere_geomShader, GL_GEOMETRY_SHADER, "sphereGeom");
	sphereShaders[2] = compileShader(sphere_fragShader_flatColor, GL_FRAGMENT_SHADER, "sphereFrag");

	sphereProgram = glCreateProgram();
	glAttachShader(sphereProgram, sphereShaders[0]);
	glAttachShader(sphereProgram, sphereShaders[1]);
	glAttachShader(sphereProgram, sphereShaders[2]);
	glBindAttribLocation(sphereProgram, 0, "in_Position");
	linkProgram(sphereProgram);
}
void cleanupSphere() {
	glDeleteBuffers(1, &sphereVbo);
	glDeleteVertexArrays(1, &sphereVao);

	glDeleteProgram(sphereProgram);
	glDeleteShader(sphereShaders[0]);
	glDeleteShader(sphereShaders[1]);
	glDeleteShader(sphereShaders[2]);
}
void drawSphere() {
	glBindVertexArray(sphereVao);
	glUseProgram(sphereProgram);
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(_MVP));
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(_modelView));
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "projMat"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUniform4fv(glGetUniformLocation(sphereProgram, "camPoint"), 1, &_cameraPoint[0]);
	glUniform4f(glGetUniformLocation(sphereProgram, "color"), 0.6f, 0.1f, 0.1f, 1.f);
	glUniform1f(glGetUniformLocation(sphereProgram, "radius"), 1.f);
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
	glBindVertexArray(0);
}