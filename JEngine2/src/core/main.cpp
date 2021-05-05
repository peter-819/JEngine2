#include "pch.h"
#include "Application.h"

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
	std::cout << "Hello JEngine2 !" << std::endl;
	JEngine2::Application App(hInstance);
	App.Run();
	return 0;
}