#pragma once
#include <DirectXMath.h>
#include "TypeDefines.h"

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	// functions
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Move(sFloat2 xy);
	void Move(float32 x, float32 y);

	sFloat3 GetPosition();
	sFloat3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	float m_PosX, m_PosY, m_PosZ;
	float m_RotX, m_RotY, m_RotZ;
	DirectX::XMMATRIX m_ViewMatrix;
};