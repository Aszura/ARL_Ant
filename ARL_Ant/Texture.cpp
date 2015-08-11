#include "Texture.h"
#include <FreeImage.h>
#include <iostream>

Texture::Texture(const char* texturePath)
{
	//Get the order for the colors for the system
	int order = GL_BGRA;
	#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
	order = GL_RGBA;
	#endif

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(texturePath,0); 
	FIBITMAP* imagen = FreeImage_Load(formato, texturePath); 
	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);
	m_size.x = FreeImage_GetWidth(imagen); 
	m_size.y = FreeImage_GetHeight(imagen); 
	m_pixels = reinterpret_cast<char*>(FreeImage_GetBits(imagen));
	
	//Generate texture
	glGenTextures(1, &m_textureId);
	//Bind normal texture for storing image
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	//Use trilinear interpolation for minification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Use bilinear interpolation for magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Set anisotropic filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT); 
	//Load texture from pixels
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, order, GL_UNSIGNED_BYTE, m_pixels);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Generate mipmaps
	//glGenerateMipmap(GL_TEXTURE_2D);


	if(glGetError())
	{ 
		std::cout << "There was an error loading the texture: " << texturePath << std::endl; 
	}

	FreeImage_Unload(imagen);
}

Texture::~Texture()
{
}