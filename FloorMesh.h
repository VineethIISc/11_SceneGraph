//! Floor Plane class
/*!
Floor for our world
*/

#ifndef FLOOR_MESH_H
#define FLOOR_MESH_H

#include "Mesh.h"

//! Grid size by default
#define GRID_SIZE 1.0

//! Number of grid in row by default
#define NUM_CELL_ROW 10

//! Number of vertices for line
#define NUM_VERT_LINE 2

//! Number of instance
#define NUM_GRID_DIRECTION 2

//! Rotate about y-axis
#define Z_GRID_ROT_ANGLE 90

//! Number of rows in 4 X 4 matrix 
#define NUM_ROW_MAT4 4

//! Number of elements in a row of 4 X 4 matrix 
#define NUM_ELEMENT_ROW_MAT4 4


class FloorMesh :	public Mesh
{
public:
	//! Constructor
	FloorMesh(Vector4f color = Vector4f(0.5f, 0.5f, 0.5f, 1.0f));

	//! Overloaded Constructor
	FloorMesh(int numCellInRow, float gridSize, Vector4f color = Vector4f(0.5f, 0.5f, 0.5f, 1.0f));

	//! Destructor
	~FloorMesh() {};

	//! Override the render function in Mesh class
	void RenderMesh() override;

private:
	//*********************************
	//*********** variables ***********
	//*********************************
	// Buffer for matrices
	GLuint m_MBO;
	
	// Size of grid
	float m_GridSpace;

	// Number of grid in rows
	int m_NumCellInRow;

	// Color for the grid
	Vector4f m_GridColor;

	//! number of Instances
	GLuint m_NumOfInstances;

	//! tranformation matrices
	std::vector<Matrix4f> m_InstanceTransMat;

	//*********************************
	//*********** methods *************
	//*********************************
	//! Make Transformation Instances
	void CreateTransformationMatrix();

	//! Send Matrices to buffer
	void SendDataToBuffer();

	// Generate the grid mesh
	void InitGridMesh();

	// Populate the Vertical line vertices
	void PopulateVerticalLines(int numVertVert);

	// Transform the grid to center
	void TranformGrid();

};
#endif // !FLOOR_MESH_H

