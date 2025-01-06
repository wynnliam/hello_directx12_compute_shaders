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

	//
	// Now that we have allocated the buffer, create a UAV
	// descriptor for it.
	//

	create_buffer_descriptor(buffer, dx12);

	//
	// Next, set up the readback buffer. This is the buffer we
	// will copy our compute shader result into.
	//

	initialize_readback_buffer(buffer, dx12);
}

void allocate_buffer_on_gpu(
	compute_buffer* buffer,
	dx12_handler* dx12
) {
	D3D12_RESOURCE_DESC buffer_desc;
	HRESULT result;
	CD3DX12_HEAP_PROPERTIES default_heap;
	ComPtr<ID3D12Device5> dev;

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

void create_buffer_descriptor(
	compute_buffer* buffer,
	dx12_handler* dx12
) {
	descriptor_heap* desc_heap;
	ComPtr<ID3D12Device5> dev;
	D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc;

	desc_heap = dx12->cbv_srv_uav_heap;
	dev = dx12->device;

	uav_desc = {};
	uav_desc.Format = buffer->format;
	uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	buffer->uav_index = next_available_heap_index(desc_heap);

	dev->CreateUnorderedAccessView(
		buffer->buffer.Get(),
		NULL,
		&uav_desc,
		heap_cpu_handle(desc_heap, buffer->uav_index)
	);
}

void initialize_readback_buffer(
	compute_buffer* buffer,
	dx12_handler* dx12
) {
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT num_rows;
	UINT64 row_size_in_bytes;
	UINT64 total_buffer_size;
	ComPtr<ID3D12Device5> device;
	ComPtr<ID3D12Resource> data_buffer;
	D3D12_RESOURCE_DESC buffer_desc;
	D3D12_RESOURCE_DESC readback_desc;
	D3D12_HEAP_PROPERTIES heap_properties;
	HRESULT result;

	device = dx12->device;
	data_buffer = buffer->buffer;
	buffer_desc = data_buffer->GetDesc();

	//
	// Not entirely clear why we need to do this step.
	//

	device->GetCopyableFootprints(
		&buffer_desc,
		0,
		1,
		0,
		&footprint,
		&num_rows,
		&row_size_in_bytes,
		&total_buffer_size
	);

	//
	// Use the result from above to create our readback buffer.
	// Note: we can hardcode a lot of this I think.
	//

	readback_desc = {};
	readback_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	readback_desc.Alignment = 0;
	readback_desc.Width = total_buffer_size;
	readback_desc.Height = 1;
	readback_desc.DepthOrArraySize = 1;
	readback_desc.MipLevels = 1;
	readback_desc.Format = DXGI_FORMAT_UNKNOWN;
	readback_desc.SampleDesc.Count = 1;
	readback_desc.SampleDesc.Quality = 0;
	readback_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	readback_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);

	result = device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&readback_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		NULL,
		IID_PPV_ARGS(&(buffer->readback_buffer))
	);

	throw_if_failed(result);
}
