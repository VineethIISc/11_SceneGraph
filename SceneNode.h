//! Scene Node Class
/*!
Class is base class which represent the node in scene graph

Field of Class:
1) Mesh associated with the node
2) Shader associated with the node
3) Local transformation (position and orientation) w.r.t parent node
4) Global transformation (position and orientation) w.r.t global frame
5) Parent Node
6) Childrens of the Node
*/

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "CommonHeader.h"

class Mesh;
class Shader;

class SceneNode
{
public:
	SceneNode(Mesh * mesh = NULL, Shader* shader = NULL);
	virtual ~SceneNode();

	//! Render method
	virtual void draw();

	//! Important function that update the mesh position in worldCoordinate
	virtual void updateTranformation();

	//! Method to add child to node
	void addChild(SceneNode * node);

	//! Setters and getters for Local tranformation
	void SetLocalTransform(Matrix4f localTransMat) { m_Localtransform = localTransMat; }
	Matrix4f GetLocalTransform() { return m_Localtransform; }

	//! Setters and getters for World tranformation
	void SetWorldTransform(Matrix4f worldTransMat) { m_Worldtransform = worldTransMat; }
	Matrix4f GetWorldTransform() const { return m_Worldtransform; }	

	//! Setters and getters for mesh
	void setMesh(Mesh* mesh) { m_Mesh = mesh; }
	Mesh* getMesh() { return m_Mesh; }

	//! Setters and getters for Shaders
	void setShader(Shader* shader) { m_Shader = shader; }
	Shader* getShader() { return m_Shader; }

	//! Setters and getters for Scale
	void SetModelScale(Vector3f scale) { m_ModelScale = scale; }
	Vector3f GetModelScale() { return m_ModelScale; }

	//! Iterator for the childrens
	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return m_Children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return m_Children.end(); }

protected:
	//! Mesh associated with node if any
	Mesh* m_Mesh;		

	//! Shader associated with node if any
	Shader* m_Shader;

	//! Relative to the neighbour node(parent)
	Matrix4f m_Localtransform;	

	//! Relative to the world origin
	Matrix4f m_Worldtransform;	

	//! Parent of the current node
	SceneNode* m_Parent;		

	//! Scale factor per node
	Vector3f m_ModelScale;

	//! Children of current node
	std::vector<SceneNode*> m_Children;	


};
#endif // !SCENE_NODE_H



