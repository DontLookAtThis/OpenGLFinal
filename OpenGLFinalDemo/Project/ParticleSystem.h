#pragma once
#include "RainParticle.h"
#include "Engine/Camera.h"

class CParticleSystem {

public: 
	CParticleSystem(glm::vec3 origin, CCamera* _camera, std::string texFileName);
	~CParticleSystem();
		
	void render(float dt);
		
	std::vector<CRainParticle> particles; 
	std::vector<glm::vec3> vPosition;

	CCamera* m_pCamera;

	GLuint vao, vbo, texture, program;
	float nParticles;

	float Randomization();
};
