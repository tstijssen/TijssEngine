#include "Camera.h"
using namespace DirectX;

Camera::Camera()
{
	m_PosX = 0.0f;
	m_PosY= 0.0f;
	m_PosZ = 0.0f;

	m_RotX = 0.0f;
	m_RotY = 0.0f;
	m_RotZ = 0.0f;
}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{

	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
	return;
}

void Camera::Move(sFloat2 xy)
{
	m_PosX += xy.x;
	m_PosY += xy.y;
}
void Camera::Move(float32 x, float32 y)
{
	m_PosX += x;
	m_PosY += y;
}

void Camera::SetRotation(float x, float y, float z)
{

	m_RotX = x;
	m_RotY = y;
	m_RotZ = z;
	return;
}

sFloat3 Camera::GetPosition()
{
	return sFloat3(m_PosX, m_PosY, m_PosZ);
}

sFloat3 Camera::GetRotation()
{
	return sFloat3(m_RotX, m_RotY, m_RotZ);
}

void Camera::Render()
{
	XMFLOAT3 up, pos, lookAt;
	XMVECTOR upVec, posVec, lookAtVec;

	float yaw, pitch, roll;

	XMMATRIX rotMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVec = XMLoadFloat3(&up);

	pos.x = m_PosX;
	pos.y = m_PosY;
	pos.z = m_PosZ;

	posVec = XMLoadFloat3(&pos);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVec = XMLoadFloat3(&lookAt);

	pitch = m_RotX * 0.0174532925f;
	yaw = m_RotY * 0.0174532925f;
	roll = m_RotZ * 0.0174532925f;

	rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVec = XMVector3TransformCoord(lookAtVec, rotMatrix);
	upVec = XMVector3TransformCoord(upVec, rotMatrix);

	lookAtVec = XMVectorAdd(posVec, lookAtVec);

	m_ViewMatrix = XMMatrixLookAtLH(posVec, lookAtVec, upVec);

}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_ViewMatrix;
	return;
}