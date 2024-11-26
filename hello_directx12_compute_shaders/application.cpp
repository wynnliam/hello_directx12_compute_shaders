// Liam Wynn, 11/22/2024, Hello DirectX 12: Compute Shader Edition

#include "application.h"
#include "utils.h"
#include <string>
#include <iostream>

using namespace std;

void initialize_application(application* app) {

	//
	// Initialize DirectX 12.
	//

	app->dx12 = new dx12_handler;
	initialize_dx12_handler(app->dx12);

	//
	// Initialize the root signature.
	//

	app->root_signature = create_root_signature(app);

	//
	// Initialize the pipeline state.
	//

	app->pipeline_state = initialize_pipeline_state(app);

	//
	// Initialize the compute buffer.
	//

	app->buffer = new compute_buffer;
	initialize_compute_buffer(
		app->buffer,
		app->dx12,
		256,
		256,
		DXGI_FORMAT_R8G8B8A8_UNORM
	);
}

// TODO: When integrating into hello_directx_12, I should abstract out
// the code to set up a root signature and pipeline. Otherwise the code
// will quickly become gross and repetitive.
ComPtr<ID3D12RootSignature> create_root_signature(application* app) {
	ComPtr<ID3D12Device5> dev;
	ComPtr<ID3D12RootSignature> root_signature;
	CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
	CD3DX12_ROOT_PARAMETER1 pipeline_parameters[1];
	D3D12_ROOT_SIGNATURE_FLAGS flags;
	D3D12_FEATURE_DATA_ROOT_SIGNATURE feature_data;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	ComPtr<ID3DBlob> root_signature_blob;
	ComPtr<ID3DBlob> err_blob;
	string err_msg;
	HRESULT result;

	dev = app->dx12->device;

	//
	// Essentially saying "we have a single parameter that is a UAV buffer.
	// It is the 0th register of the 0th register range.
	//

	ranges[0] = {};
	ranges[0].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		1,
		0,
		0
	);

	pipeline_parameters[0] = {};
	pipeline_parameters[0].InitAsDescriptorTable(1, &(ranges[0]));

	flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	//
	// Attempt to get version 1.1 support. Fall back on 1.0 if that
	// fails.
	//

	feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	result = dev->CheckFeatureSupport(
		D3D12_FEATURE_ROOT_SIGNATURE,
		&feature_data,
		sizeof(feature_data)
	);

	if (result != S_OK) {
		feature_data.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	//
	// Create the root signature itself.
	//

	// TODO: Omitting creation of a sampler here. I want to see what happens
	// when we don't include it. I suspect we'll be ok because our shader
	// doesn't use it. Code does not explain why it is neccessary.

	root_signature_desc.Init_1_1(
		1,
		pipeline_parameters,
		0,
		NULL,
		flags
	);

	result = D3DX12SerializeVersionedRootSignature(
		&root_signature_desc,
		feature_data.HighestVersion,
		&root_signature_blob,
		&err_blob
	);

	if (err_blob != NULL) {
		err_msg = string((char*)err_blob->GetBufferPointer());
		cerr << "Failed to load root signature: " << err_msg << endl;
		throw_if_failed(result);
	}

	result = dev->CreateRootSignature(
		0,
		root_signature_blob->GetBufferPointer(),
		root_signature_blob->GetBufferSize(),
		IID_PPV_ARGS(&root_signature)
	);

	throw_if_failed(result);

	return root_signature;
}

// TODO: Just like root signature initialization, I want to abstract this
// code too.
ComPtr<ID3D12PipelineState> initialize_pipeline_state(application* app) {
	ComPtr<ID3D12PipelineState> pipeline_state;
	ComPtr<ID3D12Device5> dev;
	ComPtr<ID3D12RootSignature> root_signature;
	UINT compile_flags;
	ComPtr<ID3DBlob> compute_blob;
	ComPtr<ID3DBlob> err_blob;
	char* err_msg;
	D3D12_PIPELINE_STATE_STREAM_DESC pipeline_state_stream_desc;
	pipeline_state_stream pss;
	HRESULT result;

	dev = app->dx12->device;
	root_signature = app->root_signature;

	//
	// Compile the shader source code.
	//

	compile_flags = 0;
#if defined(_DEBUG)
	compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DCompileFromFile(
		L"./hello_compute.hlsl",
		NULL,
		NULL,
		"main",
		"cs_5_1",
		compile_flags,
		0,
		&compute_blob,
		&err_blob
	);

	if (FAILED(result) && err_blob != NULL) {
		err_msg = (char*)err_blob->GetBufferPointer();
		cerr << err_msg << endl;
	}

	throw_if_failed(result);

	//
	// Now create our pipeline state description.
	//

	pss.root_sig = root_signature.Get();
	pss.bytecode = CD3DX12_SHADER_BYTECODE(compute_blob.Get());
	pipeline_state_stream_desc = { sizeof(pss), &pss };

	//
	// Finally, create the pipeline state.
	//

	result = dev->CreatePipelineState(
		&pipeline_state_stream_desc,
		IID_PPV_ARGS(&pipeline_state)
	);

	throw_if_failed(result);

	return pipeline_state;
}
