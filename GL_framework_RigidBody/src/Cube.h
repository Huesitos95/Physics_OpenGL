#pragma once
//INTENT CLASE CUB PER RENDER
//POTSER PASAR-HO A RENDER.cpp & Renderprims?
class Cube
{
public:
	static Cube &getInstance()
	{
		static Cube instance;
		return instance;
	}

	Cube &operator=(Cube &c) = delete;
	~Cube();

	void Update();

private:
	Cube();

};

namespace Cube
{
	extern void updateCube();
}

void Cube::updateCube()
{

}