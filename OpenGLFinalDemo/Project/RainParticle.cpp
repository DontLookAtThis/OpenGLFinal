#include "RainParticle.h"

#include "Engine/Time.h"

#include <random>

CRainParticle::CRainParticle()
{
	m_fElapsedTime = 5.0f;
	m_vVelocity = glm::vec3(0, 0, 0);
	m_fSpeed = 0.0f;
	m_Transform.position = glm::vec3(0, 0, 0);
	m_vInitalPos = m_Transform.position;
}

CRainParticle::CRainParticle(glm::vec3 Position, glm::vec3 Velocity, float ElapsedTime, float Speed, float ID, CCamera* camera)
{
	m_fElapsedTime = ElapsedTime;
	m_vVelocity = Velocity;
	m_fSpeed = Speed;
	m_Transform.position = Position;
	m_vInitalPos = m_Transform.position;
	m_fID = ID;
	m_pCamera = camera;
}

CRainParticle::~CRainParticle()
{

}

void CRainParticle::Update(float _delta)
{
	m_vVelocity.y += -0.2 *  .0167f;
	m_Transform.position += m_vVelocity;
	m_fElapsedTime -= .000167;

	if (m_fElapsedTime <= 0.0f)
	{
		m_fElapsedTime = 5.0f;
		m_Transform.position = m_vInitalPos;

		m_vVelocity = glm::vec3(0.25 * cos(m_fID * .0167) + 0.25f * Randomization() - 0.125f,
			1.5f + 0.25f * Randomization() - 0.125f,
			0.25 * sin(m_fID* .0167) + 0.25f * Randomization() - 0.125f);
		m_fElapsedTime = (Randomization() + 0.125);
	}
}



float CRainParticle::Randomization()
{
	float fr = (float)rand() / (double)RAND_MAX;
	return fr;
}
