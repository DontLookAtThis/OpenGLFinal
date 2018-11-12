#include "OpenGLScene.h"
#include "Engine/Camera.h"
#include "Engine/AssetMgr.h"
#include "Engine/RigidBody2D.h"
#include "Engine/SpriteRender.h"
#include "Engine/Input.h"
#include "Engine/SceneMgr.h"
#include "Engine/Debug.h"
#include "Engine/Time.h"

#include "ParticleSystem.h"
#include "RainParticle.h"

#include <cmath>
#include <math.h>
#include <array>

void COpenGlScene::RenderScene()
{
	__super::RenderScene();
	if (m_mainCamera)
	{

	}
}

void COpenGlScene::ConfigurateScene()
{
	__super::ConfigurateScene();
	this->m_sceneName = "OpenGLScene";
	/** Create Camera Object */
	this->m_mainCamera = new CCamera();

}

void COpenGlScene::ResetScene()
{
	__super::ResetScene();

}

void COpenGlScene::UpdateScene(float _tick)
{
	__super::UpdateScene(_tick);

}
