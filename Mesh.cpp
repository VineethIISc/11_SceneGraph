#include "Mesh.h"



Mesh::Mesh(): m_VBO(0), m_IBO(0), m_TextureObjects(0), m_NumOfVertices(0), m_NumOfIndices(0),
m_NumOfFaces(0), m_IndexBufferOffset(0)
{
	/*!
	Initialise the primitive type to Triangles
	*/
	m_PrimitiveType = GL_TRIANGLES;	

}


Mesh::~Mesh()
{
	//delete the vertex array objects
	if(m_VAO)
		glDeleteVertexArrays(1, &m_VAO);

	//delete the buffer object created
	if(m_VBO)
		glDeleteBuffers(1, &m_VBO);	

	//delete the buffer object created
	if (m_IBO)
		glDeleteBuffers(1, &m_IBO);

	//delete the texture object
	if(m_TextureObjects)
		glDeleteTextures(1, &m_TextureObjects);
}

void Mesh::RenderMesh()
{
	//bind vertex array
	glBindVertexArray(m_VAO);

	// If indices is there draw using drawElements
	// Else use draw array
	if (!m_Indices.empty())
		glDrawElements(m_PrimitiveType, m_NumOfIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(m_IndexBufferOffset));
	else
		glDrawArrays(m_PrimitiveType, 0, m_NumOfVertices);

	//unbind the VAO
	glBindVertexArray(0);

}

int Mesh::GetNumberOfFaces() const
{
	return m_NumOfFaces;
}

void Mesh::PrintNumberOfFaces()
{
	std::cout << "number of Faces: " << m_NumOfFaces << std::endl;
}

int Mesh::GetNumberOfVertices() const
{
	return m_NumOfVertices;
}

void Mesh::PrintNumberOfVertices()
{
	std::cout << "number of Vertices: " << m_NumOfVertices << std::endl;
}

int Mesh::GetNumberOfIndices() const
{
	return m_NumOfIndices;
}

void Mesh::PrintNumberOfIndices()
{
	std::cout << "number of Indices: " << m_NumOfIndices << std::endl;
}

void Mesh::AssignBuffers(GLuint dataAttribLocation, GLint dataComponentsPerVert, GLuint dataSize, GLintptr dataOffset, const GLvoid* data )
{
	// Send data to GPU
	glBufferSubData(GL_ARRAY_BUFFER, dataOffset, dataSize, data);

	// Enable vertex attrib pointer 
	glEnableVertexAttribArray(dataAttribLocation);		// Position attribute location

	// Assign pointer to data location
	glVertexAttribPointer(dataAttribLocation, dataComponentsPerVert, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(dataOffset));
}

void Mesh::LoadData()
{
	/*!
	1) Generate vertex array object (VAO) and array buffer (VBO)
	2) Bind them
	3) Send following data to GPU
		a) vertices
		b) normals
		c) indices if any
		d) color if any
	4) Unbind them
	*/


	// Create vertex array objects
	glGenVertexArrays(1, &m_VAO);

	// Create buffer objects
	glGenBuffers(1, &m_VBO);

	// Bind VAO, VBO
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);			// Attach VBO to array buffer

	//allocate memory in GPU
	GLsizeiptr positionSize = sizeof(Vector3f) * this->m_NumOfVertices;
	GLsizeiptr normalSize = sizeof(Vector3f) * this->m_NumOfNormals;
	GLsizeiptr totalSize = positionSize + normalSize;
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

	// Push position data to GPU
	GLintptr offset = 0;
	
	// Position data of vertices
	AssignBuffers(POSITION_ATTRIB, POSITION_COORD_PER_VERTEX, positionSize, offset, this->m_Vertices.m_Position[0]);
	offset += positionSize;

	// If normal data is there
	if (this->m_NumOfNormals)
	{
		// Position data of vertices
		AssignBuffers(NORMAL_ATTRIB, NORMAL_COORD_PER_VERTEX, normalSize, offset, this->m_Vertices.m_Normals[0]);
		offset += normalSize;
	}

	// If normal data is there
	if (!m_Indices.empty())
	{
		// Create buffer objects
		glGenBuffers(1, &m_IBO);

		// Bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);	// Attach IBO to element array buffer for indices

		// Send the Data to GPU buffer
		GLsizeiptr indexSize = sizeof(Vector3ui) * m_Indices.size();
		m_IndexBufferOffset = offset = 0;							//reset the offset for IBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, m_Indices[0], GL_STATIC_DRAW);
		offset += indexSize;		
	}
		

	//unbind VAO, VBO, IBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(POSITION_ATTRIB);
}

void Mesh::PushPositionData(Vector3f& position)
{
	// Push the position of vertices to Vertex structure
	m_Vertices.m_Position.push_back(position);
}

void Mesh::PushNormalData(Vector3f& normal)
{
	// Push the normal of vertices to Vertex structure
	m_Vertices.m_Normals.push_back(normal);
}

void Mesh::PushIndicesData(Vector3ui& indices)
{
	// Push the position of vertices to Vertex structure
	m_Indices.push_back(indices);
}

void Mesh::ReserveMemoryPositionData()
{
	// Reserve the memory for position data
	m_Vertices.m_Position.reserve(m_NumOfVertices);

	// Reserve the memory for normal data
	m_Vertices.m_Normals.reserve(m_NumOfVertices);

	// If indices is there
	if (m_NumOfIndices)
	{
		// Reserve the memory for indices data
		m_Indices.reserve(m_NumOfIndices);
	}

}

void Mesh::ComputeVertexNormals()
{
	// Set the number of normals
	this->m_NumOfNormals = this->m_NumOfVertices;

	// Resize the normals with zero
	Vector3f zeroVal(0.0f, 0.0f, 0.0f);
	m_Vertices.m_Normals.resize(this->m_NumOfNormals, zeroVal);

	// Traverse through index list and find the normal to each face
	for (unsigned int i = 0; i < this->m_NumOfFaces; i++)
	{
		// Retrieve the indices of each faces from index list
		Vector3ui ind(m_Indices[i]);

		// Retrieve vertex position of face from vertex list
		Vector3f v1(m_Vertices.m_Position[ind.a]);		// first vertex
		Vector3f v2(m_Vertices.m_Position[ind.b]);		// second vertex
		Vector3f v3(m_Vertices.m_Position[ind.c]);		// third vertex

		// Compute two vector in the faces
		Vector3f p1(v2 - v1);
		Vector3f p2(v3 - v1);

		// Calculate the normal from cross product, then normalise it
		Vector3f n(p1.Cross(p2));
		n.Normalize();									// unit normal of face

		// Store the normal to all vertices to corresponsing indices
		m_Vertices.m_Normals[ind.a] += n;
		m_Vertices.m_Normals[ind.b] += n;
		m_Vertices.m_Normals[ind.c] += n;		
	}

	// Traverse through list of normals and normalize them
	for (unsigned int i = 0; i < this->m_NumOfNormals; i++)
	{
		// Make unit normals
		m_Vertices.m_Normals[i].Normalize();
	}
}





