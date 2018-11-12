#pragma once
#include "Engine/Utility.h"
#include "Engine/Camera.h"
#include <vector>
class CRainParticle
{
public:
	CRainParticle();
	CRainParticle(glm::vec3 Position, glm::vec3 Velocity, float ElapsedTime, float Speed, float ID, CCamera* camera);
	~CRainParticle();

	void Update(float _delta);

	//Storage
	Transform m_Transform;

	float m_fRand;
	float m_fElapsedTime;
	float m_fInitalElapsedTime;

	glm::vec3 m_vInitalPos;
	glm::vec3 m_vVelocity;
	float m_fSpeed;

	float m_fID;
	CCamera* m_pCamera;

private:
	float Randomization();

};