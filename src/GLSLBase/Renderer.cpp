#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

float gTime{ 0.f };

float points[]
{
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
};

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	//default settings
	glClearDepth(1.f);

	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("Shaders/SolidRectV.glsl", "Shaders/SolidRectF.glsl");
	m_Lecture3Shader = CompileShaders("Shaders/Lecture3V.glsl", "Shaders/Lecture3F.glsl");
	m_Lecture3ParticleShader = CompileShaders("Shaders/Lecture3V_particle.glsl", "Shaders/Lecture3F_particle.glsl");
	m_Lecture4Shader = CompileShaders("Shaders/Lecture4V.glsl", "Shaders/Lecture4F.glsl");
	m_Lecture5Shader = CompileShaders("Shaders/Lecture5V.glsl", "Shaders/Lecture5F.glsl");
	m_Lecture6Shader = CompileShaders("Shaders/Lecture6V.glsl", "Shaders/Lecture6F.glsl");
	m_Lecture8Shader = CompileShaders("Shaders/Lecture8V.glsl", "Shaders/Lecture8F.glsl");
	m_Lecture9Shader = CompileShaders("Shaders/Lecture9V.glsl", "Shaders/Lecture9F.glsl");
	m_Lecture11Shader = CompileShaders("Shaders/Lecture11V.glsl", "Shaders/Lecture11F.glsl");

	//Create VBOs
	CreateVertexBufferObjects();
	//Create Particles
	CreateParticle(1000);
	//Create Line
	CreateLine(2000);
	//Create Texture
	CreateTexture();
	//Load Texture
	m_TexRGB = CreatePngTexture(const_cast<char*>("rgb.png"));
	//Create Dummy Mesh
	CreateDummyMesh();
	//Create FBO
	CreateFBO();

	//Initialize camera settings
	m_v3Camera_Position = glm::vec3(0.f, 0.f, 1000.f);
	m_v3Camera_Lookat = glm::vec3(0.f, 0.f, 0.f);
	m_v3Camera_Up = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		m_v3Camera_Position,
		m_v3Camera_Lookat,
		m_v3Camera_Up
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		-static_cast<float>(windowSizeX) / 2.f, static_cast<float>(windowSizeX) / 2.f,
		-static_cast<float>(windowSizeY) / 2.f, static_cast<float>(windowSizeY) / 2.f,
		0.0001f, 10000.f);
	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

