#pragma once

#include <string>

using std::string;

enum class CompType
{
	Transform,
	Mesh,
	ShaderProgram,
	PhysicsBody
};


class Component
{
public:
	Component();
	Component(CompType compType);
	~Component();
	virtual void update(float deltaTime) = 0;
	CompType getCompType() const;
	void setCompType(const CompType& compType);
	int getID() const;
	void setID(const int id);

private:
	CompType _compType;
	unsigned int _id;
};