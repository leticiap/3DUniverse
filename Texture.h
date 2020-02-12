#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* filePath);

	bool LoadTexture();
	bool LoadTextureAlpha();

	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char * fileLocation;
};

