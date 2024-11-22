// Liam Wynn, 10/22/2024, Hello DirectX 12: Compute Shader Edition

#include "dx12_handler.h"
#include "utils.h"

void initialize_dx12_handler(dx12_handler* dx12) {
	ComPtr<IDXGIFactory4> factory;
	ComPtr<IDXGIAdapter4> adapter;

	enable_dx12_debug_layer();

	factory = create_dx12_factory();
	adapter = get_valid_adapter(factory);

	dx12->device = create_dx12_device(adapter);
	dx12->command_queue = create_command_queue(dx12);
	dx12->command_allocator = create_command_allocator(dx12);
	dx12->command_list = create_command_list(dx12);

	dx12->cbv_srv_uav_heap = new descriptor_heap;
	initialize_descriptor_heap(
		dx12,
		dx12->cbv_srv_uav_heap,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		1000,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	);
}

void enable_dx12_debug_layer() {
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debug_interface;
	HRESULT result;

	result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface));
	throw_if_failed(result);

	debug_interface->EnableDebugLayer();
#endif
}

ComPtr<IDXGIFactory4> create_dx12_factory() {
	ComPtr<IDXGIFactory4> factory;
	UINT create_factory_flags;
	HRESULT result;

	create_factory_flags = 0;

#if defined(_DEBUG)
	create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	result = CreateDXGIFactory2(
		create_factory_flags,
		IID_PPV_ARGS(&factory)
	);

	throw_if_failed(result);

	return factory;
}

ComPtr<IDXGIAdapter4> get_valid_adapter(ComPtr<IDXGIFactory4> factory) {
	ComPtr<IDXGIAdapter4> adapter;
	HRESULT result;
	SIZE_T max_dedicated_vid_mem;
	UINT i;
	ComPtr<IDXGIAdapter1> next_adapter;
	DXGI_ADAPTER_DESC1 next_adapter_desc;
	UINT next_adapter_flags;
	UINT software_flag;
	BOOL is_hardware_adapter;
	BOOL can_create_device;
	SIZE_T curr_video_mem;
	BOOL has_more_video_mem;

	max_dedicated_vid_mem = 0;
	i = 0;

	while (true) {
		result = factory->EnumAdapters1(i, &next_adapter);
		if (result == DXGI_ERROR_NOT_FOUND) {
			break;
		}

		result = next_adapter->GetDesc1(&next_adapter_desc);

		next_adapter_flags = next_adapter_desc.Flags;
		software_flag = next_adapter_flags & DXGI_ADAPTER_FLAG_SOFTWARE;
		is_hardware_adapter = software_flag == 0;

		result = D3D12CreateDevice(
			next_adapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			__uuidof(ID3D12Device),
			NULL
		);

		can_create_device = SUCCEEDED(result);

		curr_video_mem = next_adapter_desc.DedicatedVideoMemory;
		has_more_video_mem = curr_video_mem > max_dedicated_vid_mem;

		if (is_hardware_adapter && can_create_device && has_more_video_mem) {
			max_dedicated_vid_mem = curr_video_mem;

			result = next_adapter.As(&adapter);
			throw_if_failed(result);
		}

		i++;
	}

	return adapter;
}

ComPtr<ID3D12Device> create_dx12_device(ComPtr<IDXGIAdapter4> adapter) {
	ComPtr<ID3D12Device> device;
	HRESULT result;

#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> debug_info;
	D3D12_INFO_QUEUE_FILTER filter;

	D3D12_MESSAGE_SEVERITY ignore_severities[] = {
		D3D12_MESSAGE_SEVERITY_INFO
	};

	D3D12_MESSAGE_ID ignore_ids[] = {
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
		D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
		D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
	};

	filter = {};
	filter.DenyList.NumSeverities = _countof(ignore_severities);
	filter.DenyList.pSeverityList = ignore_severities;
	filter.DenyList.NumIDs = _countof(ignore_ids);
	filter.DenyList.pIDList = ignore_ids;
#endif

	result = D3D12CreateDevice(
		adapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&device)
	);

	throw_if_failed(result);

#if defined(_DEBUG)
	result = device.As(&debug_info);
	if (SUCCEEDED(result)) {
		debug_info->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		debug_info->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		debug_info->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		result = debug_info->PushStorageFilter(&filter);
		throw_if_failed(result);
	}
#endif

	return device;
}

ComPtr<ID3D12CommandQueue> create_command_queue(dx12_handler* dx12) {
	ComPtr<ID3D12CommandQueue> command_queue;
	D3D12_COMMAND_QUEUE_DESC command_queue_desc;
	ComPtr<ID3D12Device> dev;
	HRESULT result;

	dev = dx12->device;

	command_queue_desc = {};

	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	command_queue_desc.NodeMask = 0;

	result = dev->CreateCommandQueue(
		&command_queue_desc,
		IID_PPV_ARGS(&command_queue)
	);

	throw_if_failed(result);

	return command_queue;
}

ComPtr<ID3D12CommandAllocator> create_command_allocator(dx12_handler* dx12) {
	ComPtr<ID3D12CommandAllocator> command_allocator;
	ComPtr<ID3D12Device> dev;
	HRESULT result;

	dev = dx12->device;

	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&command_allocator)
	);

	throw_if_failed(result);

	return command_allocator;
}

ComPtr<ID3D12GraphicsCommandList> create_command_list(dx12_handler* dx12) {
	ComPtr<ID3D12GraphicsCommandList> command_list;
	HRESULT result;
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12CommandAllocator> allocator;

	dev = dx12->device;
	allocator = dx12->command_allocator;

	result = dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		allocator.Get(),
		NULL,
		IID_PPV_ARGS(&command_list)
	);

	throw_if_failed(result);

	result = command_list->Close();
	throw_if_failed(result);

	return command_list;
}

void initialize_descriptor_heap(
	dx12_handler* dx12,
	descriptor_heap* heap,
	const D3D12_DESCRIPTOR_HEAP_TYPE heap_type,
	const unsigned int num_descriptors,
	const D3D12_DESCRIPTOR_HEAP_FLAGS flags
) {
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12DescriptorHeap> dx_heap;
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	HRESULT result;

	dev = dx12->device;

	desc = {};
	desc.Type = heap_type;
	desc.NumDescriptors = num_descriptors;
	desc.Flags = flags;

	result = dev->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&dx_heap)
	);

	throw_if_failed(result);

	heap->heap = dx_heap;
	heap->curr_descriptor_index = 0;
	heap->descriptor_count = num_descriptors;
	heap->descriptor_size = dev->GetDescriptorHandleIncrementSize(heap_type);
}
