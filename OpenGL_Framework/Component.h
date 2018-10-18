#pragma once

#include <string>

using std::string;

enum class CompType
{
	Transform,
	Mesh,
	ShaderProgram,
	Camera,
	MeshBounds,
	ColliderBounds,
};


class Component
{
public:
	virtual void update(float deltaTime) = 0;
	CompType getCompType() const;
	void setCompType(const CompType& compType);
	string getName() const;
	void setName(const string& name);
	int getID() const;
	void setID(const int id);

private:
	CompType _compType;
	string _name;
	unsigned int _id;
};