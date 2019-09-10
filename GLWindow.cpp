#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#include "SharedData.h"
#include "Camera.h"

#include "TestPlyModel.h"
#include "TestStlModel.h"
#include "TranslateFrame.h"
#include "ShaderInterface.h"

#include "FloorNode.h"
#include "LightNode.h"

#include "GLWindow.h"

Shader* ShaderInterface::m_BasicShader = NULL;
Shader* ShaderInterface::m_InstanceShading = NULL;

GLWindow::GLWindow(SharedData* modelMVC, QWidget* parent): QOpenGLWidget(parent), 
screenWidth(0), screenHeight(0), theModel(modelMVC), m_ObjectShader(NULL)
{
	/*!
	1) Instantiate the camera and set it to identity

	2) Instantiate the perpsective projection matrix

	3) Instantiate the root of scene node
	*/

	// Instantiate the main camera class
	m_Camera = new Camera;

	// Initialise the camera matrix to identity
	m_CameraMatrix.InitIdentity();

	// Set the initial projection matrix
	m_ProjectionInfo.FOV = 90.0f;
	m_ProjectionInfo.zNear = 0.1f;
	m_ProjectionInfo.zFar = 1000.0f;
	m_ProjectionInfo.Width = width();
	m_ProjectionInfo.Height = height();

	// Initialise the projection matrix
	m_ProjectionMatrix.InitPersProjTransformRHS(m_ProjectionInfo);

	// Initialise a root/header
	root = new SceneNode;	
}


GLWindow::~GLWindow()
{
	//! delete the camera
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

	//! delete the stlModel
	if (stlModel)
	{
		delete stlModel;
		stlModel = NULL;
	}
		
	//! delete the plyModel
	if (plyModel)
	{
		delete plyModel;
		plyModel = NULL;
	}
		
	//! delete the frame node
	if (m_TransFrame)
	{
		delete m_TransFrame;
		m_TransFrame = NULL;
	}
		

	// Delete the (static) translate mesh
	TranslateFrame::DeleteTranslateMesh();

	// Delete the shaders
	ShaderInterface::DeleteBasicShaders();
	ShaderInterface::DeleteInstanceShaders();

	// Delete stl model
	TestStlModel::DeleteModel();

	// Delete ply model
	TestPlyModel::DeleteModel();

	// For Lighting
	LightNode::DeleteModel();


}

void GLWindow::initializeGL()
{
	// Initialise glew at first before using opengl functions
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Failed to initialise the glew" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Opengl version and graphic card driver
	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

	// Graphic card
	std::cout << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;

	// Background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	 //black color
	
	// Load data to GPU
	sendDataToGPU();

	// Set a timer
	myTimer = new QTimer;
	connect(myTimer, &QTimer::timeout, this, &GLWindow::myUpdate);
	myTimer->start(0);	//start immediately wihtout any delay

}

void GLWindow::DrawNode(SceneNode* node)
{
	if (node->getMesh())
	{
		// Compute Model Matrix and Model View Projection Matrix(MVP)
		Matrix4f MMatrix, MVPMatrix;
		Matrix4f nodeWorldTrans, nodeScale;
		nodeScale.InitScaleTransform(node->GetModelScale().x, node->GetModelScale().y, node->GetModelScale().z);
		nodeWorldTrans = node->GetWorldTransform();

		// Concatentate the matrices
		MMatrix = nodeWorldTrans * nodeScale;
		MVPMatrix = m_ProjectionMatrix * m_CameraMatrix * MMatrix;

		// Lighting
		Vector3f LightPosition = m_MainLight->GetLightPosition();
		Vector3f LightColor(0.0f, 1.0f, 0.0f);
		float LightIntensity(0.5f);

		if(m_ObjectShader = node->getShader())
		{
			// Bind shader
			m_ObjectShader->Bind();

			// Set the unfiorm variable for each node
			m_ObjectShader->setUniformVariable("ModelMatrix", MMatrix);

			// Set the unfiorm variable for each node
			m_ObjectShader->setUniformVariable("ModelViewProjectionMatrix", MVPMatrix);
			
			// Set the unfiorm variable for each node
			m_ObjectShader->setUniformVariable("LightInScene.LightPosition", LightPosition);

			// Set the unfiorm variable for each node
			m_ObjectShader->setUniformVariable("LightInScene.LightColor", LightColor);

			// Set the unfiorm variable for each node
			m_ObjectShader->setUniformVariable("LightInScene.LightIntensity", LightIntensity);
			
			// Draw the mesh of each node
			node->draw();

			// Unbind shader
			m_ObjectShader->Unbind();
		}		
	}

	for (auto i = node->GetChildIteratorStart(); i != node->GetChildIteratorEnd(); ++i)
		DrawNode(*i);
}

