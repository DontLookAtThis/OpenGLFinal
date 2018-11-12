#pragma once

#include "Engine/Scene.h"
#include "Engine/Utility.h"
// Forward Declare
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

class CGameObject;

class COpenGlScene : public CScene
{
public:
	//inherited funcs
	virtual void RenderScene() override;
	virtual void ConfigurateScene() override;
	virtual void ResetScene() override;
	virtual void UpdateScene(float _tick) override;

private:

};