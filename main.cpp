#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"
#include "CollisionDetection.h"

Window mainWindow;
std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Camera* camera;

Texture textures[4];

int pyramidText1 = 0;
int pyramidText2 = 1;

Material shinyMaterial;
Material dullMaterial;

Model cat;
Model wand;

glm::vec3 catPosition;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLuint	uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

GLfloat catAngle = 0.0f;
bool catMove = true;

float cooldown = 0;

void HandleUserInput()
{
	// Get + Handle User Input
	glfwPollEvents();
	camera->KeyControl(mainWindow.getKeys(), deltaTime, catAngle);
	camera->MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

	if (cooldown <= 0)
	{
		if (mainWindow.getKeys()[GLFW_KEY_T])
		{
			spotLights[0].Toggle();
			if (catMove && CollisionDetection::Cat(camera->getCameraPosition(), catAngle, 3.5f))
			{
				catMove = false;
				cat = Model();
				cat.LoadModel("Material/Cat1.obj");
			}

			else if (CollisionDetection::Pyramid1(camera->getCameraPosition(), 3.0f))
			{
				// We must change from 0 to 1 and from 1 to 0, this is a easier way instead of keep checking "if value == 1"
				pyramidText1 = (pyramidText1 - 1) * -1;
			}

			else if (CollisionDetection::Pyramid2(camera->getCameraPosition(), 3.0f))
			{
				// We must change from 0 to 1 and from 1 to 0, this is a easier way instead of keep checking "if value == 1"
				pyramidText2 = (pyramidText2 - 1) * -1;
			}
			mainWindow.getKeys()[GLFW_KEY_L] = false;
			cooldown = 0.2f;
		}
	}
	else
	{
		cooldown -= deltaTime;
		if (cooldown <= 0)
		{
			spotLights[0].Toggle();
		}
	}

}

void CalcAverageNormals(unsigned int * indices, unsigned int indicesCount, GLfloat * vertices, unsigned int verticesCount, unsigned int vLenght, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicesCount; i+= 3)
	{
		unsigned int in0 = indices[i] * vLenght;
		unsigned int in1 = indices[i + 1] * vLenght;
		unsigned int in2 = indices[i + 2] * vLenght;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount/vLenght; i++)
	{
		unsigned int nOffset = i * vLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x    y      z		 u	   v		 nx	   ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{

	glm::mat4 model;

	//Pyramid 1 rendering
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, -7.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	textures[pyramidText1].UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	//Pyramid 2 rendering
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	textures[pyramidText2].UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	// Plane rendering
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	textures[3].UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();
	
	// Cat rendering
	if (catMove)
	{
		float newAngle = catAngle + 5.0f * deltaTime;
		if (newAngle > 360.0f)
			newAngle = 0.1f;
		if (!CollisionDetection::Cat(camera->getCameraPosition(), newAngle, 2.0f))
			catAngle = newAngle;
	}

	model = glm::mat4();
	model = glm::rotate(model, glm::radians(-catAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(10.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	textures[2].UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	cat.RenderModel();

	// Wand rendering
	model = glm::mat4();
	model = glm::translate(model, camera->getCameraPosition());
	model = glm::rotate(model, glm::radians(-camera->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(camera->getPitch()), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	textures[2].UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	wand.RenderModel();

}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	omniShadowShader.UseShader();
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, 1366, 768);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);

	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 wandLight = camera->getCameraPosition();
	wandLight.z += 0.5f;
	wandLight.y += 0.5f;
	spotLights[0].SetFlash(wandLight, camera->getCameraDirection());

	shaderList[0].Validate();
	RenderScene();
}

int main()
{
	mainWindow = Window(1024, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera::getInstance();

	textures[0] = Texture((char *) "Textures/brick.png");
	textures[0].LoadTextureAlpha();
	textures[1] = Texture((char *) "Textures/dirt.png");
	textures[1].LoadTextureAlpha();
	textures[2] = Texture((char *) "Textures/plain.png");
	textures[2].LoadTextureAlpha();
	textures[3] = Texture((char *) "Textures/floor.png");
	textures[3].LoadTextureAlpha();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	cat = Model();
	cat.LoadModel("Material/Cat2.obj");

	wand = Model();
	wand.LoadModel("Material/Wand.obj");

	mainLight = DirectionalLight(2048, 2048,
								1.0f, 0.8f, 0.5f,
								0.6f, 0.8f,
								10.0f, -12.0f, 18.5f);

	pointLights[0] = PointLight(1024, 1024, 
								0.01f, 100.0f,
								0.0f, 1.0f, 0.3f,
								0.2f, 1.0f,
								0.0f, 2.0f, 3.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	spotLights[0] = SpotLight(1024, 1024,
							0.1f, 100.0f,
							1.0f, 1.0f, 0.4f,
							0.0f, 2.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_negz.jpg");
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_posz.jpg");
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_posy.jpg");
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_negy.jpg");
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_posx.jpg");
	skyboxFaces.push_back("Textures/Skybox/DawnDusk_negx.jpg");

	skybox = Skybox(skyboxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		GLfloat timeNow = glfwGetTime();
		deltaTime = timeNow - lastTime;
		lastTime = timeNow;

		HandleUserInput();

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera->CalculateViewMatrix());

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}


	return 0;
}