#include "FloorMesh.h"

void FloorMesh::RenderMesh()
{
	/*!
	1) Draw the mesh using the instancing

	2) Reference: https://www.youtube.com/watch?v=yy2Zq-WkNqI
	*/

	// Bind the VAO
	glBindVertexArray(Mesh::m_VAO);

	// Render object using instancing
	glDrawArraysInstanced(Mesh::m_PrimitiveType, 0, Mesh::m_NumOfVertices, this->m_NumOfInstances);

	// Unbind VAO
	glBindVertexArray(0);
}


FloorMesh::FloorMesh(Vector4f color) : m_GridSpace(GRID_SIZE), m_NumCellInRow(NUM_CELL_ROW),
m_GridColor(color)
{
	/*!
	1) Initialise Grid size in width, height

	2) Initialise number of grid in floor

	3) Generate grid Mesh
	*/

	// Compute the Mesh for floor
	InitGridMesh();

	// Number of instances
	this->m_NumOfInstances = NUM_GRID_DIRECTION;

	// Load the data to GPU
	Mesh::LoadData();

	// Create transformation matrix buffer
	CreateTransformationMatrix();

	// Send the tranformation matrices to GPU
	SendDataToBuffer();
}


FloorMesh::FloorMesh(int numCellInRow, float gridSize, Vector4f color) : m_GridSpace(gridSize),
m_NumCellInRow(numCellInRow), m_GridColor(color)
{
	/*!
	Argument number of grid division in rows and size of grid
	*/

	// Compute the Mesh for floor
	InitGridMesh();

	// Number of instances
	this->m_NumOfInstances = NUM_GRID_DIRECTION;

	// Load the data to GPU
	Mesh::LoadData();

	// Create transformation matrix buffer
	CreateTransformationMatrix();

	// Send the tranformation matrices to GPU
	SendDataToBuffer();
}






void FloorMesh::InitGridMesh()
{
	// Number of vertical lines: Number of cells plus one
	int numVertLines = (this->m_NumCellInRow + 1);

	// Total number of vertices for vertical lines
	Mesh::m_NumOfVertices = NUM_VERT_LINE * numVertLines;

	//// Reserve the space in vectors
	//Mesh::m_Vertices.reserve(Mesh::m_NumOfVertices);		// Vertices
	//Mesh::m_Normals.reserve(Mesh::m_NumOfVertices);			// Normals
	//Mesh::m_Colours.reserve(Mesh::m_NumOfVertices);			// Colors

	// Generate the vertical lines
	PopulateVerticalLines(numVertLines);

	// Shift the grid and color it
	TranformGrid();

	// Set the draw type to GL_Lines
	Mesh::m_PrimitiveType = GL_LINES;
}

void FloorMesh::PopulateVerticalLines(int numVertVert)
{
	// Compute the step size
	float rowStepSize = this->m_GridSpace / this->m_NumCellInRow;
	
	// Local variable
	Vector3f point1, point2;

	// Unit vector in x-axis
	Vector3f deltaX(1.0f, 0.0f, 0.0f);

	// Unit vector in z-axis
	Vector3f deltaZ(0.0f, 0.0f, this->m_GridSpace);

	// Vertical lines
	for (int i = 0; i < numVertVert; i++)
	{
		// Each vertices is fraction of unit x-axis
		point1 = deltaX * rowStepSize * i;
		point2 = point1 + deltaZ;

		// Store the vertices in Mesh class
		Mesh::PushPositionData(point1);
		Mesh::PushPositionData(point2);
	}
}

void FloorMesh::TranformGrid()
{
	// Shift the grid to center
	for (int i = 0; i < Mesh::m_NumOfVertices; i++)
	{
		Mesh::m_Vertices.m_Position[i] -= Vector3f(1.0, 0.0f, 1.0f) * (0.5f * this->m_GridSpace);

		if (fabsf(Mesh::m_Vertices.m_Position[i].x) < MINI_EPSILON)
			Mesh::m_Colours.push_back(Vector4f(1.0f, 0.0f, 0.0f, 1.0f));	//red color for x-axis
		else if (fabsf(Mesh::m_Vertices.m_Position[i].z) < MINI_EPSILON)
			Mesh::m_Colours.push_back(Vector4f(0.0f, 0.0f, 1.0f, 1.0f));	//blue color for z-axis
		else
			Mesh::m_Colours.push_back(this->m_GridColor);
	}
}

void FloorMesh::CreateTransformationMatrix()
{
	/*!
	1) Create the matrices that are used for each instances

	2) Push them into a vector
	*/

	// local variable
	Matrix4f trans;
	trans.InitIdentity();					// create an identity matrix

	// Resize the vector to hold 2 matrices
	m_InstanceTransMat.reserve(NUM_GRID_DIRECTION);

	/*!
	First matrix is to create normal direction grid lines
	It has to be Identity
	*/
	m_InstanceTransMat.push_back(trans);	// Push the matrices


	/*!
	Second Matrix is to draw the grid in another perpendicular direction
	It has rotate about y-axis by 90deg in ccw
	*/
	trans.InitRotateTransform(0.0f, Z_GRID_ROT_ANGLE, 0.0f);
	m_InstanceTransMat.push_back(trans);	// Push the matrices

}

void FloorMesh::SendDataToBuffer()
{

	/*!
	1) Generate the array buffer

	2) Bind the buffer and vertex array object

	3) Send the matrices to buffer

	4) Set the Divisor

	5) Reference: https://www.youtube.com/watch?v=yy2Zq-WkNqI
	*/

	// Create array objects
	glGenBuffers(1, &this->m_MBO);

	// Bind VAO, MBO
	glBindVertexArray(Mesh::m_VAO);						//use the Mesh class VAO
	glBindBuffer(GL_ARRAY_BUFFER, this->m_MBO);

	// Size of matrices for three arrows
	GLsizeiptr MatrixSize = sizeof(Matrix4f) * NUM_GRID_DIRECTION;

	// Allocate memory in GPU and copy the data to GPU
	glBufferData(GL_ARRAY_BUFFER, MatrixSize, this->m_InstanceTransMat[0], GL_STATIC_DRAW);

	// Assign attribute pointer 4 times for matrix4f (check the reference)
	GLintptr currBufferOffset = 0;		// Buffer offset
	GLuint attribIndex = 0;				// Attribute index
	for (unsigned int i = 0; i < NUM_ROW_MAT4; i++)
	{
		// Increment the attribute index by one
		attribIndex = MATRIX_ATTRIB + i;

		// Enable 4 attribute pointers
		glEnableVertexAttribArray(attribIndex);

		// Send 4 float at time (limitation in OpenGl), send mat4 as 4 times
		GLintptr stride = sizeof(Matrix4f);					// Matrix4f is the stride
		glVertexAttribPointer(attribIndex, NUM_ELEMENT_ROW_MAT4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(currBufferOffset));

		// Increment the offset by 4 floats
		currBufferOffset += NUM_ELEMENT_ROW_MAT4 * sizeof(float);

		// Divisor for instances
		glVertexAttribDivisor(attribIndex, 1);
	}

	// Unbind VAO, MBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Disable the attribute
	attribIndex = 0;				// Reuse the variable			
	for (unsigned int i = 0; i < NUM_ROW_MAT4; i++)
	{
		// Start with attribute 4 and increment by 1
		attribIndex = MATRIX_ATTRIB + i;
		glDisableVertexAttribArray(attribIndex);
	}
}
