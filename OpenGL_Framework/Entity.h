#pragma once

class Entity
{
public:
	Entity();
	~Entity();

	unsigned int getEid() const;
	static void resetEids();

private:
	unsigned int _eid;

	static unsigned int eidCounter;
};