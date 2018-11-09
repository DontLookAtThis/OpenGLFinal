#define _USE_MATH_DEFINES

#include "TerrainScene.h"
#include "Engine/Camera.h"
#include "Engine/AssetMgr.h"
#include "Engine/RigidBody2D.h"
#include "Engine/SpriteRender.h"
#include "Engine/Input.h"
#include "Engine/SceneMgr.h"
#include "Engine/Debug.h"
#include "Engine/Time.h"

#include <cmath>
#include <math.h>
#include <array>



void CTerrainScene::RenderScene()
{
	__super::RenderScene();
	if (m_mainCamera)
	{
		//TERRAIN
		RenderTerrain();
		//----------


		//SPHERE------
		RenderSphere();
		//---------------------------------
		RenderGeo();
	}
}

void CTerrainScene::ConfigurateScene()
{
	__super::ConfigurateScene();

	fVerticeGap = 2.0f;
	fHeightMod = 3.0f / (float)util::PIXELUNIT;

	this->m_sceneName = "Terrain Scene";

	/** Create Camera Object */

	this->m_mainCamera = new CCamera();
	m_mainCamera->m_cameraPosition = glm::vec3(10.0f, 0.0f, 0.0f);
	std::string str = "Resources/";
	std::wstring wstr(str.begin(), str.end());

	TerrainTrans.position = glm::vec3(0.0f, 0.0f, 0.0f);
	TerrainTrans.rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	TerrainTrans.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	SphereTrans.position = glm::vec3(0.0f, 0.0f, 0.0f);
	SphereTrans.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	SphereTrans.scale = glm::vec3(5.0f, 5.0f, 5.0f);

	//BackGround = new CBlocks(1);
	//BackGround->SetWorld(this);
	//BackGround->m_name = "BackGround";
	//BackGround->m_tag = "BackGround";
	//BackGround->m_transform.position = glm::vec3(0.0f, 0.0f,-1.0f);
	//BackGround->m_transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	//BackGround->m_transform.rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	//this->m_vGameObj.push_back(BackGround);
	//BackGround->GetComponent<CSpriteRender>()->SetSprite("Block");

	InitSphere();
	InitGeoShader();


	LoadHeightMap(wstr);
	smoothing();
	InitialisingVertices();
}

void CTerrainScene::ResetScene()
{
	__super::ResetScene();
}

void CTerrainScene::UpdateScene(float _tick)
{
	__super::UpdateScene(_tick);
	m_mainCamera->m_cameraFacing = SphereTrans.position;
	SphereTrans.position.y = CheckTerrainHeight(SphereTrans.position.x, SphereTrans.position.z) + 10.0f;
	m_mainCamera->m_cameraPosition.y = SphereTrans.position.y + 50.0f;
	m_mainCamera->m_cameraPosition.z = SphereTrans.position.z + 50.0f;
	m_mainCamera->m_cameraPosition.x = SphereTrans.position.x + 50.0f;

	GeoTrans.position = SphereTrans.position;
	GeoTrans.position.y = SphereTrans.position.y + 10.0f;

	//SphereTrans.position = m_mainCamera->m_cameraPosition + (m_mainCamera->m_cameraFacing * 20.0f);
	//BackGround->m_transform.position = m_mainCamera->m_cameraPosition + (m_mainCamera->m_cameraFacing);
	if (CInput::GetInstance()->g_cKeyState[unsigned char('w')] == INPUT_HOLD)
	{
		SphereTrans.position.z -= 0.5f;
	}
	else if (CInput::GetInstance()->g_cKeyState[unsigned char('s')] == INPUT_HOLD)
	{
		SphereTrans.position.z += 0.5f;
	}
	if (CInput::GetInstance()->g_cKeyState[unsigned char('a')] == INPUT_HOLD)
	{
		SphereTrans.position.x -= 0.5f;
	}
	else if (CInput::GetInstance()->g_cKeyState[unsigned char('d')] == INPUT_HOLD)
	{
		SphereTrans.position.x += 0.5f;
	}
}

void CTerrainScene::InitialisingVertices()
{
	GLfloat Verts[((256 * 256) * 3)];
	GLuint Indic[(256 - 1) * (256 - 1) * 6];
	int IndicePos = 0;
	for (int X = 0; X < 256; X++)
	{
		for (int Y = 0; Y < 256; Y++)
		{
			TerrainPointArray[X][Y].Position = glm::vec3(X * fVerticeGap, Y* fVerticeGap, TerrainPointArray[X][Y].Position.z);
			vertices.push_back(TerrainPointArray[X][Y].Position.x);
			vertices.push_back(TerrainPointArray[X][Y].Position.y);
			vertices.push_back(TerrainPointArray[X][Y].Position.z);
		}
	}
	for (int i = 0; i < vertices.size(); i++)
	{
		Verts[i] = vertices[i];
	}
	for (int X = 0; X < 255; X++)
	{
		for (int Y = 0; Y < 255; Y++)
		{
			Indic[IndicePos] = X * 256 + Y;
			Indic[IndicePos + 1] = X * 256 + Y + 1;
			Indic[IndicePos + 2] = (X + 1) * 256 + Y;

			Indic[IndicePos + 3] = (X + 1) * 256 + Y;
			Indic[IndicePos + 4] = X * 256 + Y + 1;
			Indic[IndicePos + 5] = (X + 1) * 256 + Y + 1;

			IndicePos += 6;
		}
	}

	GLuint EBO;

	glGenVertexArrays(1, &m_vaoT);
	glBindVertexArray(m_vaoT);

	glGenBuffers(1, &m_vboT);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indic), Indic, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	IndiceCount = sizeof(Indic) / sizeof(GLuint);

}

