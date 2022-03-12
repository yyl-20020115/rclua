#include "pch.h"
#include "CppUnitTest.h"
#include "..\src\lua.hpp"
#include <filesystem>
#include <corecrt_io.h>
#include <Windows.h>

extern "C" void luaRC_deinit(lua_State * L);
extern "C" int luaC_set_enable_gc(int enable);

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(RCLuaUnitTest)
{
public:
	int ProcessLuaFile(const std::string& file) {
		luaC_set_enable_gc(0);

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaopen_base(L);
		luaopen_io(L);

		int ret = luaL_dofile(L, file.c_str());

		lua_close(L);

		luaRC_deinit(L);

		return ret;
	}

	int ProcessLuaFiles(const std::string& dir) {
			
		int ret = 0;
		struct _finddata_t data = { 0 };
		std::string filter = dir + "\\*.lua";
		long hnd = _findfirst(filter.c_str(), &data);
		if (hnd != -1)
		{
			do
			{
				if (data.attrib != _A_SUBDIR)
				{
					std::string file = dir + "\\" + data.name;
					ret = ProcessLuaFile(file);
					if (ret != 0) break;
				}
			} while (0 == _findnext(hnd, &data));
			_findclose(hnd);
		}
		return ret;
	}

	std::string ResetCurrentDirectory() {
		char buffer[_MAX_PATH] = { 0 };
		GetCurrentDirectoryA(sizeof(buffer), buffer);
		char* p = strrchr(buffer, '\\');
		if (p != 0) {
			*p = '\0';
			p = strrchr(buffer, '\\');
			if (p != 0) {
				*(p + 1) = '\0';
			}
		}
		SetCurrentDirectoryA(buffer);
		return buffer;
	}

public:
		
	void TestSpecificsImpl()
	{
		ResetCurrentDirectory();
		ProcessLuaFile("test\\specific\\_test.lua");
	}
	
	TEST_METHOD(TestSpecifics)
	{
		TestSpecificsImpl();
	}
};
