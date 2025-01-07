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
	ComPtr<ID3D12PipelineState> pipeline_state;
};

struct pipeline_state_stream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE root_sig;
	CD3DX12_PIPELINE_STATE_STREAM_CS bytecode;
};

void initialize_application(application* app);
ComPtr<ID3D12RootSignature> create_root_signature(application* app);
ComPtr<ID3D12PipelineState> initialize_pipeline_state(application* app);

void run_compute(application* app);

void shutdown_app(application* app);