void Renderer::CreateVertexBufferObjects()
{
	GLfloat rect[] =
	{
		//Triangle1
		-0.5, -0.5, 0.f,
		-0.5, 0.5, 0.f,
		0.5, 0.5, 0.f,

		//Triangle2
		-0.5, -0.5, 0.f,
		0.5, 0.5, 0.f,
		0.5, -0.5, 0.f,
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

#pragma region LECTURE2
	GLfloat lecture2[] =
	{
		// vertices
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
	};

	glGenBuffers(1, &m_VBOLecture2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture2), lecture2, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE3
	GLfloat lecture3[] =
	{
		// vertices
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,

		// color
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
	}; // 21

	glGenBuffers(1, &m_VBOLecture3);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3), lecture3, GL_STATIC_DRAW);

	GLfloat particleSize{ 0.1f };
	GLfloat lecture3_singleParticle[] =
	{
		// vertices
		-particleSize, -particleSize,
		 particleSize,  particleSize,
		-particleSize,  particleSize,

		-particleSize, -particleSize,
		 particleSize, -particleSize,
		 particleSize,  particleSize,

		 // color
		 1.f, 1.f, 1.f, 1.f,
		 1.f, 1.f, 1.f, 1.f,
		 1.f, 1.f, 1.f, 1.f,

		 1.f, 1.f, 1.f, 1.f,
		 1.f, 1.f, 1.f, 1.f,
		 1.f, 1.f, 1.f, 1.f,
	}; // 21

	glGenBuffers(1, &m_VBOSingleParticleQuad);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3_singleParticle), lecture3_singleParticle, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE4
	GLfloat rectSize{ 0.5f };
	GLfloat lecture4[] =
	{
		// vertices				   // color
		-rectSize, -rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,
		 rectSize,  rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,
		-rectSize,  rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,

		-rectSize, -rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,
		 rectSize, -rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,
		 rectSize,  rectSize, 0.f, 1.f, 1.f, 1.f, 1.f,
	}; // 21

	glGenBuffers(1, &m_VBOLecture4);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4), lecture4, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE5
	rectSize = 1.f;
	float lecture5_fullRect[]
	{
		-rectSize, -rectSize, 0.0f,
		 rectSize,  rectSize, 0.0f,
		-rectSize,  rectSize, 0.0f,
		-rectSize, -rectSize, 0.0f,
		 rectSize, -rectSize, 0.0f,
		 rectSize,  rectSize, 0.0f,
	};

	glGenBuffers(1, &m_VBOFullRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture5_fullRect), lecture5_fullRect, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE6
	rectSize = 0.5f;
	float lecture6_fullRect[]
	{
		-rectSize, -rectSize, 0.0f, 0.0f, 0.0f,
		 rectSize,  rectSize, 0.0f, 1.0f, 1.0f,
		-rectSize,  rectSize, 0.0f, 0.0f, 1.0f,
		-rectSize, -rectSize, 0.0f, 0.0f, 0.0f,
		 rectSize, -rectSize, 0.0f, 1.0f, 0.0f,
		 rectSize,  rectSize, 0.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &m_VBOLecture6);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture6_fullRect), lecture6_fullRect, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE8
	rectSize = 0.5f;
	float lecture8_fullRect[]
	{
		-rectSize, -rectSize, 0.0f, 0.0f, 0.0f,
		 rectSize,  rectSize, 0.0f, 1.0f, 1.0f,
		-rectSize,  rectSize, 0.0f, 0.0f, 1.0f,
		-rectSize, -rectSize, 0.0f, 0.0f, 0.0f,
		 rectSize, -rectSize, 0.0f, 1.0f, 0.0f,
		 rectSize,  rectSize, 0.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &m_VBOLecture8);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture6_fullRect), lecture6_fullRect, GL_STATIC_DRAW);
#pragma endregion

#pragma region LECTURE11
	rectSize = 1.f;
	float lecture11_fullRect[]
	{
		-rectSize, -rectSize, 0.0f,
		 rectSize,  rectSize, 0.0f,
		-rectSize,  rectSize, 0.0f,
		-rectSize, -rectSize, 0.0f,
		 rectSize, -rectSize, 0.0f,
		 rectSize,  rectSize, 0.0f,
	};

	glGenBuffers(1, &m_VBOLecture11);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture11_fullRect), lecture11_fullRect, GL_STATIC_DRAW);
