#include "cPhysicsFactory.hpp"

namespace nPhysics {
	cPhysicsFactory::cPhysicsFactory() { }

	iPhysicsWorld* cPhysicsFactory::CreateWorld() {
		return new cPhysicsWorld();
	}

	iBallComponent* cPhysicsFactory::CreateBall(const sBallDef& def, const unsigned& id) {
		return new cBallComponent(def, id);
	}

	iPlaneComponent* cPhysicsFactory::CreatePlane(const sPlaneDef& def, const unsigned& id) {
		return new cPlaneComponent(def, id);
	}

	iClothComponent* cPhysicsFactory::CreateCloth(const sClothDef& def, const unsigned& id)
	{
		// todo make sure def is possible?
		return new cClothComponent(def, id);
	}
}