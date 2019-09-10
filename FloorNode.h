#ifndef FLOOR_NODE_H
#define FLOOR_NODE_H

#include "SceneNode.h"

//! Compile FloorMesh class
class FloorMesh;

class FloorNode : public SceneNode
{
public:
	//! Constructor
	FloorNode();

	/*!
	No Destructor is required:
	Data is in tree node deleted at the end
	*/

	//! Override the update function
	void updateTranformation() override;

private:
	// Instance to mesh of floor
	FloorMesh* m_GridMesh;
};
#endif //!FLOOR_NODE_H
