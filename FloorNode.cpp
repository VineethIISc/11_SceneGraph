
#include "FloorMesh.h"

//! ShaderInterface class
#include "ShaderInterface.h"

#include "FloorNode.h"



FloorNode::FloorNode()
{
	// Create a mesh instance
	m_GridMesh = new FloorMesh(100, 5);

	// Set the floor mesh to current node
	SceneNode::setMesh(m_GridMesh);

	// Set the shader to current node
	SceneNode::setShader(ShaderInterface::m_InstanceShading);

	// Set the Scale of model
	SceneNode::SetModelScale(Vector3f(10.0f, 10.0f, 10.0f));
}

void FloorNode::updateTranformation()
{
	Matrix4f Rot, Scale;
	Rot.InitRotateTransform(0.0f, 0.0f, 2.0f);

	// Call the update function of base class
	SceneNode::updateTranformation();
}


