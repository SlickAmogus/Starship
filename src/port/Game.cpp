#include <libultraship.h>

#include <Fast3D/interpreter.h>
#include "Engine.h"

#ifdef NXDK
#include "xbox_debug.h"
#include <hal/debug.h>
#endif

extern "C" {
#include <sf64mesg.h>
    void Main_SetVIMode(void);
    void Main_Initialize(void);
    void Main_ThreadEntry(void* arg);
    void Lib_FillScreen(u8 setFill);
    void Graphics_ThreadUpdate();
    void AudioThread_CreateTask();
}

extern "C"
void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

void push_frame() {
    Graphics_ThreadUpdate();
    GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    Timer_Update();
    // thread5_iteration();
    GameEngine::EndAudioFrame();
}

#ifdef _WIN32
int SDL_main(int argc, char **argv) {
#else
#if defined(__cplusplus) && defined(PLATFORM_IOS)
extern "C"
#endif
int main(int argc, char *argv[]) {
#endif
#ifdef NXDK
    xbox_log("SDL_main entered\n");
    debugPrint("  Creating game engine...\n");
#endif
    GameEngine::Create();
#ifdef NXDK
    xbox_log("GameEngine::Create() done\n");
    debugPrint("  Engine created OK\n");
    debugPrint("  Initializing game...\n");
#endif
    Main_SetVIMode();
#ifdef NXDK
    xbox_log("Main_SetVIMode() done\n");
#endif
    Lib_FillScreen(1);
    Main_Initialize();
#ifdef NXDK
    xbox_log("Main_Initialize() done\n");
    debugPrint("  Game initialized OK\n");
    debugPrint("  Starting main thread...\n");
#endif
    Main_ThreadEntry(NULL);
#ifdef NXDK
    xbox_log("Main_ThreadEntry() done, entering main loop\n");
    debugPrint("  Starting render loop!\n");
    int frameCount = 0;
#endif
    while (WindowIsRunning()) {
        push_frame();
#ifdef NXDK
        frameCount++;
        if (frameCount <= 5 || (frameCount % 60) == 0) {
            xbox_log("Frame %d\n", frameCount);
        }
#endif
    }
    GameEngine::Instance->Destroy();
    return 0;
}