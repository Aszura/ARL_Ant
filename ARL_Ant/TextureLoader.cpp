#include "TextureLoader.h"
#include "Texture.h"

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
	for(std::map<const char*, Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}
	m_textures.clear();
}

Texture* TextureLoader::Load(const char* texturePath)
{
	if(m_textures.find(texturePath) != m_textures.end())
	{
		return m_textures[texturePath];
	}
	else
	{
		m_textures[texturePath] = new Texture(texturePath);
		return m_textures[texturePath];
	}
}