#pragma endregion
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//????????? ???????????? ??????
	GLuint ShaderObj{ glCreateShader(ShaderType) };

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//????????? ????????? ????????? ??????????????? ??????
	glShaderSource(ShaderObj, 1, p, Lengths);

	//????????? ????????? ????????? ?????????
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj ??? ??????????????? ????????? ???????????? ??????
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL ??? shader log ???????????? ?????????
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram ??? attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(std::string filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(std::string filenameVS, std::string filenameFS)
{
	GLuint ShaderProgram{ glCreateProgram() }; //??? ????????? ???????????? ??????

	if (ShaderProgram == 0) { //????????? ??????????????? ?????????????????? ??????
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shaderV.gla??? vs ????????? ?????????
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shaderF.glsl??? fs ????????? ?????????
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram ??? vs.c_str() ????????? ???????????? ???????????? ????????? attach???
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram ??? fs.c_str() ??????????????? ???????????? ???????????? ????????? attach???
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success{ 0 };
	GLchar ErrorLog[1024] = { 0 };

	//Attach ????????? shaderProgram ??? ?????????
	glLinkProgram(ShaderProgram);

	//????????? ??????????????? ??????
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program ????????? ?????????
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char* Renderer::loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE* file{ nullptr };
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char* filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char* filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char* bmp{ loadBMPRaw(filePath, width, height) };

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::CreateParticle(int count)
{
	//(x, y, z, vx, vy, vz, emit_time, life_time, amp, period, value, r, g, b, a)
	// v = ??????, emit_time = ????????? ??????, life_time = ????????? ?????? ??????
	// amp = ?????? ???, period = ??????, value = ???
	// r, g, b, a = ??????
	int floatCount = count * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4) * 3 * 2;
	float* particleVertices = new float[floatCount];
	m_VBOManyParticleVertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.1f;

	for (int i = 0; i < count; ++i)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;
		float randomAmp = 0.f;
		float randomPeriod = 0.f;
		float randomValue = 0.f;
		float r, g, b, a = 0.f;

		randomValueX = 0.f;// ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueY = 0.f;// ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueZ = 0.f;
		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f; //-1~1
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f; //-1~1
		randomValueVZ = 0.f;
		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomAmp = ((float)rand() / (float)RAND_MAX) * 0.4f - 0.2f;
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f;
		randomValue = ((float)rand() / (float)RAND_MAX) * 1.f;
		r = ((float)rand() / (float)RAND_MAX) * 1.f;
		g = ((float)rand() / (float)RAND_MAX) * 1.f;
		b = ((float)rand() / (float)RAND_MAX) * 1.f;
		a = 0.f;

		//v0
		//Position XYZ
		particleVertices[index++] = -particleSize / 2.f + randomValueX;
		particleVertices[index++] = -particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;

		//v1
		//Position XYZ
		particleVertices[index++] = particleSize / 2.f + randomValueX;
		particleVertices[index++] = -particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;

		//v2
		//Position XYZ
		particleVertices[index++] = particleSize / 2.f + randomValueX;
		particleVertices[index++] = particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;

		//v3
		//Position XYZ
		particleVertices[index++] = -particleSize / 2.f + randomValueX;
		particleVertices[index++] = -particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;

		//v4
		//Position XYZ
		particleVertices[index++] = particleSize / 2.f + randomValueX;
		particleVertices[index++] = particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;

		//v5
		//Position XYZ
		particleVertices[index++] = -particleSize / 2.f + randomValueX;
		particleVertices[index++] = particleSize / 2.f + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		//Emit Time
		particleVertices[index++] = randomEmitTime;
		//Life Time
		particleVertices[index++] = randomLifeTime;
		// Amp
		particleVertices[index++] = randomAmp;
		// Period
		particleVertices[index++] = randomPeriod;
		// Value
		particleVertices[index++] = randomValue;
		// Color
		particleVertices[index++] = r;
		particleVertices[index++] = g;
		particleVertices[index++] = b;
		particleVertices[index++] = a;
	}

	glGenBuffers(1, &m_VBOManyParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);

	delete[] particleVertices;
}

void Renderer::CreateLine(int count)
{
	int floatCount{ count * 3 };
	float* lineVertices{ new float[floatCount] };
	m_VBOLineSegmentCount = count;
	int index{ 0 };

	for (int i = 0; i < count; ++i)
	{
		lineVertices[index++] = -1.f + i * 2.f / (count - 1);
		lineVertices[index++] = 0.f;
		lineVertices[index++] = 0.f;
	}

	glGenBuffers(1, &m_VBOLineSegment);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineSegment);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, lineVertices, GL_STATIC_DRAW);

	delete[] lineVertices;
}

