#pragma once
namespace nAI {
	// IDLE = 1
	// SEEK = 2
	// APPROACH = 4
	// PURSURE = 8
	// FLEE = 16
	// EVADE = 32
	// WANDER = 64
	// FOLLOW_PATH = 128
	// SEPERATION = 256
	// ALIGNMENT = 512
	// COHESION = 1024
	// FLOCK = SEPERATION | ALIGNMENT | COHESION
	enum class eAIBehviours {
		// BASIC STEERING BEHAVIOURS
		IDLE = 1,
		SEEK = 2,
		APPROACH = 4,
		PURSURE = 8,
		FLEE = 16,
		EVADE = 32,
		WANDER = 64,
		FOLLOW_PATH = 128,
		SEPERATION = 256,
		ALIGNMENT = 512,
		COHESION = 1024,

		// COMPLEX COMBINATION BEHAVIOURS
		FLOCK = SEPERATION | ALIGNMENT | COHESION,
		FLOCK_PATH = FLOCK | FOLLOW_PATH
	};

	enum class ePlayerTypes {
		PLAYER,
		FRIENDLY,
		NEUTRAL,
		HOSTILE
		// D&D / PATHFINDER HAS MORE STATES POSSIBLY EXPAND LATER...
	};

	enum class eFormationBehaviours {
		NONE,
		CIRCLE,
		SQUARE,
		WEDGE,
		LINE,
		COLUMN
	};

	struct waypoints {
		std::vector<glm::vec3> vec_waypointPositions;
		size_t waypointPos;
		float waypointRadius;
		bool isInvert;
	};

	struct formationAnchor {
		glm::vec3 position;
		// ask dylan about storing facing direction of the anchor object...
		glm::vec3 centerPos;
		float offset;
		float circleoffset;
		float nodes;
		float radius;	// USE IN PLACE OF OFFSET FOR THE CIRCLE FORMATION
		size_t rowdef;
		size_t columndef;
	};
}
