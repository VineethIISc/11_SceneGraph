//! Arrow mesh
/*!
Arrow mesh inhertis from the Mesh class
1) Override the render method of Mesh class for instance drawing

2) Refernce:
	a) For instancing: https://www.youtube.com/watch?v=yy2Zq-WkNqI
*/

#ifndef ARROW_MESH_H
#define ARROW_MESH_H

#include "PLYMesh.h"

//! Number of arrow make the frame
#define NUM_ARROWS 3	

//! Arrow rotate by 90 degree about z-axis
#define Y_ARROW_ROT_ANGLE 90.0f

//! Arrow rotate by 2700 degree about y-axis
#define Z_ARROW_ROT_ANGLE 270.0f

//! Number of rows in 4 X 4 matrix 
#define NUM_ROW_MAT4 4

//! Number of elements in a row of 4 X 4 matrix 
#define NUM_ELEMENT_ROW_MAT4 4

//! Number of transformations
#define NUM_TRANS_MAT 3


class ArrowMesh : public PLYMesh
{
public:
	//! Constructor
	ArrowMesh();

	//! Destructor
	~ArrowMesh();

	//! Method to load ply file override from PLYMesh class
	void LoadPLYMesh(std::string filename) override;

	//! Override the render function in Mesh class
	void RenderMesh() override;


private:
	//*********************************
	//*********** variables ***********
	//*********************************
	// Buffer for matrices
	GLuint m_MBO;

	// number of Instances
	GLuint m_NumOfInstances;

	// tranformation matrices
	std::vector<Matrix4f> m_InstanceTransMat;


	//*********************************
	//*********** methods *************
	//*********************************
	// Make Transformation Instances
	void CreateTransformationMatrix();

	// Send Matrices to buffer
	void SendDataToBuffer();
};
#endif // !ARROW_MESH_H



