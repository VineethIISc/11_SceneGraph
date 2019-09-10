#include "STLMesh.h"

void STLMesh::LoadSTLMesh(std::string filename)
{
	/*!
	1) Load the stl File

	2) Check format of stl (ascii/binary)

	3) Choose ASCII reader or BINARY reader

	4) Close the file

	5) Load the data to GPU
	*/

	// Read the file as stream
	std::ifstream inputFile(filename, std::ios::binary);
	if (!inputFile)
	{
		std::cout << "Failed to load the file: " << filename << std::endl;
		return;
	}	

	// Check for the format of input file 
	this->m_FileFormat = CheckFormat(inputFile);

	// Read the file using respective methods
	if (this->m_FileFormat == STL_ASCII)
	{
		// Read the ascii file
		ReadASCIIstl(inputFile);		
	}		
	else
	{
		// Read the binary file
		ReadBINARYstl(inputFile);		
	}
		

	//close the file
	inputFile.close();

	// Assign number of vertices (very important for stl files)
	Mesh::m_NumOfVertices = Mesh::m_NumOfNormals = Mesh::m_NumOfFaces * STL_NUM_VERTEX_PER_FACE;

	//load the data to GPU
	Mesh::LoadData();

}

int STLMesh::CheckFormat(std::ifstream& input)
{
	/*
	1) Read data into char array of size STL_SOLID_WORD_LEGNTH + 1 = 6 bytes
	2) Check for word facet, because some binary file start with "solid"
	*/

	// Read data 5 bytes of information
	char format[STL_SOLID_WORD_LEGNTH + 1];
	input.read(format, STL_SOLID_WORD_LEGNTH);
	
	// Write the end character (must for no error)
	format[STL_SOLID_WORD_LEGNTH] = '\0';

	// File is ascii if it start with word solid
	if (strcmp(format, STL_SOLID_START) == 0)
	{
		// Search for word "facet"
		if (CheckForFacet(input))
			return STL_ASCII;
	}

	// Otherwise file is binary
	return STL_BINARY;
}


bool STLMesh::CheckForFacet(std::ifstream& input)
{
	// Local variable
	std::string line;

	// Read each line: here read 3 line for time being
	for (int i = 0; i < STL_CHECK_LINE; i++)
	{
		// Check for END OF FILE (maynot occur)
		if (std::getline(input, line))
		{
			// Local variable
			std::string search = STL_FACE;

			// Search "facet" word in the line
			size_t found = line.find(search);

			// If found facet in line: "ASCII file format cofirmed"
			if (found != std::string::npos)
				return true;
		}
	}

	// False if failed
	return false;
}

void STLMesh::ReadASCIIstl(std::ifstream & input)
{
	// Set the file get pointer to beginning
	input.seekg(0, std::ios::beg);

	// Read till end of file
	while (input)
	{
		// Local variable
		std::string data;
		input >> data;

		// Check for "facet"
		if (data == STL_FACE)
		{
			// Do till "endfacet"
			do
			{
				//  Read data
				input >> data;

				// Check for "vertex"
				if (data == STL_VERTEX)
				{
					Vector3f v;
					input >> v.x >> v.y >> v.z;

					// Vertices are inherited from  Mesh class
					Mesh::PushPositionData(v);
				}
				// Check for "normal"
				else if (data == STL_NORMAL)
				{
					Vector3f n;
					input >> n.x >> n.y >> n.z;

					// Normals are inherited from  Mesh class
					Mesh::PushNormalData(n);
				}

			} while (data != STL_END_FACE);

			/* One face is read
			 Increment the variable in Mesh class */
			Mesh::m_NumOfFaces++;							
		}
	}

}

void STLMesh::ReadBINARYstl(std::ifstream & input)
{
	// Set the file get pointer to beginning
	input.seekg(0, std::ios::beg);

	// Read the header whose size is 80 bytes
	char header[STL_BINARY_HEADER_SIZE + 1];
	input.read(header, STL_BINARY_HEADER_SIZE);

	// Assign end character as last element (must do for no error)
	header[STL_BINARY_HEADER_SIZE] = '\0';

	// Read 4bytes or 32bit unsinged int for number of triangles
	input.read((char*)& this->m_TempNumOfFaces, sizeof(uint32_t));

	// Assign to member of Mesh class
	Mesh::m_NumOfFaces = this->m_TempNumOfFaces;

	// Reserve the memory 
	ReserveMemoryPositionData();

	// Read the triangles and data of each faces
	for (uint32_t i = 0; i < Mesh::m_NumOfFaces; i++)
	{
		// Read normals which are 3 floats or 12 bytes or 48bit
		Vector3f n;
		input.read((char*)& n, STL_BINARY_TRIANGLE_NORMAL_SIZE);

		// Normals are inherited from  Mesh class
		Mesh::PushNormalData(n);

		// For each face there will be 3 vertices
		for (int j = 0; j < STL_NUM_VERTEX_PER_FACE; j++)
		{
			// Read vertex which are 3 floats or 12 bytes or 48 bit
			Vector3f v;
			input.read((char*)& v, STL_BINARY_TRIANGLE_NORMAL_SIZE);

			// Vertices are inherited from  Mesh class
			Mesh::PushPositionData(v);
		}

		// Read 2bytes or 16bit attribute count
		uint16_t attributeCount;
		input.read((char*)& attributeCount, STL_BINARY_TRIANGLE_ATTRIBUTE_SIZE);

		// Check for presence of attribute
		if (attributeCount)
			std::cout << "Alert!!!!!!!!!!! Attribute Count: " << attributeCount << std::endl;

	}
}





