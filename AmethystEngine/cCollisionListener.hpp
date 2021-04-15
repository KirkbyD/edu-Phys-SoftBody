#pragma once
#include <physics/interfaces/iCollisionListener.h>

//cInherits from the iCollisionListener interface in Physics_Interfaces project.
class cCollisionListener : public nPhysics::iCollisionListener{
public:
	cCollisionListener() : nPhysics::iCollisionListener() {}
	virtual ~cCollisionListener() { }

	// Inherited via iCollisionListener
	// The Collide function is automatically called at the physics library level and chains outward to be resolved here.
	virtual void Collide(nPhysics::iPhysicsComponent* compA, nPhysics::iPhysicsComponent* compB) override;
};