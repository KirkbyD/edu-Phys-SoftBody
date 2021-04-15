#include "cCollisionListener.hpp"
#include "cComplexObject.hpp"
#include <iostream>

void cCollisionListener::Collide(nPhysics::iPhysicsComponent* compA, nPhysics::iPhysicsComponent* compB) {
	//Resolve all collisions here!

	//Only printing out the name of ball type objects that are colliding.
	if (compA->GetComponentType() == nPhysics::eComponentType::ball
		&& compB->GetComponentType() == nPhysics::eComponentType::ball)
		std::cout << "Collision:\t"
			<< reinterpret_cast<cComplexObject*>(compA->GetParentPointer())->friendlyName
			<< " on "
			<< reinterpret_cast<cComplexObject*>(compB->GetParentPointer())->friendlyName
			<< std::endl;
}
