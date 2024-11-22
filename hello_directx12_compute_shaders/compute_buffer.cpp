// Liam Wynn, 10/31/2024, Hello DirectX 12: Compute Shader Edition

#include "compute_buffer.h"
#include "utils.h"

void initialize_compute_buffer(
	compute_buffer* buffer,
	dx12_handler* dx12,
	const unsigned int width,
	const unsigned int height,
	const DXGI_FORMAT format
) {
	buffer->width = width;
	buffer->height = height;
	buffer->format = format;

	//
	// Allocate an unordered access view buffer on the GPU.
	//

	allocate_buffer_on_gpu(buffer, dx12);
}

void allocate_buffer_on_gpu(
	compute_buffer* buffer,
	dx12_handler* dx12
) {
	D3D12_RESOURCE_DESC buffer_desc;
	HRESULT result;
	CD3DX12_HEAP_PROPERTIES default_heap;
	ComPtr<ID3D12Device> dev;

	dev = dx12->device;

	//
	// Set up the buffer description for the resource.
	//

	buffer_desc = {};
	buffer_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	buffer_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	buffer_desc.Width = buffer->width;
	buffer_desc.Height = buffer->height;
	buffer_desc.Format = buffer->format;
	buffer_desc.MipLevels = 1;
	buffer_desc.DepthOrArraySize = 1;
	buffer_desc.SampleDesc.Count = 1;
	buffer_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//
	// Allocate the buffer using the resource description.
	//

	default_heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	result = dev->CreateCommittedResource(
		&default_heap,
		D3D12_HEAP_FLAG_NONE,
		&buffer_desc,
		D3D12_RESOURCE_STATE_COMMON,
		NULL,
		IID_PPV_ARGS(&buffer->buffer)
	);

	throw_if_failed(result);
}
