#include "Input.h"

#include "Renderer.h"
#include "Utils.h"

#ifdef _DEBUG
#include "Debug.h"
#endif



using namespace Sky::Core;

// Static variables
std::array<Input::KeyState, Input::s_keyCount> Input::s_keys = Utils::InitArray<Input::KeyState, Input::KeyState::Up, Input::s_keyCount>();
std::map<std::thread::id, std::array<Input::ThreadInputData, Input::s_keyCount>> Input::s_threadKeys;


constexpr bool Input::IsKeyCodeValid(int32_t keyCode)
{
	return keyCode > 0 && keyCode < s_keyCount;
}

Input::InputMessage Input::GetInputMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	InputMessage inputMsg;
	inputMsg.Key = static_cast<int32_t>(wParam);

	switch (msg)
	{
		// Keyboard
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		inputMsg.KeyState = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		inputMsg.KeyState = KeyState::Up;
		break;

		// Mouse
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		inputMsg.KeyState = msg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
		inputMsg.Key = VK_MBUTTON;
		break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		inputMsg.KeyState = msg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
		inputMsg.Key = VK_RBUTTON;
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		inputMsg.KeyState = msg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
		inputMsg.Key = VK_LBUTTON;
		break;

	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		inputMsg.KeyState = msg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
		inputMsg.Key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;

	//	// Not an input message
	//default:
	//	return;
	}

	return inputMsg;
}

void Input::ProcessInput(InputMessage inputMsg)
{
	if (!inputMsg.IsValid())
		return;

	s_keys[inputMsg.Key] = inputMsg.KeyState;
}

void Input::ProcessInput(UINT msg, WPARAM wParam, LPARAM lParam)
{
	InputMessage inputMsg = Input::GetInputMessage(msg, wParam, lParam);
	ProcessInput(inputMsg);
}

bool Input::IsKeyDownOnce(int32_t keyCode)
{
	if (!IsKeyCodeValid(keyCode))
		return false;

	std::thread::id tId = std::this_thread::get_id();
	if (s_threadKeys.find(tId) == s_threadKeys.end())
	{
		s_threadKeys.insert(std::pair<std::thread::id, std::array<Input::ThreadInputData, Input::s_keyCount>>(
			tId,
			std::array<Input::ThreadInputData, Input::s_keyCount>()
		));
	}

	std::array<ThreadInputData, Input::s_keyCount>* keys = &s_threadKeys[tId];
	ThreadInputData* inputData = keys[keyCode].data();

	bool isKeyDown = Input::IsKeyDown(keyCode);
	if (isKeyDown)
	{
		inputData->IsClicked = false;
		inputData->IsDown = true;
	}
	else
	{
		inputData->IsClicked = inputData->IsDown;
		inputData->IsDown = false;
	}

	return inputData->IsClicked;

}

Input::KeyState Input::GetKeyState(int32_t keyCode)
{
	if (!IsKeyCodeValid(keyCode))
		return KeyState::None;

	return s_keys[keyCode];
}

#pragma region Keyboard wrapper

bool Input::IsKeyDown(int32_t keyCode)
{
    //return IsKeyCodeValid(keyCode) && GetAsyncKeyState(keyCode) & 1;
	return Input::GetKeyState(keyCode) == KeyState::Down;
}

bool Input::IsKeyUp(int32_t keyCode)
{
    //return !Input::IsKeyDown(keyCode);
	return Input::GetKeyState(keyCode) == KeyState::Up;
}

bool Input::IsKeyPressed(int32_t keyCode)
{
	return Input::GetKeyState(keyCode) == KeyState::Pressed;
}

#pragma endregion

#pragma region Mouse wrapper

#pragma region Left MB
bool Input::IsLeftMouseDown()
{
	return IsKeyDown(VK_LBUTTON);
}

bool Input::IsLeftMouseUp()
{
	return IsKeyUp(VK_LBUTTON);
}

bool Input::IsLeftMousePressed()
{
	return IsKeyPressed(VK_LBUTTON);
}
#pragma endregion

#pragma region Right MB
bool Input::IsRightMouseDown()
{
	return IsKeyDown(VK_RBUTTON);
}

bool Input::IsRightMouseUp()
{
	return IsKeyUp(VK_RBUTTON);
}

bool Input::IsRightMousePressed()
{
	return IsKeyPressed(VK_RBUTTON);
}
#pragma endregion

#pragma region Middle MB
bool Input::IsMiddleMouseDown()
{
	return IsKeyDown(VK_MBUTTON);
}
bool Input::IsMiddleMouseUp()
{
	return IsKeyUp(VK_MBUTTON);
}
bool Input::IsMiddleMousePressed()
{
	return IsKeyPressed(VK_MBUTTON);
}
#pragma endregion

#pragma endregion