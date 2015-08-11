/* Shader.cpp - Implements Shader operations, created with the help of http://www.swiftless.com */
#include "Shader.h"
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <iostream>

//static helper funktion - Read shader file
static char* textFileRead(const char *fileName) 
{
	char* text;
    
	if (fileName != NULL) {
        FILE *file;
		fopen_s(&file, fileName, "rt");
        
		if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

//static helper funktion - Look for shader validation errors
static void validateShader(GLuint shader, const char* file = 0) 
{
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		cout << "Shader " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
	}
}

//static helper funktion - Look for shader validation errors
static void validateProgram(GLuint program) 
{
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	memset(buffer, 0, BUFFER_SIZE);
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	if (length > 0)
		cout << "Program " << program << " link error: " << buffer << endl;
    
	glValidateProgram(program);
	GLint status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
		cout << "Error validating shader " << program << endl;
}

//Empty constructor
Shader::Shader() 
{
    
}

//Constructor. Takes shader filepaths and call initialization
Shader::Shader(const char *vsFile, /*const char *gsFile,*/ const char *fsFile) 
{
	init(vsFile, /*gsFile,*/ fsFile);
}

//Create, Compile and Attach shader
void Shader::init(const char *vsFile, /*const char *gsFile,*/ const char *fsFile) 
{
	//GLenum err = glewInit();
	//if (GLEW_OK != err){
	//	/* Problem: glewInit failed, something is seriously wrong. */
	//	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	//}

	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	//shader_gp = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

	// // Bind a constant attribute location for positions of vertices  
	//glBindAttribLocation(shader_id, 0, "in_Vertices"); // Bind another constant attribute location, this time for color  

	const char* vsText = textFileRead(vsFile);
	//const char* gsText = textFileRead(gsFile);
	const char* fsText = textFileRead(fsFile);	
    
    if (vsText == NULL || /*gsText == NULL ||*/ fsText == NULL) {
        cerr << "Either vertex or fragment shader file not found." << endl;
        return;
    }
    
	glShaderSource(shader_vp, 1, &vsText, 0);
	//glShaderSource(shader_gp, 1, &gsText, 0);
	glShaderSource(shader_fp, 1, &fsText, 0);
    
	glCompileShader(shader_vp);
	validateShader(shader_vp, vsFile);
	//glCompileShader(shader_gp);
	//validateShader(shader_gp, gsFile);
	glCompileShader(shader_fp);
	validateShader(shader_fp, fsFile);
    
	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	//glAttachShader(shader_id, shader_gp);
	glAttachShader(shader_id, shader_vp);

	/*glProgramParameteri(shader_id, GL_GEOMETRY_VERTICES_OUT, 4);
	glProgramParameteri(shader_id, GL_GEOMETRY_INPUT_TYPE, GL_POINTS);
	glProgramParameteri(shader_id, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP);*/
}

//Link shader program and get uniform locations
void Shader::LinkProg(){
	glLinkProgram(shader_id);
	validateProgram(shader_id);
}

void Shader::StoreUniformLocations()
{
	m_colorMapLocation = glGetUniformLocation(shader_id, "colorMap");
	m_texSizeLocation = glGetUniformLocation(shader_id, "texSize");
	m_uBlockGlobalMatricesIndex = glGetUniformBlockIndex(shader_id, "GlobalMatrices");
}

//Deteach and Delete shader in destructor
Shader::~Shader() {
	glDetachShader(shader_id, shader_fp);
	//glDetachShader(shader_id, shader_gp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	//glDeleteShader(shader_gp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

//Shader id property
unsigned int Shader::id() {
	return shader_id;
}

//Bind Shader for usage
void Shader::bind() {
	glUseProgram(shader_id);
}

//Undbind Shader after usage
void Shader::unbind() {
	glUseProgram(0);
}