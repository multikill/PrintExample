#include "Print.hpp"
#include <Windows.h>
#include <consoleapi.h>
#include <consoleapi2.h>
#include <consoleapi3.h>
#include <WinUser.h>
#include <fileapi.h>
#include <stdio.h>
#include <memory>

#define FAIL -1
#define OK 0

struct PrintToConsole
{
	errno_t err;
	bool already_exist = false;

	PrintToConsole() : err(FAIL)
	{
		err = AllocConsole();
		if (err == 0)
		{
			err = OK;
			already_exist = true;
			return;
		}

		err = freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		if (err)
		{
			err = FAIL;
			return;
		}

		err = SetConsoleTitleA(console_title);
		if (err == 0)
		{
			err = FAIL;
			return;
		}
	}
	~PrintToConsole()
	{
		if (already_exist)
			return;

		err = fclose((FILE*)stdout);
		if (err)
		{
			err = FAIL;
			return;
		}

		HWND HwndConsole = GetConsoleWindow();
		if (HwndConsole == NULL)
		{
			err = FAIL;
			return;
		}

		err = FreeConsole();
		if (err == 0)
		{
			err = FAIL;
			return;
		}

	}
	void print(const char* str)
	{
		if (err == FAIL)
			return;

		fprintf(stdout, str);
	}
};

std::unique_ptr<PrintToConsole> ptc = nullptr;
void printConsole(const char* str)
{
	if (ptc == nullptr)
		ptc = std::make_unique<PrintToConsole>();

	if (ptc)
		ptc->print(str);
}


struct PrintToFile
{
	FILE* file_ptr;
	errno_t err;
	char filePath[MAX_PATH];

	PrintToFile() : file_ptr(nullptr), err(FAIL)
	{
		// Get temp path
		char tempPath[MAX_PATH + 1];
		err = GetTempPathA(ARRAYSIZE(tempPath), tempPath);
		if (err == 0)
		{
			err = FAIL;
			return;
		}

		// Get logfile path
		err = sprintf_s(filePath, ARRAYSIZE(filePath), "%s%s", tempPath, log_file);
		if (err == -1)
		{
			err = FAIL;
			return;
		}

		// open path
		err = fopen_s(&file_ptr, filePath, "w");
		if (err)
		{
			err = FAIL;
			return;
		}

		// close file
		err = fclose(file_ptr);
		if (err)
		{
			err = FAIL;
			return;
		}
	}
	~PrintToFile()
	{

	}
	void print(const char* str)
	{
		if (err == FAIL)
			return;

		// open path
		err = fopen_s(&file_ptr, filePath, "a");
		if (err)
		{
			err = FAIL;
			return;
		}

		// print file
		fprintf(file_ptr, str);

		// flush buffer
		err = fflush(file_ptr);
		if (err)
		{
			err = FAIL;
			return;
		}

		// close file
		err = fclose(file_ptr);
		if (err)
		{
			err = FAIL;
			return;
		}
	}
};


std::unique_ptr<PrintToFile> ptf = nullptr;
void printFile(const char* str)
{
	if (ptf == nullptr)
		ptf = std::make_unique<PrintToFile>();

	if (ptf)
		ptf->print(str);
}


void printSelect(OUTPUT o, const char* fmt, ...)
{
#ifdef _DEBUG

	char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 255, fmt, args);


	switch (o)
	{
	case OUTPUT::TXT:
		printFile(buffer);
		break;
	case OUTPUT::INGAME:
		printConsole(buffer);
		break;
	case OUTPUT::WATCH:
	{
		int pos = args[0];
		printConsole(buffer);
		break;
	}
	default:
		// OUTPUT::CONSOLE
		printConsole(buffer);
		break;
	}

	va_end(args);

#endif
}
