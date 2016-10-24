#pragma once
#include "Vector4.h"
#include <string>
#include <iostream>
class Texture
{
public:
	Texture();
	Texture(int width, int height);
	~Texture();
	void LoadTexture(std::string name);
	Vector4 GetTexel(int x, int y);
private:
	int mWidth;
	int mHeight;
	int msize;
	
	Vector4* textureBuffer;
};

