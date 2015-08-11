/* Shader.h - Definitions for shader loading and compilation */
#pragma once

using namespace std;

class Shader
{
public:
	//Empty constructor
	Shader();

	//Constructor. Takes shader filepaths and call initialization
	Shader(const char *vsFile, /*const char *gsFile,*/ const char *fsFile);

	//Deteach and Delete shader in destructor
	~Shader();
	
	//Create, Compile and Attach shader
    void init(const char *vsFile, /*const char *gsFile,*/ const char *fsFile);

	//Link shader program
    void LinkProg();

	void StoreUniformLocations();

	//Bind Shader for usage
	void bind();

	//Undbind Shader after usage
	void unbind();
	
	//Shader id property
	unsigned int id();

	unsigned int getUBlockGlobalMatricesIndex() const
	{
		return m_uBlockGlobalMatricesIndex;
	}
	unsigned int getColorMapLocation() const
	{
		return m_colorMapLocation;
	}
	unsigned int getTexSizeLocation() const
	{
		return m_texSizeLocation;
	}
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	//unsigned int shader_gp;
	unsigned int shader_fp;

	unsigned int m_uBlockGlobalMatricesIndex;
	unsigned int m_colorMapLocation;
	unsigned int m_texSizeLocation;
};