// Xbox entry point for Starship
// This replaces the PC main() and sets up the Xbox-specific environment

#ifdef PLATFORM_XBOX

#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <SDL.h>
#include "xbox_debug.h"

// Game.cpp defines SDL_main on _WIN32 platforms
extern "C" int SDL_main(int argc, char* argv[]);

// Xbox applications start from main() when using nxdk
int main(void) {
    // Initialize Xbox video
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    debugPrint("=====================================\n");
    debugPrint("  Starship - Star Fox 64 Xbox Port\n");
    debugPrint("=====================================\n\n");
    debugPrint("Loading...\n\n");

    xbox_log("=== Starship Xbox Boot ===\n");
    xbox_log("XVideoSetMode done\n");

    // Call the game's SDL_main entry point
    char* argv[] = { (char*)"starship", nullptr };
    xbox_log("Calling SDL_main...\n");
    int result = SDL_main(1, argv);

    xbox_log("SDL_main returned: %d\n", result);
    debugPrint("SDL_main returned: %d\n", result);
    Sleep(10000);
    return result;
}

#endif // PLATFORM_XBOX
