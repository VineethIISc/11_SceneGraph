//! Shader Class
/*!
	Base Class:
*/

#ifndef SHADER_H
#define SHADER_H

#include "CommonHeader.h"

#define SHADER_VERTEX 0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2

class Shader
{
public:
	Shader();	
	virtual ~Shader();

	//shader program id
	GLuint getProgramId();
	   
	//bind and unbind shader program
	void Bind();
	void Unbind();

	//assign texture variable 
	void setTextureVariable(const std::string& name, int value);

	//function overloading
	void setUniformVariable(const std::string& name, int value);
	void setUniformVariable(const std::string& name, float value);
	void setUniformVariable(const std::string& name, Vector3f& value);
	void setUniformVariable(const std::string& name, Vector4f& value);
	void setUniformVariable(const std::string& name, Matrix4f& value);

	//! Load the input shaders and return shader program id
	void LoadShaders(std::string vertex, std::string fragment, std::string geometry = "");

private:
	GLuint programID;
	//! static member for normal shader
	static Shader m_BasicShader;

	//! static member for instance shader
	static Shader m_InstanceShader;

	GLuint objects[3];
	
	bool loadFailed;

	//generateShaders from the source file
	GLuint generateShaders(std::string, GLenum shaderType);

	//link the shader program
	bool LinkProgram();

	//load shader from file
	bool LoadShaderFile(std::string from, std::string & into);
};
#endif

