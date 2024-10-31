// Liam Wynn, 10/22/2024, Hello DirectX 12: Compute Shader Edition

#pragma once

#ifndef  WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <exception>

inline void throw_if_failed(const HRESULT result) {
	if (FAILED(result)) {
		throw std::exception();
	}
}