void Renderer::CreateTexture()
{
	static const GLulong checkerBoard[] =
	{
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};

	glGenTextures(1, &m_TexChecker);
	glBindTexture(GL_TEXTURE_2D, m_TexChecker);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerBoard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::CreateDummyMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_DummyVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; ++x)
	{
		for (int y = 0; y < pointCountY; ++y)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;

	for (int x = 0; x < pointCountX - 1; ++x)
	{
		for (int y = 0; y < pointCountY - 1; ++y)
		{
			//Triangle part 1
			vertices[vertIndex++] = point[(y * pointCountX + x) * 2 + 0];
			vertices[vertIndex++] = point[(y * pointCountX + x) * 2 + 1];
			vertices[vertIndex++] = 0.f;

			vertices[vertIndex++] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertices[vertIndex++] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertices[vertIndex++] = 0.f;

			vertices[vertIndex++] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertices[vertIndex++] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertices[vertIndex++] = 0.f;

			//Triangle part 2
			vertices[vertIndex++] = point[(y * pointCountX + x) * 2 + 0];
			vertices[vertIndex++] = point[(y * pointCountX + x) * 2 + 1];
			vertices[vertIndex++] = 0.f;

			vertices[vertIndex++] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertices[vertIndex++] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertices[vertIndex++] = 0.f;

			vertices[vertIndex++] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertices[vertIndex++] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertices[vertIndex++] = 0.f;
		}
	}

	glGenBuffers(1, &m_VBOLecture9);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::CreateFBO()
{
	glGenTextures(4, m_FBOTexture);
	glGenRenderbuffers(4, m_RenderBufferDepth);
	glGenFramebuffers(4, m_FBO);

	for (int i = 0; i < 4; ++i)
	{
		// Color Buffer
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Depth Buffer
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferDepth[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[i]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture[i], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferDepth[i]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << " gen FBO error on FBO[" << i << "]\n";
		}

		// FBO ????????? = main frame buffer??? ?????? ???????????? ??????
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition{ glGetAttribLocation(m_SolidRectShader, "a_Position") };
	glEnableVertexAttribArray(attribPosition);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3()
{
	glUseProgram(m_Lecture3Shader);

	int attribPosition{ glGetAttribLocation(m_Lecture3Shader, "a_Position") };
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribColor{ glGetAttribLocation(m_Lecture3Shader, "inColor") };
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(float) * 9));

	GLint uniPosTime{ glGetUniformLocation(m_Lecture3Shader, "uTime") };
	glUniform1f(uniPosTime, gTime);

	GLint uniPosColor{ glGetUniformLocation(m_Lecture3Shader, "uColor") };
	glUniform4f(uniPosColor, 1.f, 0.f, 0.f, 1.f);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	gTime -= 0.01f;

	if (gTime < 0.f)
	{
		gTime = 1.f;
	}

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_Particle()
{
	auto shader{ m_Lecture3ParticleShader };

	glUseProgram(shader);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// ???????????? ??? ?????????

	// ????????? VBO??? ????????? ?????? ??????????????? Bind ??? ????????? ????????? ?????? 1?????? ????????? ??????
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);

	// per vertex
	int attribPosition{ glGetAttribLocation(shader, "a_Position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);

	int attribVelocity{ glGetAttribLocation(shader, "velocity") };
	glEnableVertexAttribArray(attribVelocity);
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

	int attribEmitTime{ glGetAttribLocation(shader, "emit_time") };
	glEnableVertexAttribArray(attribEmitTime);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 6));

	int attribLifeTime{ glGetAttribLocation(shader, "life_time") };
	glEnableVertexAttribArray(attribLifeTime);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 7));

	int attribAmp{ glGetAttribLocation(shader, "amp") };
	glEnableVertexAttribArray(attribAmp);
	glVertexAttribPointer(attribAmp, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 8));

	int attribPeriod{ glGetAttribLocation(shader, "period") };
	glEnableVertexAttribArray(attribPeriod);
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 9));

	int attribValue{ glGetAttribLocation(shader, "value") };
	glEnableVertexAttribArray(attribValue);
	glVertexAttribPointer(attribValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 10));

	int attribColor{ glGetAttribLocation(shader, "in_color") };
	glEnableVertexAttribArray(attribColor);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, reinterpret_cast<GLvoid*>(sizeof(float) * 11));

	// per shader
	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);

	int uniformAccel{ glGetUniformLocation(shader, "accel") };
	glUniform3f(uniformAccel, 0.f, 0.f, 0.f);

	gTime += 0.01f;

	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribVelocity);
	glDisableVertexAttribArray(attribEmitTime);
	glDisableVertexAttribArray(attribLifeTime);
	glDisableVertexAttribArray(attribPeriod);
	glDisableVertexAttribArray(attribValue);
	glDisableVertexAttribArray(attribColor);

	glDisable(GL_BLEND);
}

