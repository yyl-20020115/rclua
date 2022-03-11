#include "pch.h"
#include <Windows.h>
#include "rclua_unit_test.cpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	RCLuaUnitTest test;

	test.TestOriginals();
	test.TestSpecifics();

	return 0;
}