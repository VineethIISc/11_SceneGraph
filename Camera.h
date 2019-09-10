//! Camera Class
/*!
Camera class
Use pitch and Yaw
Reference :https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/theviewmatrix/Tutorial%202%20-%20The%20View%20Matrix.pdf
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "CommonHeader.h"

class Camera
{
public:
	//! Constructor
	Camera();

	//! Constructor to set value
	Camera(float pitchAngle, float yawAngle, Vector3f position);
	
	//! Destructor
	~Camera() {};

	//! 
	void UpdateCamera(float msec = 10.0f);

	//! Get the configuration of the camera 
	Matrix4f GetCameraMatrix();

	//! 
	void mouseUpdate(const Vector2f& newMousePosition);

	//! Accessor functions: Get and Set position	
	Vector3f GetPosition() const { return m_PosOfCamera; }
	void SetPosition(const Vector3f val) { m_PosOfCamera = val; }
	
	//! Accessor functions: Get and Set yaw angle
	float GetYaw() const { return m_YawAngle; }
	void SetYaw(const float y) { m_YawAngle = y; }
	
	//! Accessor functions: Get and Set pitch angle
	float GetPitch() const { return m_PitchAngle; }
	void SetPitch(const float p) { m_PitchAngle = p; }

private:
	//! Movement about vertical axis
	float m_YawAngle;

	//! Movement about horizontal axis
	float m_PitchAngle;

	//! Position of the camera
	Vector3f m_PosOfCamera;

	//! View Direction of camera
	Vector3f m_ViewDirection;

	//! Up Direction of camera (it is a constant direction)
	const Vector3f m_Up;

	//! Old mouse position
	Vector2f m_oldMousePosition;

	//! Camera Configuration Matrix;
	Matrix4f CameraMatrix;



};
#endif


