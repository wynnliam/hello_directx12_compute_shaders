// Liam Wynn, 11/22/2024, Hello DirectX 12: Compute Shader Edition

#include "application.h"

void initialize_application(application* app) {

	//
	// Initialize DirectX 12.
	//

	app->dx12 = new dx12_handler;
	initialize_dx12_handler(app->dx12);

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