void CTerrainScene::InitSphere()
{
	fRadiusSphere = 1.0f;
	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float Spherevertices[(sections) * (sections)* vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			Spherevertices[offset] = x * fRadiusSphere;
			Spherevertices[offset + 1] = y * fRadiusSphere;
			Spherevertices[offset + 2] = z * fRadiusSphere;

			Spherevertices[offset + 3] = x;
			Spherevertices[offset + 4] = y;
			Spherevertices[offset + 5] = z;

			Spherevertices[offset + 6] = (float)i / (sections - 1);
			Spherevertices[offset + 7] = (float)j / (sections - 1);

			theta += (M_PI / (sections - 1));
			offset += 8;
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	GLuint Sphereindices[(sections) * (sections)* indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			Sphereindices[offset] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			Sphereindices[offset + 1] = (((i + 1) % sections) * sections) + (j);
			Sphereindices[offset + 2] = (i * sections) + (j);

			Sphereindices[offset + 3] = (i * sections) + ((j + 1) % sections);
			Sphereindices[offset + 4] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			Sphereindices[offset + 5] = (i * sections) + (j);

			offset += 6;
		}
	}
	offset = 0;
	GLuint EBO;

	glGenVertexArrays(1, &m_vaoS);
	glBindVertexArray(m_vaoS);

	glGenBuffers(1, &m_vboS);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Spherevertices), Spherevertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Sphereindices), Sphereindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	SphereIndiceCount = sizeof(Sphereindices) / sizeof(GLuint);
}

void CTerrainScene::InitGeoShader()
{
	GeoTrans.position = { 0.0f,0.0f,0.0f };
	GeoTrans.rotation = { 0.0f,0.0f,0.0f };
	GeoTrans.rotation = { 0.0f,0.0f,0.0f };

	GLfloat points[] = { 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

	glGenVertexArrays(1, &m_vaoG);
	glBindVertexArray(m_vaoG);

	glGenBuffers(1, &m_vboG);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);


}

void CTerrainScene::LoadHeightMap(std::wstring _fileName)
{
	std::vector<unsigned char> MapByteReader(256 * 256);
	// Open the file.
	std::ifstream inFile;
	inFile.open("Resources/HeightMap3.data", std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&MapByteReader[0], (std::streamsize)MapByteReader.size());
		// Done with file.
		inFile.close();
	}
	for (int X = 0; X < 256; X++)
	{
		for (int Y = 0; Y < 256; Y++)
		{
			TerrainPointArray[X][Y].Position.z = (float)MapByteReader[((X * 256) + Y)] * fHeightMod;
		}
	}
}

void CTerrainScene::LoadHeightMapPNG(std::wstring _fileName)
{


}

void CTerrainScene::smoothing()
{
	Vertex TempArray[256][256] = {};
	for (int X = 0; X < 256; X++)
	{
		for (int Y = 0; Y < 256; Y++)
		{
			TempArray[X][Y].Position.z = Average(X, Y);
		}
	}
	for (int X = 0; X < 256; X++)
	{
		for (int Y = 0; Y < 256; Y++)
		{
			TerrainPointArray[X][Y].Position.z = TempArray[X][Y].Position.z;
		}
	}
}

float CTerrainScene::Average(int x, int y)
{
	float average = 0.0f;
	float num = 0.0f;

	for (int m = x - 1; m <= x + 1; ++m)
	{
		for (int n = y - 1; n <= y + 1; ++n)
		{
			if (m < 256 && n < 256 && m >= 0 && n >= 0)
			{
				average += TerrainPointArray[m][n].Position.z;
				num += 1.0f;
			}
		}
	}

	return average / num;
}

