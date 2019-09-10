//! Translate Frame class
/*!
This class is a frame node for each object
*/

#ifndef TRANSLATE_FRAME_H
#define TRANSLATE_FRAME_H

#include "SceneNode.h"
#include "ArrowMesh.h"
#include "Shader.h"


class TranslateFrame : public SceneNode
{
public:	
	//! Constructor 
	TranslateFrame(); 

	/*!
	No Destructor is required:
	Data is in tree node deleted at the end
	*/

	//! Override the update function
	void updateTranformation() override;

	//! Static member function
	/*!
	Create the mesh and assign to static private member m_TranslateMesh
	*/
	static void CreateTranslateMesh()
	{
		ArrowMesh* m = new ArrowMesh;		//!< instantiating
		m->LoadPLYMesh(MODEL_PATH"arrow.ply");	//!< Load the arrow ply
		m_TranslateMesh = m;			//!< Assign to static member

	}

	//! static member function
	/*!
	Delete the static private member m_TranslateMesh
	*/
	static void DeleteTranslateMesh()
	{
		delete m_TranslateMesh;
		m_TranslateMesh = NULL;

	}

	  	

private:
	//! Mesh for frame
	/*!
	Create static mesh so it cna be used for many drawing.
	No need to add Mesh class here becauz SceneNode is contain it
	*/	
	static Mesh* m_TranslateMesh;	

	//! Local variable
	SceneNode* first;

	
};
#endif // !TRANSLATE_FRAME_H



