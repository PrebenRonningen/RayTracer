#pragma once
#include "Ray.h"
#include <string>

class Camera
{
public:
	Camera(const Elite::FPoint3& position, const Elite::FVector3& forwardVector);

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;

	~Camera();

	Ray* GetRay() const;
	HitRecord* GetHitRecord() const;

	void SetFOV(const float degrees);
	float GetFOV() const;

	void SetOBNMatrix();
	void SetOBNMatrix(const Elite::FVector3& upVector);
	void SetScreenSpaceCoordinate(const Elite::FVector3& screenSpace);
	float GetS() const;

	void MoveLocalX(const float deltaTime, const bool positiveDirection);
	void MoveLocalY(const float deltaTime, const bool positiveDirection);
	void MoveLocalZ(const float deltaTime, const bool positiveDirection);
	
	void RotateAndMoveLRMouse(const float deltaTime, const int x, const int y);
	void RotateMouse(const float deltaTime, const int x, const int y);
	void MoveUpDownMouse(const float deltaTime, const int y);

	void Pan(const float deltaTime, const bool positiveDirection);
	void Tilt(const float deltaTime, const bool positiveDirection);

	void Sprint(bool isSprinting);

	const size_t GetID() const;
	const char* GetType() const;

private:	
	Elite::FMatrix4 m_ONB; // Look-At Matrix
	Elite::FMatrix3 m_RotationMatrix;
	Elite::FPoint3 m_Position;
	Elite::FVector3 m_WorldUp;
	Elite::FVector3 m_U, m_V, m_W;	// Right, Up, Forward
	
	HitRecord* m_pHitRecord;
	Ray* m_pRay;
	float m_FOV;
	float m_S;
	float m_ActiveMovementSpeed;

	static const float m_MovementSpeed;
	static const float m_RotationSpeed;
	static const float m_SprintSpeed;
	static size_t m_AmountOfCameras;

	const size_t m_ID;
};