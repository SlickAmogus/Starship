// Xbox entry point for Starship
// This replaces the PC main() and sets up the Xbox-specific environment

#ifdef PLATFORM_XBOX

#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <SDL.h>

// Game.cpp defines SDL_main on _WIN32 platforms
extern "C" int SDL_main(int argc, char* argv[]);

// Xbox applications start from main() when using nxdk
int main(void) {
    // Initialize Xbox video
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    debugPrint("Starship - Star Fox 64 Xbox Port\n");
    debugPrint("Initializing...\n");

    // Call the game's SDL_main entry point
    char* argv[] = { (char*)"starship", nullptr };
    int result = SDL_main(1, argv);

    if (result != 0) {
        debugPrint("Game exited with error: %d\n", result);
        Sleep(5000);
    }

    return result;
}

#endif // PLATFORM_XBOX
