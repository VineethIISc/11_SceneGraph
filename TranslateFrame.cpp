#include "ShaderInterface.h"
#include "TranslateFrame.h"

Mesh * TranslateFrame::m_TranslateMesh = NULL;

TranslateFrame::TranslateFrame()
{

	// Create an identity matrix
	Matrix4f Translate;
	//Translate.InitTranslationTransform(0.0, 0.0, 0.0f);
	Translate.InitAxisRotateTransform(Vector3f(0.0f, 0.0f, 1.0f), 90.0f);


	/*!
	1)Create a sceneNode and assign as the child of this instance		
	*/
	first = new SceneNode(m_TranslateMesh, ShaderInterface::m_InstanceShading);
	first->SetModelScale(Vector3f(0.1, 0.1, 0.1));
	first->SetLocalTransform(Translate);
	/*!
	2)add the first as child of the current instance
	*/
	addChild(first);
}


void TranslateFrame::updateTranformation()
{
	Matrix4f Rot;
	Rot.InitRotateTransform(0.0f, 0.0f, 0.0f);

	//update the local transform
	m_Localtransform = m_Localtransform * Rot;

	//call the update function of base class
	SceneNode::updateTranformation();
}






