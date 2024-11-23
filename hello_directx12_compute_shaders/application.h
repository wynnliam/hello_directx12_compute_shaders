// Liam Wynn, 11/22/2024, Hello DirectX 12: Compute Shader Edition

/*
	Defines the "application". Although in our case it's a bit light.
	The justification for this is that it reflects my full DX12 learning
	code structurally. The dx12_handler is sort of the spine, and the
	application is everything around it that isn't main.
*/

#pragma once

#include "dx12_handler.h"
#include "compute_buffer.h"

struct application {
	dx12_handler* dx12;
	compute_buffer* buffer;

	ComPtr<ID3D12RootSignature> root_signature;
};

void initialize_application(application* app);
ComPtr<ID3D12RootSignature> create_root_signature(application* app);
