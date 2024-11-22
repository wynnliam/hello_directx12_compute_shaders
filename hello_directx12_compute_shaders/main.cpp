// Liam Wynn, 10/21/2024, Hello DirectX 12: Compute Shader Edition

/*
	In this program, I demonstrate a simple compute shader - or shaders.
	Not sure yet as of writing this. I am following this tutorial here:

	https://www.stefanpijnacker.nl/article/compute-with-directx12-part-1/
*/

#include <iostream>
#include "dx12_handler.h"
#include "compute_buffer.h"

using namespace std;

int main() {
	dx12_handler* dx12;
	compute_buffer* comp_buffer;

	cout << "Hello, DirectX 12" << endl;

	dx12 = new dx12_handler;
	initialize_dx12_handler(dx12);

	comp_buffer = new compute_buffer;
	initialize_compute_buffer(
		comp_buffer,
		dx12,
		256,
		256,
		DXGI_FORMAT_R8G8B8A8_UNORM
	);

	delete dx12;
	delete comp_buffer;

	return 0;
}