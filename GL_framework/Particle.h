#pragma once
#include <glm\glm.hpp>

//http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/

class Particle
{
private:
	glm::vec3 _currentPosition;
	glm::vec3 _previousPosition;
	glm::vec3 _force;
	glm::vec3 _velocity;
	float _lifeTime;
	float emissionRate;

public:

	Particle();
	Particle(const float & x, const float& y, const float &z);
	~Particle();

	//Setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);

	//Getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();
	glm::vec3 getForce();
	glm::vec3 getVelocity();
	float getLifeTime();

	//Funcions
	void addForce(glm::vec3 force);
	void addForce(const float &x, const float &y, const float &z);
	void updateParticle(); //Implementar tant per Euler com per Verlet
};

