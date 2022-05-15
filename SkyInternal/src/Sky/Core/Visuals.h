#pragma once

#include "Math.h"

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Forward declaration
class IDirect3DDevice9;
struct D3DXVECTOR3;

namespace Sky
{
	namespace Core
	{
		// Forward declaration
		class Renderer;

		struct Color
		{
			DWORD ColorBits = 0;

			// maps unsigned 8 bits/channel to Color
			constexpr static Color FromARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
			{
				return Color{ static_cast<DWORD>(((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))) };
				//return Color{ ((DWORD)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))) };
			}

			constexpr static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				return FromARGB(a, r, g, b);
			}

			constexpr static Color FromRGB(uint8_t r, uint8_t g, uint8_t b)
			{
				return FromARGB(255, r, g, b);
			}

			// maps floating point channels (0.f to 1.f range) to Color
			constexpr static Color FromRGBA(float r, float g, float b, float a)
			{
				return FromRGBA(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
			}

			constexpr D3DCOLOR ToDXColor() const
			{
				return static_cast<D3DCOLOR>(ColorBits);
			}

		};

		class Visuals
		{
			friend class Renderer;

		// Fields
		private:
			IDirect3DDevice9* m_device;
			int32_t m_windowWidth, m_windowHeight;

		// Methods
		private:

		public:
			Visuals(int32_t windowWidth, int32_t windowHeight);
			Visuals(const Visuals&) = delete;
			~Visuals() = default;

			bool DxWorldToScreen(Vector3f& position, Vector2f& outScreenPos) const;
			bool WorldToScreen(Vector3f& position, Vector2f& outScreenPos, ViewMatrix matrix) const;
			bool WorldToScreenFast(Vec3f& pos, Vec2f& outScreenPos, float matrix[16]) const;


			void DrawBox(int32_t x, int32_t y, int32_t w, int32_t h, int32_t thickness, Color color) const;
			//void DrawBox(int32_t x, int32_t y, int32_t w, int32_t h, int32_t thickness) const;

			void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Color color) const;
			void DrawRect(D3DRECT rect, Color color) const;
			void DrawRects(DWORD rectCount, D3DRECT* rects, Color color) const;
		};

	}
}

