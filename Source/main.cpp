/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*b
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS )
#include "raylib.h"
#pragma warning(pop)
#include "game.hpp"
#include <print>
#include <format>
#include <stdexcept>


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
	try {
		// Initialization
		Game game( State::STARTSCREEN );
		// Game loop
		game.Run();
	}
	catch (const std::runtime_error& e) {
		std::print("\nRuntime Error: {}\n", e.what());
	}
	catch (const std::exception& e) {
		std::print("\nException: {}\n", e.what());
	}
	catch (...) {
		std::print("\nUnknown exception. I have no idea what broke.\n");
	}

	// Deinitialization handled by destructors
	return 0;
}