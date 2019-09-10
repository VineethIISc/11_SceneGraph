#ifndef TEST_STL_MODEL_H
#define TEST_STL_MODEL_H

#include "STLMesh.h"
#include "SceneNode.h"

class Mesh;

class TestStlModel : public SceneNode
{
public:
	//! Constructor
	TestStlModel();
	
	/*!
	No Destructor is required:
	Data is in tree node deleted at the end
	*/

	//! Override the update function
	void updateTranformation() override;

	// Static member function to create the model only once
	static void CreateModel()
	{
		STLMesh* m = new STLMesh;
		m->LoadSTLMesh(MODEL_PATH"table.stl");
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
#endif // !TEST_STL_MODEL_H



