// Liam Wynn, 10/22/2024, Hello DirectX 12: Compute Shader Edition

/*
	The DX12 Handler will handle all of the DirectX 12 components.
	Anything DX12 related will need to go through this system.
*/

#pragma once

#include "stdafx.h"

struct dx12_handler {
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> command_queue;
	ComPtr<ID3D12CommandAllocator> command_allocator;
	ComPtr<ID3D12GraphicsCommandList> command_list;
};

void initialize_dx12_handler(dx12_handler* dx12);
void enable_dx12_debug_layer();
ComPtr<IDXGIFactory4> create_dx12_factory();
ComPtr<IDXGIAdapter4> get_valid_adapter(ComPtr<IDXGIFactory4> factory);
ComPtr<ID3D12Device> create_dx12_device(ComPtr<IDXGIAdapter4> adapter);
ComPtr<ID3D12CommandQueue> create_command_queue(dx12_handler* dx12);
ComPtr<ID3D12CommandAllocator> create_command_allocator(dx12_handler* dx12);
ComPtr<ID3D12GraphicsCommandList> create_command_list(dx12_handler* dx12);
