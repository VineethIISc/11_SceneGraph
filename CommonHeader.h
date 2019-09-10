#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "sc_math.h"

#define MINI_EPSILON 1E-03
#define BUFFER_OFFSET(bytes) ((GLvoid*)bytes)

#define SHADER_PATH "shaders/"
#define IMAGE_PATH "images/"
#define MODEL_PATH "model/"

//! Attributes per vertices
/*!
Vertex can have following attribute:
position, normal, colour, texture, matrix
*/
enum MeshAttributes
{
	POSITION_ATTRIB, NORMAL_ATTRIB, COLOUR_ATTRIB, TEXTURE_ATTRIB, MATRIX_ATTRIB, MAX_ATTRIB
};

#endif // !COMMON_HEADER_H

