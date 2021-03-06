#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies/glew.h"
#include "Dependencies/wglew.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char* filePath);
	GLuint CreateBmpTexture(char* filePath);

	void Test();
	void Lecture3();
	void Lecture3_Particle();
	void Lecture4();
	void Lecture4_Raindrop();
	void Lecture4_RadarCircle();
	void Lecture5();
	void Lecture5_FullRect();
	void Lecture6();
	void Lecture8();
	void Lecture9();
	void Lecture11(GLuint texID);
	void FBORender();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(std::string filename, std::string* target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(std::string filenameVS, std::string filenameFS);
	void CreateVertexBufferObjects();
	unsigned char* loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight);
	void CreateParticle(int count);
	void CreateLine(int count);
	void CreateTexture();
	void CreateDummyMesh();
	void CreateFBO();

	bool m_Initialized{ false };

	unsigned int m_WindowSizeX{ 0 };
	unsigned int m_WindowSizeY{ 0 };

	//camera position
	glm::vec3 m_v3Camera_Position;
	//camera lookat position
	glm::vec3 m_v3Camera_Lookat;
	//camera up vector
	glm::vec3 m_v3Camera_Up;

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4ProjView;

#pragma region LECTURE1
	GLuint m_VBORect{ 0 };
	GLuint m_SolidRectShader{ 0 };
#pragma endregion

#pragma region LECTURE2
	GLuint m_VBOLecture2{ 0 };
	GLuint m_VBOLecture3{ 0 };
	GLuint m_Lecture3Shader{ 0 };
#pragma endregion

#pragma region LECTURE3
	GLuint m_VBOSingleParticleQuad{ 0 };
	GLuint m_Lecture3ParticleShader{ 0 };
#pragma endregion

#pragma region LECTURE4
	GLuint m_VBOManyParticle{ 0 };
	GLuint m_VBOManyParticleVertexCount{ 0 };

	GLuint m_VBOLecture4{ 0 };
	GLuint m_Lecture4Shader{ 0 };
#pragma endregion

#pragma region LECTURE5
	GLuint m_VBOLineSegment{ 0 };
	GLuint m_VBOLineSegmentCount{ 0 };

	GLuint m_Lecture5Shader{ 0 };
	GLuint m_VBOFullRect{ 0 };
#pragma endregion

#pragma region LECTURE6
	GLuint m_Lecture6Shader{ 0 };
	GLuint m_VBOLecture6{ 0 };

	GLuint m_TexRGB{ 0 };
	GLuint m_TexChecker{ 0 };
#pragma endregion

#pragma region LECTURE8
	GLuint m_VBOLecture8{ 0 };
	GLuint m_Lecture8Shader{ 0 };
#pragma endregion

#pragma region LECTURE9
	GLuint m_VBOLecture9{ 0 };
	GLuint m_Lecture9Shader{ 0 };

	GLuint m_DummyVertexCount{ 0 };
#pragma endregion

#pragma region LECTURE11
	GLuint m_VBOLecture11{ 0 };
	GLuint m_Lecture11Shader{ 0 };

	GLuint m_FBOTexture[4];
	GLuint m_RenderBufferDepth[4];
	GLuint m_FBO[4];
#pragma endregion
};

