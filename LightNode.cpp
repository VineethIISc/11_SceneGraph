// Shader Interface for assigning shader to node
#include "ShaderInterface.h"


#include "LightNode.h"

// Create an instance of static member
Mesh* LightNode::model = NULL;

LightNode::LightNode()
{

	// Set the mesh to current node
	SceneNode::setMesh(this->model);

	// Set the shader to current node
	SceneNode::setShader(ShaderInterface::m_BasicShader);

	// Set the Scale of model
	float scaleFactor = 0.05f;
	SceneNode::SetModelScale(Vector3f(scaleFactor, scaleFactor, scaleFactor));

	// Set the Initial Light location
	Matrix4f LightPosition;
	LightPosition.InitTranslationTransform(0.0f, 10.0f, 10.0f);
	SceneNode::SetLocalTransform(LightPosition);
}

void LightNode::updateTranformation()
{
	// Call the update function of base class
	SceneNode::updateTranformation();
}