void GLWindow::sendDataToGPU()
{
	/*! 
	1) Call static function that create static mesh
	2) Instantiate Node that to be rendered
	3) Assign the instantiated node as child of the root
	*/
	
	// Must Initiate the shaders
	ShaderInterface::CreateBasicShaders();
	ShaderInterface::CreateInstanceShaders();

	//do after glew is initialised otherwise error occur
	TestStlModel::CreateModel();	
	stlModel = new TestStlModel;
	root->addChild(stlModel);

	// Call create mesh function for the frame
	TranslateFrame::CreateTranslateMesh();
	m_TransFrame = new TranslateFrame;
	root->addChild(m_TransFrame);

	// Call create mesh function for the frame
	TestPlyModel::CreateModel();
	plyModel = new TestPlyModel;
	root->addChild(plyModel);

	// Create the Floor node
	m_Floor = new FloorNode;
	root->addChild(m_Floor);

	// Instantiate the Light Node
	LightNode::CreateModel();
	m_MainLight = new LightNode;
	root->addChild(m_MainLight);
		
}



void GLWindow::paintGL()
{
	// Retrieve camera position per frame
	m_CameraMatrix = m_Camera->GetCameraMatrix();
	
	// Update the transformation
	root->updateTranformation();

	// Draw the objects in the scene
	drawObjects();

	// Check for any error
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << err << std::endl;
	}
	
}


void GLWindow::drawObjects()
{
	// Clear the Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable the depth test
	glEnable(GL_DEPTH_TEST);	
	
	// Draw each node
	DrawNode(root);	
}

void GLWindow::screenToClip(Vector2f& screenCoordinate)
{
	// y-coordinate
	screenCoordinate.y = 1.0f - (screenCoordinate.y/ (0.5f * screenHeight) );

	// x-coordinate
	screenCoordinate.x = (screenCoordinate.x / (0.5f * screenWidth)) - 1.0f;
}

void GLWindow::myUpdate()
{
	// Draw again
	static unsigned int counter = 0;
	counter++;
	if (counter % 100 == 0)
	{
		repaint();
		counter = 0;
	}
}



void GLWindow::resizeGL(int w, int h)
{
	// Update the screen width and height when window size changes
	screenWidth = w;
	screenHeight = h;

	// Update the projection information
	m_ProjectionInfo.Width = screenWidth;
	m_ProjectionInfo.Height = screenHeight;

	// New the projection matrix
	Matrix4f newProjection;
	newProjection.InitPersProjTransformRHS(m_ProjectionInfo);

	// Assign the new projection information
	m_ProjectionMatrix = newProjection;


}

void GLWindow::mousePressEvent(QMouseEvent* buttonPress)
{
	// Current mouse position
	QPoint mousePos = buttonPress->pos();

	// Convert mouse point from pixel->Normalized Coordinates
	lastMousePos = Vector2f(mousePos.x(), mousePos.y());
	screenToClip(lastMousePos);

	// When the Right button is pressed the mouse position is updated
	// in the camera class
	if (buttonPress->buttons() & Qt::MiddleButton)
	{
		// Pass the Mouse position
		m_Camera->mouseUpdate(lastMousePos);
	}

	if (buttonPress->buttons() & Qt::LeftButton)
	{
		//current mouse position
		QPoint mousePos = buttonPress->pos();

		//covert the mouse movement points from pixel->Normalized Coordinates
		lastMousePos = Vector2f(mousePos.x(), mousePos.y());
		screenToClip(lastMousePos);
	}
	

	// After process we have to repaint the scene
	repaint();
}

void GLWindow::mouseMoveEvent(QMouseEvent* Move)
{
	// Current mouse position
	QPoint mousePos = Move->pos();

	// Convert mouse point from pixel->Normalized Coordinates
	Vector2f currentMousePos(mousePos.x(), mousePos.y());
	screenToClip(currentMousePos);

	// Check whether the left button is pressed
	if (Move->buttons() & Qt::LeftButton)
	{
		//update mouse position
		lastMousePos = currentMousePos;		
	}

	// If the middle button is pressed send data to camers
	// Check whether the left button is pressed
	if (Move->buttons() & Qt::RightButton)
	{
		// Send the mouse position to main camera class
		m_Camera->mouseUpdate(currentMousePos);

		//update mouse position
		lastMousePos = currentMousePos;
	}

	// After process we have to repaint the scene
	repaint();

	
	
}


