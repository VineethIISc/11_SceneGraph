/*
reference: https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/
*/

#ifndef MESH_H
#define MESH_H

#include "CommonHeader.h"

//! 3D Position is represented with 3 coordinate
#define POSITION_COORD_PER_VERTEX 3

//! Normal is represented with 3 coordinate
#define NORMAL_COORD_PER_VERTEX 3

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();	//virtual destructor	//reference https://www.youtube.com/watch?v=DJS9eSN4jAc
	
	virtual void RenderMesh();	//rendering function

	/*!
	Getters are written to access the private member
	Shouldnot change them so use "const"
	*/
	//! Get number of faces
	int GetNumberOfFaces() const;	

	//! Print number of faces
	void PrintNumberOfFaces();	

	//! Get number of vertices
	int GetNumberOfVertices() const;	

	//! Print number of faces
	void PrintNumberOfVertices();	//print number of vertices

	//! Get number of indices
	int GetNumberOfIndices() const;

	//! Print number of indices
	void PrintNumberOfIndices();	//print number of vertices

protected:
	//! send data to GPU
	virtual void LoadData();		

	//! Vertex Array Object (store vertex attributes)
	GLuint m_VAO;				

	//! Memory for position, normal, colour, textureCoord
	GLuint m_VBO;				
	
	//! Index Buffer object
	GLuint m_IBO;		
	
	//! Object to store the textures
	GLuint m_TextureObjects;			

	//! type of geometrical primitives
	GLuint m_PrimitiveType;			

	//! Number of face/triangles
	GLuint m_NumOfFaces;

	//! Number of vertices
	GLuint m_NumOfVertices;

	//! Number of normals
	GLuint m_NumOfNormals;
	
	//! Number of indices
	GLuint m_NumOfIndices;

	//! Start of indices in buffer
	GLintptr m_IndexBufferOffset;
	
	//! Color (r, g, b, a) stored in a vector 
	std::vector<Vector4f> m_Colours;	

	//! Texture Coordinates (u, v) stored in a vector 
	std::vector<Vector2ui> m_TextCoords;	

	// Indices (i1, i2, i3) stored in a vector 
	std::vector<Vector3ui> m_Indices;

	// Data structure to store the attributes of vertex
	struct Vertex
	{
		std::vector<Vector3f> m_Position;	// Vertices (x, y, z) stored in a vector 
		std::vector<Vector3f> m_Normals;	// Normals (n1, n2, n3) stored in a vector 
	};

	// Vertex attributes are stored in single container
	Vertex m_Vertices;

	// Push the vertex data
	void PushPositionData(Vector3f& position);
	
	// Push the normals data
	void PushNormalData(Vector3f& normal);

	// Push the indices data
	void PushIndicesData(Vector3ui& indices);

	// Reserve the memory for position data
	void ReserveMemoryPositionData();

	// Send data to buffers
	void AssignBuffers(GLuint dataAttribLocation, GLint dataComponentsPerVert, GLuint dataSize, GLintptr dataOffset, const GLvoid * data);
	

	// Compute the normals per vertices
	void ComputeVertexNormals();



};
#endif // !MESH_H