void Renderer::Lecture4()
{
	auto shader{ m_Lecture4Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture4);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture4_Raindrop()
{
	auto shader{ m_Lecture4Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture4);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int uniformPoints{ glGetUniformLocation(shader, "points") };
	glUniform3fv(uniformPoints, 10, points);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);
	gTime += 0.001f;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture4_RadarCircle()
{
	auto shader{ m_Lecture4Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture4);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int uniformPoints{ glGetUniformLocation(shader, "points") };
	glUniform3fv(uniformPoints, 10, points);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);

	gTime += 0.001f;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture5()
{
	auto shader{ m_Lecture5Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineSegment);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);

	gTime += 0.1f;

	glDrawArrays(GL_LINE_STRIP, 0, m_VBOLineSegmentCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture5_FullRect()
{
	auto shader{ m_Lecture5Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullRect);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);

	gTime += 0.01f;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture6()
{
	auto shader{ m_Lecture6Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture6);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	int attribTex{ glGetAttribLocation(shader, "inTexPos") };
	glEnableVertexAttribArray(attribTex);
	glVertexAttribPointer(attribTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

	int uniformTexSampler{ glGetUniformLocation(shader, "texSampler") };
	glUniform1i(uniformTexSampler, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexRGB);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTex);
}

void Renderer::Lecture8()
{
	auto shader{ m_Lecture8Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture8);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	int attribTex{ glGetAttribLocation(shader, "inTexPos") };
	glEnableVertexAttribArray(attribTex);
	glVertexAttribPointer(attribTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

	int uniformTexSampler{ glGetUniformLocation(shader, "texSampler[0]") };
	glUniform1i(uniformTexSampler, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexRGB);

	int uniformTexSampler1{ glGetUniformLocation(shader, "texSampler[1]") };
	glUniform1i(uniformTexSampler1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TexChecker);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);
	gTime += 0.01f;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTex);
}

void Renderer::Lecture9()
{
	auto shader{ m_Lecture9Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture9);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int uniformTime{ glGetUniformLocation(shader, "time") };
	glUniform1f(uniformTime, gTime);
	gTime += 0.05f;

	glDrawArrays(GL_LINE_STRIP, 0, m_DummyVertexCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture11(GLuint texID)
{
	auto shader{ m_Lecture11Shader };

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture11);

	int attribPosition{ glGetAttribLocation(shader, "position") };
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	int uniformSampler{ glGetUniformLocation(shader, "texSampler") };
	glUniform1f(uniformSampler, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::FBORender()
{
	// bind fbo
	// set viewport
	// clear
	// render
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[0]);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture3_Particle();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[1]);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture4_Raindrop();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[2]);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture4_RadarCircle();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[3]);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Lecture9();

	// restore frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// draw texture
	glViewport(0, 0, 256, 256);
	Lecture11(m_FBOTexture[0]);

	glViewport(256, 0, 256, 256);
	Lecture11(m_FBOTexture[1]);

	glViewport(0, 256, 256, 256);
	Lecture11(m_FBOTexture[2]);

	glViewport(256, 256, 256, 256);
	Lecture11(m_FBOTexture[3]);
}
