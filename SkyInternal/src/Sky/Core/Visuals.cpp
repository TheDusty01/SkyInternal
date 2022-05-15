#include "Visuals.h"

using namespace Sky::Core;

Visuals::Visuals(int32_t windowWidth, int32_t windowHeight)
	: m_device(),
	m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
}

bool Visuals::DxWorldToScreen(Vector3f& position, Vector2f& outScreenPos) const
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIX view, projection, world;
	m_device->GetViewport(&viewPort);
	m_device->GetTransform(D3DTS_VIEW, &view);
	m_device->GetTransform(D3DTS_PROJECTION, &projection);
	D3DXMatrixIdentity(&world);

	D3DXVECTOR3* dxPos = (D3DXVECTOR3*)&position;	// TODO: Unsafe but in theory the same in RAM
	

	D3DXVECTOR3 dxOut;
	D3DXVec3Project(&dxOut, dxPos, &viewPort, &projection, &view, &world);
	if (dxOut.z < 1)
	{
		*&outScreenPos = { dxOut.x, dxOut.y };
		return true;
	}


	return false;
}

bool Visuals::WorldToScreen(Vector3f& position, Vector2f& outScreenPos, ViewMatrix matrix) const
{
	////Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	
	//vec4 clipCoords;
	//clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	//clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	//clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	//clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	Vector4<float> clipCoords;
	clipCoords[0] = position[0] * matrix(0, 0) + position[1] * matrix(0, 1) + position[2] * matrix(0, 2) + matrix(0, 3);
	clipCoords[1] = position[0] * matrix(1, 0) + position[1] * matrix(1, 1) + position[2] * matrix(1, 2) + matrix(1, 3);
	clipCoords[2] = position[0] * matrix(2, 0) + position[1] * matrix(2, 1) + position[2] * matrix(2, 2) + matrix(2, 3);
	clipCoords[3] = position[0] * matrix(3, 0) + position[1] * matrix(3, 1) + position[2] * matrix(3, 2) + matrix(3, 3);

	//if (clipCoords.w < 0.1f)
	//	return false;

	if (clipCoords[3] < 0.1f)
		return false;


	////perspective division, dividing by clip.W = Normalized Device Coordinates
	//vec3_t NDC;
	//NDC.x = clipCoords.x / clipCoords.w;
	//NDC.y = clipCoords.y / clipCoords.w;
	//NDC.z = clipCoords.z / clipCoords.w;

	Vector3f ndc;
	ndc[0] = clipCoords[0] / clipCoords[3];
	ndc[1] = clipCoords[1] / clipCoords[3];
	ndc[2] = clipCoords[2] / clipCoords[3];

	//screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	//screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	outScreenPos[0] = (m_windowWidth / 2 * ndc[0]) + (ndc[0] + m_windowWidth / 2);
	outScreenPos[1] = -(m_windowHeight / 2 * ndc[1]) + (ndc[1] + m_windowHeight / 2);
	
	return true;
}

bool Visuals::WorldToScreenFast(Vec3f& pos, Vec2f& outScreenPos, float matrix[16]) const
{
	Vec4f clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3f ndc;
	ndc.x = clipCoords.x / clipCoords.w;
	ndc.y = clipCoords.y / clipCoords.w;
	ndc.z = clipCoords.z / clipCoords.w;

	outScreenPos.x = (m_windowWidth / 2 * ndc.x) + (ndc.x + m_windowWidth / 2);
	outScreenPos.y = -(m_windowHeight / 2 * ndc.y) + (ndc.y + m_windowHeight / 2);

	return true;
}

void Visuals::DrawBox(int32_t x, int32_t y, int32_t w, int32_t h, int32_t thickness, Color color) const
{
	//D3DRECT rects[4] =
	//{
	//	{x, y, w, thickness},
	//	{x, y, thickness, h},
	//	{(x + w), y, thickness, h},
	//	{x, y + h, w + thickness}
	//};
	//DrawRects(4, rects, color);

	DrawRect(x, y, w, thickness, color); // Top horizontal line
	DrawRect(x, y, thickness, h, color); // Left vertical line
	DrawRect((x + w), y, thickness, h, color); // Right vertical line
	DrawRect(x, y + h, w + thickness, thickness, color); // Bottom horizontal line
}

void Visuals::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Color color) const
{
	D3DRECT rect = { x, y, x + w, y + h };
	DrawRect(rect, color);
}

void Visuals::DrawRect(D3DRECT rect, Color color) const
{
	m_device->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color.ToDXColor(), 0, 0);
}


void Visuals::DrawRects(DWORD rectCount, D3DRECT* rects, Color color) const
{
	m_device->Clear(rectCount, rects, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color.ToDXColor(), 0, 0);
}