float CTerrainScene::CheckTerrainHeight(float x, float y)
{
	float xDif = 0.0f;
	float yDif = 0.0f;
	xDif = ((x) - TerrainTrans.position.x);
	yDif = -((y) - TerrainTrans.position.z);
	if (xDif < 0 || yDif < 0)
	{
		return 100.0f;
	}

	int xPoint = (int)(xDif / fVerticeGap);
	int yPoint = (int)(yDif / fVerticeGap);

	float Height = 0.0f;
	float num = 0.0f;
	if (xPoint < 256 && yPoint < 256)
	{
		for (int m = xPoint; m <= xPoint + 1; ++m)
		{
			for (int n = yPoint; n <= yPoint + 1; ++n)
			{
				if (m < 256 && n < 256 && m >= 0 && n >= 0)
				{
					Height += TerrainPointArray[m][n].Position.z;
					num += 1.0f;
				}
			}
		}
		return Height / num;
	}

	return 100.0f;
}

void CTerrainScene::RenderTerrain()
{
	glUseProgram(CAssetMgr::GetInstance()->GetProgramID("TerrainProgram"));
	//glEnable(GL_BLEND);
	glm::vec3 position = TerrainTrans.position * (float)util::PIXELUNIT;
	glm::vec3 scale = glm::vec3(TerrainTrans.scale.x, TerrainTrans.scale.y, TerrainTrans.scale.z);
	glm::vec3 rotation = TerrainTrans.rotation;

	glm::mat4 objTranslate = glm::translate(glm::mat4(), position);
	glm::mat4 objScale = glm::scale(glm::mat4(), scale);
	glm::mat4 objRotation;// = glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	objRotation = glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //x
	objRotation = glm::rotate(objRotation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //y
	objRotation = glm::rotate(objRotation, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
	glm::mat4 model = objTranslate * objRotation * objScale;

	glm::mat4 MVP = m_mainCamera->GetProj() * m_mainCamera->GetView() * model;

	GLint MVPLoc = glGetUniformLocation(CAssetMgr::GetInstance()->GetProgramID("TerrainProgram"), "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(m_vaoT);
	glDrawElements(GL_TRIANGLES, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDisable(GL_BLEND);
	glUseProgram(0);
	glPolygonMode(GL_FRONT, GL_FILL);
}

void CTerrainScene::RenderSphere()
{
	glUseProgram(CAssetMgr::GetInstance()->GetProgramID("TerrainProgram"));

	glm::vec3 positionSphere = SphereTrans.position;
	glm::vec3 scaleSphere = glm::vec3(SphereTrans.scale.x, SphereTrans.scale.y, SphereTrans.scale.z);
	glm::vec3 rotationSphere = SphereTrans.rotation;


	glm::mat4 objTranslateSphere = glm::translate(glm::mat4(), positionSphere);
	glm::mat4 objScaleSphere = glm::scale(glm::mat4(), scaleSphere);
	glm::mat4 objRotationSphere;// = glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	objRotationSphere = glm::rotate(glm::mat4(), glm::radians(rotationSphere.x), glm::vec3(1.0f, 0.0f, 0.0f)); //x
	objRotationSphere = glm::rotate(objRotationSphere, glm::radians(rotationSphere.y), glm::vec3(0.0f, 1.0f, 0.0f)); //y
	objRotationSphere = glm::rotate(objRotationSphere, glm::radians(rotationSphere.z), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
	glm::mat4 modelSphere = objTranslateSphere * objRotationSphere * objScaleSphere;


	glm::mat4 MVP = m_mainCamera->GetProj() * m_mainCamera->GetView() * modelSphere;

	GLuint MVPLoc = glGetUniformLocation(CAssetMgr::GetInstance()->GetProgramID("TerrainProgram"), "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(m_vaoS);			  // Bind VAO
	glDrawElements(GL_TRIANGLES, SphereIndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);			  // Unbind VAO
	glUseProgram(0);
	//glPolygonMode(GL_FRONT, GL_FILL);
}

void CTerrainScene::RenderGeo()
{
	glUseProgram(CAssetMgr::GetInstance()->GetProgramID("GeoProgram"));

	glm::mat4 model; 
	model = glm::translate(model, GeoTrans.position);

	//GeoTrans.scale *= 50.0f;
	//glm::vec3 position = GeoTrans.position * (float)util::TERRAINUNITS;
	//glm::vec3 scale = glm::vec3(GeoTrans.scale.x, GeoTrans.scale.y, GeoTrans.scale.z);
	//glm::vec3 rotation = GeoTrans.rotation;

	//glm::mat4 objTranslate = glm::translate(glm::mat4(), position);
	//glm::mat4 objScale = glm::scale(glm::mat4(), scale);
	//glm::mat4 objRotation;// = glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	//objRotation = glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //x
	//objRotation = glm::rotate(objRotation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //y
	//objRotation = glm::rotate(objRotation, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //Z

	//model = objTranslate * objRotation * objScale;


	glm::mat4 mvp = m_mainCamera->GetProj() * m_mainCamera->GetView() * model;

	GLint mvpLoc = glGetUniformLocation(CAssetMgr::GetInstance()->GetProgramID("GeoProgram"), "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(m_vaoG); 
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
