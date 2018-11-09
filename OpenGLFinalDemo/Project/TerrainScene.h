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

class CTerrainScene : public CScene
{
public:
	//inherited funcs
	virtual void RenderScene() override;
	virtual void ConfigurateScene() override;
	virtual void ResetScene() override;
	virtual void UpdateScene(float _tick) override;

	//new funcs
	void InitialisingVertices();
	void InitSphere();
	void InitGeoShader();
	void LoadHeightMap(std::wstring _fileName);
	void LoadHeightMapPNG(std::wstring _fileName);
	void smoothing();
	float Average(int x, int y);
	float CheckTerrainHeight(float x, float y);

	//Render
	void RenderTerrain();
	void RenderSphere();
	void RenderGeo();
private:
	//TERRAIN
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	
	Vertex TerrainPointArray[256][256] = {};

	int IndiceCount;
	GLuint m_vaoT; // Vertex Array Obj
	GLuint m_vboT; // Texture Obj

	Transform TerrainTrans;
	float fVerticeGap;
	float fHeightMod;

	int rows;
	int cols;


	CGameObject* BackGround;
	//SPHERE
	Transform SphereTrans;
	float fSphereHeight;
	float fSphereWIdth;
	int SphereIndiceCount;
	float fRadiusSphere;
	GLuint m_vaoS; // Vertex Array Obj
	GLuint m_vboS; // Texture Obj


	//GEO
	Transform GeoTrans;
	GLuint m_vaoG; // Vertex Array Obj
	GLuint m_vboG; // Texture Obj
};