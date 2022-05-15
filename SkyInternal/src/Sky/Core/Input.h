#pragma once

#include <Windows.h>
#include <WinUser.h>

#include <thread>
#include <array>
#include <map>

namespace Sky
{
	namespace Core
	{

		class Input
		{
			// Fields
		private:
			struct ThreadInputData
			{
				bool IsClicked = false;
				bool IsDown = false;
			};
		public:
			enum class KeyState : unsigned int
			{
				None = 1,
				Up,
				Down,
				Pressed /*Down for atleast 2 calls*/
			};
			struct InputMessage
			{
				int32_t Key = -1;
				KeyState KeyState = KeyState::None;

				bool IsValid()
				{
					return Input::IsKeyCodeValid(Key);
				}
			};

			static constexpr const int s_keyCount = 256;

			static std::array<KeyState, s_keyCount> s_keys;
			static std::map<std::thread::id, std::array<ThreadInputData, s_keyCount>> s_threadKeys;

			// Methods
		private:

		public:
			static constexpr bool IsKeyCodeValid(int32_t keyCode);

			// Input handling
			static InputMessage GetInputMessage(UINT msg, WPARAM wParam, LPARAM lParam);
			static void ProcessInput(InputMessage inputMsg);
			static void ProcessInput(UINT msg, WPARAM wParam, LPARAM lParam);

			// Thread keys
			static bool IsKeyDownOnce(int32_t keyCode);

			// Keys
			static Input::KeyState GetKeyState(int32_t keyCode);

#pragma region Wrapper
			// Keyboard wrapper
			static bool IsKeyDown(int32_t keyCode);
			static bool IsKeyUp(int32_t keyCode);
			static bool IsKeyPressed(int32_t keyCode);

			// Mouse wrapper
			// Left mouse
			static bool IsLeftMouseDown();
			static bool IsLeftMouseUp();
			static bool IsLeftMousePressed();

			// Right mouse
			static bool IsRightMouseDown();
			static bool IsRightMouseUp();
			static bool IsRightMousePressed();

			// Middle mouse
			static bool IsMiddleMouseDown();
			static bool IsMiddleMouseUp();
			static bool IsMiddleMousePressed();
#pragma endregion
		};

	}
}