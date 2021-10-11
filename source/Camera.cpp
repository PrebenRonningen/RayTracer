#include "Camera.h"

#include <string>

const float Camera::m_MovementSpeed = 10.f;
const float Camera::m_RotationSpeed = 1.f;
const float Camera::m_SprintSpeed = 75.f;

size_t Camera::m_AmountOfCameras = 0;

Camera::Camera(const Elite::FPoint3& position, const Elite::FVector3& forwardVector)
	: m_Position{position}
	, m_WorldUp{ Elite::FVector3(0,1,0) }
	, m_W{ forwardVector }
	, m_pRay{ new Ray{&m_Position, m_W} }
	, m_pHitRecord{ new HitRecord }
	, m_FOV{45.f}
	, m_S{ 1 }
	, m_ID{ m_AmountOfCameras++}

{
	m_V = m_WorldUp;
	SetFOV(m_FOV);
	SetOBNMatrix(m_WorldUp);
}

Camera::~Camera()
{
	delete m_pRay;
	m_pRay = nullptr;

	delete m_pHitRecord;
	m_pHitRecord = nullptr;
}

void Camera::SetOBNMatrix()
{
	m_U = Cross(m_V, m_W);
	m_V = Cross(m_W, m_U);
	m_W = Cross(m_U, m_V);

	Normalize(m_U);
	Normalize(m_V);
	Normalize(m_W);

	m_ONB = Elite::FMatrix4(m_U, m_V, m_W, Elite::FVector4(m_Position.x, m_Position.y, m_Position.z, 1));

	m_RotationMatrix = Elite::FMatrix3(m_U, m_V, m_W);
}

void Camera::SetOBNMatrix(const Elite::FVector3& upVector)
{
	m_U = Cross(upVector, m_W);
	m_V = Cross(m_W, m_U); 
	m_W = Cross(m_U, m_V); 

	Normalize(m_U);
	Normalize(m_V);
	Normalize(m_W);

	m_ONB = Elite::FMatrix4(m_U, m_V, m_W, Elite::FVector4(m_Position.x, m_Position.y, m_Position.z, 1));

	m_RotationMatrix = Elite::FMatrix3(m_U, m_V, m_W);
}

void Camera::SetFOV(float degres)
{
	m_FOV = tanf(Elite::ToRadians(degres) / 2.f);
}

float Camera::GetFOV() const
{
	return m_FOV;
};

void Camera::SetScreenSpaceCoordinate(const Elite::FVector3& screenSpace)
{
	m_pRay->direction = m_RotationMatrix *Elite::FVector3(screenSpace.x, screenSpace.y, -m_S);
	Normalize(m_pRay->direction);
}

float Camera::GetS() const
{
	return m_S;
}

Ray* Camera::GetRay() const
{
	return m_pRay;
}

HitRecord* Camera::GetHitRecord() const
{
	return m_pHitRecord;
}

#pragma region KEYBOARD
void Camera::MoveLocalX(const float deltaTime, const bool positiveDirection)
{
	m_Position += m_U * deltaTime * m_ActiveMovementSpeed * ( positiveDirection ? 1.f : -1.f );
}
void Camera::MoveLocalY(const float deltaTime, const bool positiveDirection)
{
	m_Position += m_V * deltaTime * m_ActiveMovementSpeed * ( positiveDirection ? 1.f : -1.f );
}

void Camera::MoveLocalZ(const float deltaTime, const bool positiveDirection)
{
	m_Position += m_W * deltaTime * m_ActiveMovementSpeed * ( positiveDirection ? 1.f : -1.f );
}

void Camera::Pan(const float deltaTime, const bool positiveDirection)
{
	m_W = MakeRotation(m_RotationSpeed * deltaTime * ( positiveDirection ? 1.f : -1.f ), m_WorldUp) * m_W;
	SetOBNMatrix(m_WorldUp);
}

void Camera::Tilt(const float deltaTime, const bool positiveDirection)
{
	if ( m_W.y >= -( 1 - FLT_EPSILON ) && m_W.y <= ( 1 - FLT_EPSILON ) )
	{
		m_W = MakeRotation(m_RotationSpeed * deltaTime * ( positiveDirection ? 1.f : -1.f ), m_U) * m_W;
		SetOBNMatrix(m_V);
	}
}
void Camera::Sprint(bool isSprinting)
{
	if ( isSprinting )
	{
		m_ActiveMovementSpeed = m_SprintSpeed;
	} else
	{
		m_ActiveMovementSpeed = m_MovementSpeed;
	}
}
const size_t Camera::GetID() const
{
	return m_ID;
}

const char* Camera::GetType() const
{
	return "Camera";
}
#pragma endregion // Keyboard Input

#pragma region MOUSE
void Camera::RotateAndMoveLRMouse(const float deltaTime, const int x, const int y)
{
	m_Position += m_W * deltaTime * m_ActiveMovementSpeed * float(-y);
	m_W = MakeRotation(deltaTime * ( x < 0  ? 1.f : (x > 0) ? -1.f : 0 ), m_WorldUp) * m_W;
	SetOBNMatrix(m_WorldUp);
}

void Camera::RotateMouse(const float deltaTime, const int x, const int y)
{
	m_W = MakeRotation(deltaTime * ( x < 0 ? 1.f : ( x > 0 ) ? -1.f : 0 ), m_WorldUp) * m_W;
	SetOBNMatrix(m_WorldUp);
	if ( m_W.y >= -( 1 - FLT_EPSILON ) && m_W.y <= ( 1 - FLT_EPSILON ) )
	{
		m_W = MakeRotation(deltaTime * ( y < 0 ? 1.f : ( y > 0 ) ? -1.f : 0 ), m_U) * m_W;
	}
	SetOBNMatrix(m_V);
}

void Camera::MoveUpDownMouse(const float deltaTime, const int y)
{
	m_Position += m_WorldUp * deltaTime * float(y) * m_ActiveMovementSpeed;
}

#pragma endregion // Mouse Input