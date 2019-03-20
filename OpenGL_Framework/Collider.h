#pragma once

#include "Component.h"
#include "SoundComponent.h"
#include "PhysicsBodyComponent.h"

enum class TTag
{
	Player,
	Platform,
	Enemy,
	Coin,
	Acorn,
	Spike,
	Checkpoint,
	VictoryPoint,
	Generic,
	TopPlatform
};

struct Bounds
{
	Bounds(const vec3& _centre = vec3(0.0f), const vec3& _size = vec3(0.0f))
	{
		centre = centre;
		extends = _size * 0.5f;
		size = _size;
		max = centre + extends;
		min = centre - extends;
	}

	Bounds(const Bounds& other)
	{
		centre = other.centre;
		extends = other.extends;
		size = other.size;
		max = other.max;
		min = other.min;
	}

	vec3 centre = vec3(0.0f);
	vec3 extends;
	vec3 max;
	vec3 min;
	vec3 size = vec3(0.0f);
};

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	void onCollisionEnter(Entity* self, Entity* other);
	void onCollisionStay(Entity* self, Entity* other);
	void onCollisionExit(Entity* self, Entity* other);

	virtual void update(TransformComponent* transform) = 0;
	virtual void draw() = 0;

	Bounds* getBounds() const;
	void setBounds(const Bounds& newBounds);
	PhysicsBodyComponent* getPhysicsBody() const;
	void setPhysicsBody(PhysicsBodyComponent* physicsBody);
	vec3 getOffset() const;
	void setOffset(const vec3& offset);
	bool getEnabled() const;
	void setEnabled(const bool enabled);

	TTag getTag() const;
	void setTag(const TTag tag);

	// Testing function pointers
	//typedef void(*onCollision)(Entity* self, Entity* other);
	//onCollision onCollisionEnter;
	//onCollision onCollisionStay;
	//onCollision onCollisionExit;

	vec3 _max;
	vec3 _min;

	bool beastMode = false;
protected:
	SoundComponent* _sound;

	Bounds* _bounds;
	vec3 _contactOffset;
	bool _enabled;
	PhysicsBodyComponent* _physicsBody = nullptr;
	TTag _tag;
};