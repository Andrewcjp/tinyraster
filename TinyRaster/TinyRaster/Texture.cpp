include "Texture.h"



Texture::Texture(int width, int height)
{
	msize = width*height;
	mWidth = width;
	mHeight = height;

	textureBuffer = new Vector4[msize];
}


Texture::~Texture()
{
}

void Texture::LoadTexture(std::string name)
{
	//debug init to white
	for (int i = 0; i < msize; i++) {
		textureBuffer[i] = Vector4(1, 1, 1, 1);
	}
}

Vector4 Texture::GetTexel(int x, int y)
{
	if ((y*mWidth + x) < ((y + 1) * mWidth) && (y*mWidth + x) > (y*mWidth) && x > 0 && y > 0) {
		if ((y*mWidth + x) < mWidth*mHeight && (y*mWidth + x) > 0) {
			return textureBuffer[y*mWidth + x];
		}
	}
}

