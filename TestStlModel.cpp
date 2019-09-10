#include "ShaderInterface.h"
#include "TestStlModel.h"

// Create an instance of static member
Mesh* TestStlModel::model = NULL;


TestStlModel::TestStlModel()
{

	//create an identity matrix
	Matrix4f Translate;
	Translate.InitTranslationTransform(20.0f, 0.0f, 0.0f);

	//create an instance of mesh for the body
	body = new SceneNode(model, ShaderInterface::m_BasicShader);
	body->SetLocalTransform(Translate);
	body->SetModelScale(Vector3f(0.1, 0.1, 0.1));
	addChild(body);	//set the current node as the child of this instance

}




void TestStlModel::updateTranformation()
{
	Matrix4f Rot, Scale;
	Rot.InitRotateTransform(0.0f, 0.0f, 2.0f);

	// Update the local transform
	m_Localtransform = m_Localtransform ;

	// Update the child local
	//body->SetLocalTransform( body->GetLocalTransform() * Rot);

	// Call the update function of base class
	SceneNode::updateTranformation();
}
