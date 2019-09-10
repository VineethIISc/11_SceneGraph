//! Test ply model class
/*!
Test ply model
*/

#ifndef TEST_PLY_MODEL_H
#define TEST_PLY_MODEL_H

#include "PLYMesh.h"
#include "SceneNode.h"


class TestPlyModel : public SceneNode
{
public:
	TestPlyModel();

	/*!
	No Destructor is required:
	Data is in tree node deleted at the end
	*/
	
	//override the update function
	void updateTranformation() override;

	//static member function to create the model only once
	static void CreateModel()
	{
		PLYMesh* m = new PLYMesh;
		m->LoadPLYMesh(MODEL_PATH"table.ply");
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
};
#endif // !TEST_PLY_MODEL_H



