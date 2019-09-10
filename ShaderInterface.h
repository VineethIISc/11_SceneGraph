#ifndef SHADER_INTERFACE_H
#define SHADER_INTERFACE_H

#include "Shader.h"

class ShaderInterface 
{
public:
	ShaderInterface() {};
	~ShaderInterface() {};

	//! Static member to create the shader program id
	static void CreateBasicShaders()
	{
		Shader* s1 = new Shader;
		s1->LoadShaders(SHADER_PATH"vsBasicShader.txt", SHADER_PATH"fsBasicShader.txt");
		m_BasicShader = s1;
	}

	//! Static member to create the shader program id
	static void CreateInstanceShaders()
	{
		Shader* s2 = new Shader;
		s2->LoadShaders(SHADER_PATH"vsInstancedShader.txt", SHADER_PATH"fsInstancedShader.txt");
		m_InstanceShading = s2;
	}
	
	//! Delete the Basic Shader
	static void DeleteBasicShaders() 
	{	
		delete m_BasicShader;
		m_BasicShader = NULL;
	}

	//! Delete the instance Shader
	static void DeleteInstanceShaders() 
	{ 
		delete m_InstanceShading;
		m_InstanceShading = NULL;

	}

	//! basic shader
	static Shader* m_BasicShader;
	static Shader* m_InstanceShading;
};

#endif // !SHADER_INTERFACE_H



