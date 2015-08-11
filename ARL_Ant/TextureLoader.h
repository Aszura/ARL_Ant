#pragma once

#include <map>

class Texture;

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	Texture* Load(const char* texturePath);
private:
	std::map<const char*, Texture*> m_textures;
};