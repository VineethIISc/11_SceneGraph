#include "PLYMesh.h"

void PLYMesh::LoadPLYMesh(std::string filename)
{
	/*!
	1) Load the ply File

	2) Check format of ply (ascii/binary)

	3) Choose ASCII reader or BINARY reader

	4) Close the file

	5) Load the data to GPU
	*/

	// Read the file as stream
	std::ifstream inputFile(filename, std::ios::binary);
	if (!inputFile)
	{
		std::cout << "Failed to load the file: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	// Local variable
	std::string data;
	inputFile >> data;

	// Check whether the input file is ply or not
	if (data == PLY_MAGIC_NUMBER)
	{
		// Check for the format of input file 
		ReadHeader(inputFile);

		// Read the file using respective methods
		if (this->m_FileFormat == PLY_ASCII)
			ReadASCIIply(inputFile);		// Read ascii file
		else
			ReadBINARYply(inputFile);		// Read binary file
	}
	else
		std::cout << "File is not ply: " << filename << std::endl;

	// Close the file
	inputFile.close();

	// If normal is not present in the ply file
	if (!Mesh::m_NumOfNormals)
		ComputeVertexNormals();

	// Load the data to GPU
	Mesh::LoadData();
}


void PLYMesh::ReadHeader(std::ifstream & input)
{
	/*!
	Read the header to determine, whether the given 
	\param input file is ascii or binary
	*/

	// local variable
	std::string data;

	// read till end of header
	do
	{		
		input >> data;
		if (data == PLY_FORMAT_WORD)				// Check for word "format"
		{
			input >> data;
			if (data == PLY_ASCII_FORMAT)			// Check for word "ascii"
			{
				m_FileFormat = PLY_ASCII;			// Assign the ascii macro
				continue;
			}
			else
			{
				m_FileFormat = PLY_BINARY;			// Assign the binary macro
				continue;
			}
		}
		if (data == PLY_VERTEX)						// Check for word "vertex"
		{
			input >> Mesh::m_NumOfVertices;				// Assign to the member of Mesh class
			
			// Read data
			input >> data;

			// Next string "property"
			while (data == PLY_PROPERTY)			// Check for word "property"
			{
				input >> data;
				if (data == PLY_FLOAT)				// Check for word "float"
				{
					m_NumOfCoord++;					// Increment the number of floats

					// Local variable
					std::string x;
					input >> x;
					input >> data;
				}
			}

		}
		else if (data == PLY_FACE)					// Check for word "face"
		{
			input >> Mesh::m_NumOfFaces;					// Assign to the member of Mesh class
			Mesh::m_NumOfIndices = Mesh::m_NumOfFaces * NUM_INDICES_TRI;		// Assign to the member of Mesh class
		}
	} while (data != PLY_END_HEADER);				// Check for word "end_header"

}

void PLYMesh::ReadASCIIply(std::ifstream & input)
{
	/*
	Read the ascii file and populate the data
	\param input: input file
	*/

	// Reserve the memory
	ReserveMemoryPositionData();

	// Read all vertices
	for (unsigned int i = 0; i < Mesh::m_NumOfVertices; i++)
	{
		Vector3f v;
		input >> v.x >> v.y >> v.z;
		Mesh::PushPositionData(v);		// Vertices are inherited from Mesh class

	}

	//read all faces
	for (unsigned int i = 0; i < Mesh::m_NumOfFaces; i++)
	{
		//read the first number in each face
		unsigned int start;
		input >> start;

		//check if it is 3 for triangle
		Vector3ui ind;
		if (start == NUM_INDICES_TRI)
			input >> ind.a >> ind.b >> ind.c;
		else
			std::cout << "Face is not triangle" << std::endl;

		Mesh::PushIndicesData(ind);		// Indices are inherited from Mesh class
	}

	

}

void PLYMesh::ReadBINARYply(std::ifstream & input)
{
	/*
	Read the binary file and populate the data
	\param input: input file
	*/

	// Set the file get pointer to beginning
	input.seekg(0, std::ios::beg);

	// Skip the header
	std::string line;
	do
	{
		std::getline(input, line);
	} while (line != PLY_END_HEADER);


	// Reserve the memory
	ReserveMemoryPositionData();

	// Read all vertices
	for (unsigned int i = 0; i < Mesh::m_NumOfVertices; i++)
	{
		// Read a vertex: contain 3 coordinates
		Vector3f v;
		input.read((char*)& v, PLY_VERTEX_SIZE_TRI);
		Mesh::PushPositionData(v);					// Vertices are inherited from  Mesh class

	}

	// Read all indices
	for (unsigned int i = 0; i < Mesh::m_NumOfFaces; i++)
	{
		// Read the start value
		unsigned char start;
		input.read((char*)& start, PLY_FACE_START_NUM_SIZE);

		// Read the faces: in term of indices
		if (start == NUM_INDICES_TRI)
		{
			Vector3ui ind;
			input.read((char*)& ind, PLY_INDICES_SIZE_TRI);
			Mesh::PushIndicesData(ind);				// Indices are inherited from Mesh class
		}
		else
			std::cout << "Face is not triangle" << std::endl;

	}

}

