// Liam Wynn, 10/31/2024, Hello DirectX 12: Compute Shader Edition

/*
	In this program, the compute_buffer represents a data buffer
	we can read and write to. This requires what is called an
	"Unordered Access View" (UAV).

	A UAV can be written/read by multiple threads without memory
	conflicts.
*/

#pragma once

#include "stdafx.h"
#include "dx12_handler.h"

struct compute_buffer {
	ComPtr<ID3D12Resource> buffer;
	ComPtr<ID3D12Resource> readback_buffer;

	unsigned int uav_index;

	unsigned int width;
	unsigned int height;
	DXGI_FORMAT format;
};

void initialize_compute_buffer(
	compute_buffer* buffer,
	dx12_handler* dx12,
	const unsigned int width,
	const unsigned int height,
	const DXGI_FORMAT format
);

void allocate_buffer_on_gpu(
	compute_buffer* buffer,
	dx12_handler* dx12
);

void create_buffer_descriptor(
	compute_buffer* buffer,
	dx12_handler* dx12
);

void initialize_readback_buffer(
	compute_buffer* buffer,
	dx12_handler* dx12
);
