//! Light Node class
/*! 
This class represent the Light Node in the scene
*/

#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include "PLYMesh.h"
#include "SceneNode.h"

class Mesh;

class LightNode :	public SceneNode
{
public:
	//! Constructor
	LightNode();
	
	/*!
	No Destructor is required:
	Data is in tree node deleted at the end
	*/

	//! Get the position of Light
	Vector3f GetLightPosition() {	return m_LightPosition;	}

	//! Override the update function
	void updateTranformation() override;

	// Static member function to create the model only once
	static void CreateModel()
	{
		PLYMesh* m = new PLYMesh;
		m->LoadPLYMesh(MODEL_PATH"cube.ply");
		model = m;

	}

	//static delete function
	static void DeleteModel()
	{
		if (model)
		{
			delete model;
			model = NULL;
		}
	}

protected:
	static Mesh* model;

	SceneNode* body;

	// Location of Light
	Vector3f m_LightPosition;
};
#endif // !LIGHT_NODE_H



