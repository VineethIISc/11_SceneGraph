#include "Mesh.h"
#include "Shader.h"

#include "SceneNode.h"



SceneNode::SceneNode(Mesh * mesh, Shader* shader): m_Mesh(mesh), 
m_Shader(shader), m_Parent(NULL)
{
	/*!
	1) Initialise the mesh, shader
	
	2) Parent is set to NULL at the beginning
	
	3) Set the local transform to identity

	4) Set the scale factor as unity
	*/

	// Local transform: relative to the parent
	m_Localtransform.InitIdentity();	

	// Scale is unity
	m_ModelScale = Vector3f(1.0f, 1.0f, 1.0f);
}


SceneNode::~SceneNode()
{
	/*!
	Delete each children of the current Node
	*/
	for (unsigned int i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i])
		{
			delete m_Children[i];
			m_Children[i] = NULL;
		}
			
	}
}

void SceneNode::draw()
{
	//draw the mesh associated with the node
	if (m_Mesh)
	{
		//call the render function of mesh
		m_Mesh->RenderMesh();	
	}
		
}

void SceneNode::updateTranformation()
{
	/*!
	1) To render the mesh, we need global tranform of each node

	2) Multiply local transform of each node with its parent's global transform

	3) Root node global transform is it local transform
	*/

	// If a parent exist for node
	if (m_Parent)
	{
		//relative with the parent world transform
		m_Worldtransform = m_Parent->m_Worldtransform * m_Localtransform;
	}
	// Root node
	else
	{
		// Root node tranformation is identity (origin)
		m_Worldtransform = m_Localtransform;
	}
		

	// Update the transformation recursively to children
	for (auto i = m_Children.begin(); i != m_Children.end(); ++i)
	{
		(*i)->updateTranformation();
	}
		
}

void SceneNode::addChild(SceneNode* node)
{
	// Check for valid node
	if (node)
	{
		// Add child to the current node
		this->m_Children.push_back(node);

		// Set this node as parent to its child
		node->m_Parent = this;
	}
}
