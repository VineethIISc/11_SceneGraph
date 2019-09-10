#include "ArrowMesh.h"



ArrowMesh::ArrowMesh(): m_NumOfInstances(NUM_ARROWS), m_MBO(0)
{
	/* Nothing much job in this constructor*/
}

ArrowMesh::~ArrowMesh()
{
	// Delete the buffer object created
	if (m_MBO)
		glDeleteBuffers(1, &m_MBO);
}


void ArrowMesh::LoadPLYMesh(std::string filename)
{
	/*!
	1) Load ply mesh using the base class (PLY mesh) function

	2) Generate the transformation matrices

	3) Send the new data to buffer
	*/

	// Call the Ply loading of the mesh
	PLYMesh::LoadPLYMesh(filename);

	// Create transformation matrix buffer
	CreateTransformationMatrix();

	// Send the tranformation matrices to GPU
	SendDataToBuffer();
}

void ArrowMesh::CreateTransformationMatrix()
{
	/*!
	1) Create the matrices that are used for each instances 
	
	2) Push them into a vector 
	*/

	// local variable
	Matrix4f trans;
	trans.InitIdentity();					// create an identity matrix

	// Resize the vector to hold 3 matrices
	m_InstanceTransMat.reserve(NUM_ARROWS);

	/*!
	First matrix is to create x-axis arrow
	It has to be Identity
	*/
	m_InstanceTransMat.push_back(trans);	// Push the matrices


	/*!
	Second matrix is to create y-axis arrow
	It has rotate about z-axis by 90 deg in ccw
	*/
	trans.InitRotateTransform(0.0f, 0.0f, Y_ARROW_ROT_ANGLE);
	m_InstanceTransMat.push_back(trans);	// Push the matrices

	/*!
	Third matrix is to create z-axis arrow
	It has rotate about y-axis by 270 deg in ccw
	*/
	trans.InitRotateTransform(0.0f, Z_ARROW_ROT_ANGLE, 0.0f);
	m_InstanceTransMat.push_back(trans);	// Push the matrices

}

void ArrowMesh::SendDataToBuffer()
{
	/*!
	1) Generate the array buffer 

	2) Bind the buffer and vertex array object 

	3) Send the matrices to buffer

	4) Set the Divisor

	5) Reference: https://www.youtube.com/watch?v=yy2Zq-WkNqI
	*/

	// Create array objects
	glGenBuffers(1, &m_MBO);

	// Bind VAO, MBO
	glBindVertexArray(Mesh::m_VAO);						//use the Mesh class VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_MBO);

	// Size of matrices for three arrows
	GLsizeiptr MatrixSize = sizeof(Matrix4f) * NUM_ARROWS;

	// Allocate memory in GPU and copy the data to GPU
	glBufferData(GL_ARRAY_BUFFER, MatrixSize, m_InstanceTransMat[0], GL_STATIC_DRAW);

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

		// Disable attributes
		glDisableVertexAttribArray(attribIndex);
	}

}

void ArrowMesh::RenderMesh()
{
	/*!
	1) Draw the mesh using the instancing

	2) Reference: https://www.youtube.com/watch?v=yy2Zq-WkNqI
	*/

	// Bind the VAO
	glBindVertexArray(Mesh::m_VAO);

	// Render object using instancing
	glDrawElementsInstanced(Mesh::m_PrimitiveType, Mesh::m_NumOfIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(Mesh::m_IndexBufferOffset), m_NumOfInstances);

	// Unbind VAO
	glBindVertexArray(0);

}

