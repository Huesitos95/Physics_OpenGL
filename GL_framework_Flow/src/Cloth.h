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

private:
	Cloth();
};

Cloth::Cloth()
{
}

Cloth::~Cloth()
{
}