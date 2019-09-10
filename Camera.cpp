#include "Camera.h"



Camera::Camera(): m_YawAngle(0.0f), m_PitchAngle(0.0f), m_PosOfCamera(20.5f, 20.0f, 20.0f),
m_ViewDirection(0.0f, 0.0f, -1.0f), m_Up(0.0f, 1.0f, 0.0f)
{
	/*
	Look initilly at -ve z- direction
	Camera Up vector is y-axis
	*/
}


Camera::Camera(float pitchAngle, float yawAngle, Vector3f position)
{
	/*! 
	Set the value of pitch and yaw angle
	Set the position of camera
	*/
	this->m_PitchAngle = pitchAngle;
	this->m_YawAngle = yawAngle;
	this->m_PosOfCamera = position;
	
}

void Camera::UpdateCamera(float msec)
{
}

Matrix4f Camera::GetCameraMatrix() 
{
	Vector3f targetPos(0.0f, 0.0f, 0.0f);
	CameraMatrix.LookAtRHS(m_PosOfCamera, targetPos, m_Up);
	return CameraMatrix;
}

void Camera::mouseUpdate(const Vector2f& newMousePosition)
{
	/*!
	1) Find the change in mouse position

	2) Rotate the view direction w.r.t up direction

	3) Change the old mouse position
	*/

	// Find the difference in position of mouse
	Vector2f mouseDelta = newMousePosition - m_oldMousePosition;

	// Create a rotation matrix about Up axis
	Matrix4f RotY;
	RotY.InitAxisRotateTransform(m_Up, mouseDelta.x);

	// Rotate the view direction with the corresponding rotation
	m_ViewDirection = RotY * m_ViewDirection;

	// Assign the new mouse position to old mouse position
	m_oldMousePosition = newMousePosition;
}


