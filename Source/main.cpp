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


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{    
    // Initialization
    //--------------------------------------------------------------------------------------

    //TODO: must wrap in try/catch. don't let exceptions escape. 

    Game game { State::STARTSCREEN };
    
    //--------------------------------------------------------------------------------------

    //TODO: State machine for update and render loop (main menu/game/credit screen)
    // Main game loop

    //TODO: Consider windowshouldclose in game instead
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game.Update();
      
        // Draw
        //----------------------------------------------------------------------------------
        //TODO: put begin draw in game.render
        BeginDrawing();
        ClearBackground(BLACK);

        game.Render();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // Deinitialization

    std::string filename = "level.txt";  

    return 0;
}