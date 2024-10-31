// Liam Wynn, 10/22/2024, Hello DirectX 12: Compute Shader Edition

/*
	A simple include file to combine a lot of reused includes like
	DirectX, WIN32API, and Standard C++ stuff. I'm not going to be
	too discerning about what libraries will be in here. This whole
	program is pretty quick-and-dirty.
*/

#pragma once

// Excludes rarely used stuff from the Windows headers.
#ifndef  WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // ! WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include <wrl.h>

using namespace Microsoft::WRL;
