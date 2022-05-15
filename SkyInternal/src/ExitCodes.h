#pragma once

// Special
#define SKY_SUCCESS 0	// General success
#define SKY_ERROR 1		// General error

#pragma region Engine codes
// 25 to 49

#pragma endregion

#pragma region Renderer codes
// 50 to 99

// Cannot get window handle
#define SKY_ERROR_WINDOW 50
// Cannot get the client rect of the window
#define SKY_ERROR_WINDOW_SIZE 51
// Cannot set the window proc
#define SKY_ERROR_WINDOW_PROC 52

#pragma endregion

#pragma region DirectXHelper codes
// 100 to 199

// Direct3DCreate9 failed
#define SKY_ERROR_DIRECT3D_CREATE 100
// d3d->CreateDevice failed
#define SKY_ERROR_DIRECT3D_CREATE_DEVICE 101

// Failed to init detours context
#define SKY_ERROR_DETOURS_INIT 110
// Failed to enable all hooks
#define SKY_ERROR_DETOURS_ENABLE 111

// Failed to hook endscene function
#define SKY_ERROR_DETOURS_HOOK_ENDSCENE 120
// Failed to hook reset function
#define SKY_ERROR_DETOURS_HOOK_RESET 121

#pragma endregion


#pragma region Memory codes
// 200 to 249

// Cannot get client.dll
#define SKY_ERROR_GET_CLIENT 200
// Cannot get engine.dll
#define SKY_ERROR_GET_ENGINE 201

#pragma endregion