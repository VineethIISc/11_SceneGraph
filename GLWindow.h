//! Opengl Window
/*!
This window is for display the scene
*/

#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "CommonHeader.h"
#include <QtWidgets/qopenglwidget.h>

//! Compile Shader Data
class SharedData;

//! Compile SceneNode
class SceneNode;

//! Compile Shader
class Shader;

//! Compile Qt classes
class QTimer;
class QMouseEvent;

//! Camera Class
class Camera;

//! TranslateFrame class
class TranslateFrame;

//! Compile ply models
class TestPlyModel;

//! Compile stl models
class TestStlModel;

//! Compile the floor node
class FloorNode;

//! Compile the light node
class LightNode;




class GLWindow: public QOpenGLWidget
{
public:
	GLWindow(SharedData* modelMVC, QWidget * parent);
	~GLWindow();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent* buttonPress) override;
	void mouseMoveEvent(QMouseEvent* mouseMove) override;

private:
	// Ply models
	TestPlyModel* plyModel;

	// Stl Models
	TestStlModel* stlModel;
	SceneNode* root;

	// Floor Node
	FloorNode* m_Floor;

	//! Draw all nodes
	void DrawNode(SceneNode* node);

	//shader object
	Shader* m_ObjectShader;

	//! Pointers to Nodes
	TranslateFrame* m_TransFrame;

	// Instance of main Camera
	Camera* m_Camera;
	Matrix4f m_CameraMatrix;

	// Information for perspective projection
	PersProjInfo_f m_ProjectionInfo;
	Matrix4f m_ProjectionMatrix;

	// Lighting
	LightNode* m_MainLight;

	
private:
	void sendDataToGPU();
	void drawObjects();
	void screenToClip(Vector2f& screenCoordinate);

private:
	//screen properties
	int screenWidth, screenHeight;
	float aspectRatio;

	//mouse position
	Vector2f lastMousePos;

	//qtimer 
	QTimer* myTimer;

private slots:
	void myUpdate();

private:
	//model view controller
	SharedData* theModel;
};
#endif

