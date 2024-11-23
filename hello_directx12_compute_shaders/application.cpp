// Liam Wynn, 11/22/2024, Hello DirectX 12: Compute Shader Edition

#include "application.h"

void initialize_application(application* app) {

	//
	// Initialize DirectX 12.
	//

	app->dx12 = new dx12_handler;
	initialize_dx12_handler(app->dx12);

	//
	// Initialize the root signature
	//

	app->root_signature = create_root_signature(app);

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

ComPtr<ID3D12RootSignature> create_root_signature(application* app) {
	ComPtr<ID3D12RootSignature> root_signature;
	CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
	CD3DX12_ROOT_PARAMETER1 pipeline_parameters[1];

	// Essentially saying "we have a single parameter that is a UAV buffer.
	// It is the 0th register of the 0th register range.
	ranges[0] = {};
	ranges[0].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		1,
		0,
		0
	);

	pipeline_parameters[0] = {};
	pipeline_parameters[0].InitAsDescriptorTable(1, &(ranges[0]));

	// TODO: Finish me!

	return root_signature;
}
