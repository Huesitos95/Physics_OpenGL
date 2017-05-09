#pragma once
class Cloth
{
public:
	static Cloth &getInstance()
	{
		static Cloth instance;
		return instance;
	}

	Cloth &operator=(Cloth &c) = delete;
	~Cloth();

	void Update(float);

private:
	Cloth();

	float** vertex;
};

namespace ClothMesh
{
	extern const int numCols;
	extern const int numRows;
	extern void updateClothMesh(float* array_data);
}

Cloth::Cloth()
{
	vertex = new float*[ClothMesh::numRows];
	for (int i = 0; i < ClothMesh::numRows; i++)
	{
		vertex[i] = new float[ClothMesh::numCols*3];

		for (int j = 0; j < ClothMesh::numCols; j++)
		{
			vertex[i][0 + j * 3] = i*0.25;
			vertex[i][1 + j * 3] = 5;
			vertex[i][2 + j * 3] = j*0.25;

		}
	}
	ClothMesh::updateClothMesh(vertex[0]);
}

Cloth::~Cloth()
{
}

void Cloth::Update(float dt)
{
	ClothMesh::updateClothMesh(vertex[0]);
}