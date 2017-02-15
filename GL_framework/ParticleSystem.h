#pragma once
#include <glm\glm.hpp>
struct Particle {
	glm::vec3 pos;
	int lifeTime;

};

class ParticleSystem
{
private:
	int emissionRate;
	glm::vec3 systemPos;

public:
	ParticleSystem();
	~ParticleSystem();
};

