#pragma once
#include <stdio.h>
#include <string>

const char log_file[] = "logfile.txt";
const char console_title[] = "mogeln";

enum class OUTPUT
{
	CONSOLE,
	TXT,
	INGAME,
	WATCH
};

void printSelect(OUTPUT o, const char* fmt, ...);


#ifdef _DEBUG

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define PRINT(type, fmt, ... ) ( printSelect(type, "[%s, %i] [%s] " fmt "\n", __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__))
#define PRINT_WATCH(pos, type, fmt, ...) ( printSelect(type, "[%i] [%s, %i] [%s] " fmt "\n", pos, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__))

#define CONSOLE( fmt, ... ) PRINT( OUTPUT::CONSOLE, fmt, __VA_ARGS__ )
#define TXT( fmt, ... )		PRINT( OUTPUT::TXT, fmt, __VA_ARGS__ )
#define INGAME( fmt, ... )	PRINT( OUTPUT::INGAME, fmt, __VA_ARGS__ )
#define WATCH(pos, fmt, ... )   PRINT_WATCH(pos, OUTPUT::WATCH, fmt, __VA_ARGS__ )

#endif // _DEBUG


#ifndef  _DEBUG

#define __FILENAME__

#define PRINT(type, fmt, ... )
#define PRINT_WATCH(pos, type, fmt, ...)

#define CONSOLE( fmt, ... )
#define TXT( fmt, ... )
#define INGAME( fmt, ... )
#define WATCH(pos, fmt, ... )

#endif // ! _DEBUG