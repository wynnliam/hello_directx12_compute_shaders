// Liam Wynn, 10/22/2024, Hello DirectX 12: Compute Shader Edition

/*
	The DX12 Handler will handle all of the DirectX 12 components.
	Anything DX12 related will need to go through this system.
*/

#pragma once

#include "stdafx.h"

struct descriptor_heap {
	ComPtr<ID3D12DescriptorHeap> heap;
	unsigned int descriptor_size;
	unsigned int descriptor_count;
	unsigned int curr_descriptor_index;
};

struct dx12_handler {
	ComPtr<ID3D12Device5> device;
	ComPtr<ID3D12CommandQueue> command_queue;
	ComPtr<ID3D12CommandAllocator> command_allocator;
	ComPtr<ID3D12GraphicsCommandList> command_list;

	descriptor_heap* cbv_srv_uav_heap;
};

void initialize_dx12_handler(dx12_handler* dx12);
void enable_dx12_debug_layer();
ComPtr<IDXGIFactory4> create_dx12_factory();
ComPtr<IDXGIAdapter4> get_valid_adapter(ComPtr<IDXGIFactory4> factory);
ComPtr<ID3D12Device5> create_dx12_device(ComPtr<IDXGIAdapter4> adapter);
ComPtr<ID3D12CommandQueue> create_command_queue(dx12_handler* dx12);
ComPtr<ID3D12CommandAllocator> create_command_allocator(dx12_handler* dx12);
ComPtr<ID3D12GraphicsCommandList> create_command_list(dx12_handler* dx12);
void initialize_descriptor_heap(
	dx12_handler* dx12,
	descriptor_heap* heap,
	const D3D12_DESCRIPTOR_HEAP_TYPE heap_type,
	const unsigned int num_descriptors,
	const D3D12_DESCRIPTOR_HEAP_FLAGS flags
);
