//! stlMesh class
/*!
1) This class inherits from the Mesh class

2) In this class two functions are override from Mesh class
	a) updateData()
	b) render()

3) Currently work for binary format and ascii format

4) There is no explicit constructor and destructor

5) Reference: https://en.wikipedia.org/wiki/STL_(file_format)
*/

#ifndef STL_MESH_H
#define STL_MESH_H

#include "Mesh.h"

#define STL_ASCII 1
#define STL_BINARY 2

//! memory in bytes 
#define STL_BINARY_HEADER_SIZE 80
#define STL_BINARY_TRIANGLE_SIZE 50
#define STL_BINARY_TRIANGLE_NORMAL_SIZE 12
#define STL_BINARY_TRIANGLE_VERTEX_SIZE 12
#define STL_BINARY_TRIANGLE_ATTRIBUTE_SIZE 2
#define STL_NUM_VERTEX_PER_FACE 3

//!definition
#define STL_SOLID_START "solid"
#define STL_SOLID_WORD_LEGNTH 5
#define STL_CHECK_LINE 3

#define STL_FACE "facet"
#define STL_END_FACE "endfacet"

#define STL_VERTEX "vertex"
#define STL_NORMAL "normal"

class STLMesh: public Mesh
{
public:
	/*! No explicit constructor and destructor*/
	
	//! Method to load stl file
	void LoadSTLMesh(std::string filename);

private:
	//*********************************
	//*********** variables ***********
	//*********************************
	// File format ascii/binary 
	int m_FileFormat;

	//number of facet
	uint32_t m_TempNumOfFaces;
	//*********************************


	//*********************************
	//*********** methods *************
	//*********************************
	// Check whether the given file is ascii or binary
	int CheckFormat(std::ifstream& input);

	// double check for ascii in stl 
	bool CheckForFacet(std::ifstream& input);

	//read ascii stl file
	void ReadASCIIstl(std::ifstream& input);

	//read binary stl file
	void ReadBINARYstl(std::ifstream& input);

};
#endif // !STL_MESH_H



