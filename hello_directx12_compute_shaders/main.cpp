// Liam Wynn, 10/21/2024, Hello DirectX 12: Compute Shader Edition

/*
	In this program, I demonstrate a simple compute shader - or shaders.
	Not sure yet as of writing this. I am following this tutorial here:

	https://www.stefanpijnacker.nl/article/compute-with-directx12-part-1/
*/

#include <iostream>
#include "application.h"

using namespace std;

int main() {
	application* app;

	cout << "Hello, DirectX 12" << endl;

	app = new application;
	initialize_application(app);
	run_compute(app);

	delete app;

	return 0;
}