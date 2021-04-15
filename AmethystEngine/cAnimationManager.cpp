#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#else
#define DBG_NEW
#endif

#include <iostream>`
#include "cAnimationManager.hpp"
#include "nConvert.hpp"
#include <glm\gtc\type_ptr.hpp>


#pragma region SINGLETON
cAnimationManager cAnimationManager::stonAnimaMngr;
cAnimationManager* cAnimationManager::GetAnimationManager() { return &(cAnimationManager::stonAnimaMngr); }
cAnimationManager::cAnimationManager() {
	std::cout << "Animation Manager Created" << std::endl;
	pMediator = nullptr;
}
#pragma endregion


void cAnimationManager::DeconstructAnimationComponents() {
	for (std::pair<std::string, cAnimationComponent*> it : mpAnima) {
		delete it.second;
	}
	mpAnima.clear();
}

cAnimationComponent* cAnimationManager::LoadMeshFromFile(const std::string& friendlyName, const std::string& filename) {
	cAnimationComponent* AnimaObj = dynamic_cast<cAnimationComponent*>(this->_fact_game_obj.CreateGameObject("[ANIMATION]"));

	if (AnimaObj->LoadMeshFromFile(friendlyName, filename)) {
		AnimaObj->friendlyIDNumber = Module_Hex_Value | System_Hex_Value | next_UniqueComponentID++;
		mpAnima[friendlyName] = AnimaObj;
		return AnimaObj;
	}
	return nullptr;
}

void cAnimationManager::IterateComponent(cAnimationComponent* component, float dt) {
	float TicksPerSecond = static_cast<float>(component->GetScene()->mAnimations[0]->mTicksPerSecond != 0 ?
		component->GetScene()->mAnimations[0]->mTicksPerSecond : 25.0);

	float TimeInTicks = dt * TicksPerSecond;
	//float AnimationTime = fmod(TimeInTicks, (float)component->GetScene()->mAnimations[0]->mDuration);

	//check prev anim
	std::string prev = component->GetPrevAnimation();
	bool wasIdle = (prev == "Idle" || prev == "FightIdle") ? true : false;

	float AnimaTime = component->GetTime() + TimeInTicks;
	if (AnimaTime > (float)component->GetScene()->mAnimations[0]->mDuration || wasIdle) {
		component->SetTime(0.f);
		AnimaTime = 0.f;

		cComplexObject* pParent = dynamic_cast<cComplexObject*>(component->GetParent());
		std::string currAnim = pParent->getCurrentAnimationName();
		
		if (!wasIdle) {
			//Check if we turned
			if (currAnim == "TurnL") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->SetOrientation(glm::quatLookAt(-right, glm::vec3(0.f, 1.f, 0.f)));
			}
			else if (currAnim == "TurnR") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->SetOrientation(glm::quatLookAt(right, glm::vec3(0.f, 1.f, 0.f)));
			}
			//Check if it was our 'Dodge' (need to move further for the dodge specifically)
			else if (currAnim == "DodgeL") {
				pParent->SetPosition(pParent->getBonePositionByBoneName("mixamorig:LeftFoot"));
			}
			else if (currAnim == "DodgeR") {
				pParent->SetPosition(pParent->getBonePositionByBoneName("mixamorig:RightFoot"));
			}

			//Clear animations and dispatch idle if queue empty
			component->PopAnimation();
		}
		if (component->QueueEmpty()) {
			//Check for alternate Idle Transitions
			if (currAnim == "IdleToFight" || currAnim == "FightIdle" || currAnim == "Punch") {
				component->QueueAnimation("FightIdle");
			}
			else
			component->QueueAnimation("Idle");
		}

		//Grab new animation name
		currAnim = pParent->getCurrentAnimationName();
		//Dispatch physics Impulses
		float accelSpeed = 2.5f;
		if (currAnim == "Idle"
			|| currAnim == "FightIdle"
			|| currAnim == "IdleToFight"
			|| currAnim == "FightToIdle"
			|| currAnim == "Punch") {
			//pParent->VelocityZero();
		}
		else if (currAnim == "JumpF" || currAnim == "DodgeL" || currAnim == "DodgeR") {
			//Check if should jump backwards
			if (currAnim == "JumpF") {
				glm::vec3 front = pParent->getFrontVector();
				glm::vec3 vel = pParent->getVelocity();
				if (glm::dot(front, vel) < 0.f) {
					component->PopAnimation();
					component->QueueAnimation("JumpB");
					//pParent->ApplyImpulse(glm::vec3(0.f, 5.f, 0.f));
				}
			/*	else
					pParent->ApplyImpulse(glm::vec3(0.f, 10.f, 0.f));*/
			}
		}
		else {
			pParent->VelocityZero();
			if (currAnim == "WalkF") {
				glm::vec3 front = pParent->getFrontVector();
				pParent->ApplyImpulse(glm::vec3(front.x * accelSpeed, 0.f, front.z * accelSpeed) * .75f);
			}
			else if (currAnim == "WalkB") {
				glm::vec3 front = pParent->getFrontVector();
				pParent->ApplyImpulse(glm::vec3(front.x * -accelSpeed, 0.f, front.z * -accelSpeed) * .5f);
			}
			else if (currAnim == "WalkL") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->ApplyImpulse(glm::vec3(right.x * accelSpeed, 0.f, right.z * accelSpeed) * .5f);
			}
			else if (currAnim == "WalkR") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->ApplyImpulse(glm::vec3(right.x * -accelSpeed, 0.f, right.z * -accelSpeed) * .5f);
			}
			else if (currAnim == "RunF") {
				glm::vec3 front = pParent->getFrontVector();
				pParent->ApplyImpulse(glm::vec3(front.x * accelSpeed, 0.f, front.z * accelSpeed) * 1.5f);
			}
			else if (currAnim == "RunB") {
				glm::vec3 front = pParent->getFrontVector();
				pParent->ApplyImpulse(glm::vec3(front.x * -accelSpeed, 0.f, front.z * -accelSpeed));
			}
			else if (currAnim == "RunL") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->ApplyImpulse(glm::vec3(right.x * accelSpeed, 0.f, right.z * accelSpeed));
			}
			else if (currAnim == "RunR") {
				glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), pParent->getFrontVector());
				pParent->ApplyImpulse(glm::vec3(right.x * -accelSpeed, 0.f, right.z * -accelSpeed));
			}
		}

		component->SetPrevAnimation(currAnim);
	}

	else
		component->SetTime(AnimaTime);
	return;
}

