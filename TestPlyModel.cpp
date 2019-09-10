#include "ShaderInterface.h"
#include "TestPlyModel.h"

//create an instance of static member
Mesh* TestPlyModel::model = NULL;

TestPlyModel::TestPlyModel()
{
	//create an identity matrix
	Matrix4f Translate;
	Translate.InitTranslationTransform(0.0f, 10.0f, 0.0f);

	//create an instance of mesh for the body
	body = new SceneNode(model, ShaderInterface::m_BasicShader);
	body->SetLocalTransform(Translate);
	body->SetModelScale(Vector3f(0.1, 0.1, 0.1));
	addChild(body);	//set the current node as the child of this instance
}


void TestPlyModel::updateTranformation()
{
	Matrix4f Rot;
	Rot.InitRotateTransform(0.0f, 1.0f, 0.0f);

	//update the local transform
	m_Localtransform = m_Localtransform;

	// Update the child local
	//body->SetLocalTransform(body->GetLocalTransform() * Rot);

	//call the update function of base class
	SceneNode::updateTranformation();
}
