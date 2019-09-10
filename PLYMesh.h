//! plyMesh class
/*!
1) This class inherits from the Mesh class

2) In this class two functions are override from Mesh class
	a) updateData()
	b) render()

3) Currently work for binary (little_endian) format and ascii format

4) There is no explicit constructor and destructor

5) Reference
	a) Greg Turk, reference: http://gamma.cs.unc.edu/POWERPLANT/papers/ply.pdf
	b) For Binary format, reference: https://medium.com/@elope139/reading-ply-files-that-are-in-binary-format-cab3a37276a2	
*/

#ifndef PLY_MESH_H
#define PLY_MESH_H

#include "Mesh.h"

#define PLY_MAGIC_NUMBER "ply"

#define PLY_ASCII 1
#define PLY_BINARY 2

#define PLY_FORMAT_WORD "format"
#define PLY_ASCII_FORMAT "ascii"
#define PLY_END_HEADER "end_header"

#define PLY_PROPERTY "property"
#define PLY_FLOAT "float"

#define PLY_ELEMENT "element"
#define PLY_VERTEX "vertex"
#define PLY_FACE "face"
#define PLY_NORMAL "normal"

#define NUM_INDICES_TRI 3
#define NUM_INDICES_LINE 2
#define PLY_NUM_VERTEX_PER_FACE 3		//faces are triangles

//! memory in bytes
#define PLY_VERTEX_SIZE_TRI 12
#define PLY_FACE_START_NUM_SIZE 1  
#define PLY_INDICES_SIZE_TRI 12

class PLYMesh : public Mesh
{
public:
	/*! No explicit constructor and destructor*/

	//! Method to load ply file
	virtual void LoadPLYMesh(std::string filename);

private: 
	//*********************************
	//*********** variables ***********
	//*********************************
	// File format ascii/binary
	int m_FileFormat;

	// place holder fo number of vertex Coordinates
	int m_NumOfCoord;
	//*********************************


	//*********************************
	//*********** methods *************
	//*********************************
	// Read the header of file
	void ReadHeader(std::ifstream& input);

	// Read the ascii file 
	void ReadASCIIply(std::ifstream& input);

	// Read the binary file 
	void ReadBINARYply(std::ifstream& input);
	//*********************************

	
};
#endif // !PLY_MESH_H