void cAnimationManager::Update(float dt, GLint shaderProgID, cRenderer* pRenderer, cVAOManager* pVAOManager) {
	for (auto it : mpAnima) {
		auto pCurrentObject = it.second;

		IterateComponent(pCurrentObject, dt);

		// Set to all identity
		const int NUMBEROFBONES = pCurrentObject->GetNumBones();

		// Taken from "Skinned Mesh 2 - todo.docx"
		std::vector< glm::mat4x4 > vecFinalTransformation;
		std::vector< glm::mat4x4 > vecOffsets;
		std::vector< glm::mat4x4 > vecObjectBoneTransformation;

		// This loads the bone transforms from the animation model
		pCurrentObject->BoneTransform(vecFinalTransformation,
									  vecObjectBoneTransformation,
									  vecOffsets);

		// Wait until all threads are done updating.

		GLint numBonesUsed = (GLint)vecFinalTransformation.size();

		std::vector<sModelDrawInfo*> vecDrawInfo = it.second->GetMeshes();

		for (size_t i = 0; i < vecDrawInfo.size(); i++) {
			pRenderer->RenderAnimaObject(pCurrentObject, vecDrawInfo[i], shaderProgID, pVAOManager, numBonesUsed, glm::value_ptr(vecFinalTransformation[0]));
		}
	}

	return;
}

//void cAnimationManager::Render(cRenderer* pRenderer, GLint shaderProgID) {
//	for (auto it : mpAnima) {
//	}
//	return;
//}



#pragma region MEDIATOR_COMMUNICATION
void cAnimationManager::setMediatorPointer(iMediatorInterface* pMediator) {
	this->pMediator = pMediator;
	return;
}

sData cAnimationManager::RecieveMessage(sData& data) {
	data.setResult(OK);

	return data;
}
#pragma endregion
