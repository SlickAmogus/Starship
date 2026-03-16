#include <libultraship.h>

#include <Fast3D/interpreter.h>
#include "Engine.h"

#ifdef NXDK
#include "xbox_debug.h"
#include <hal/debug.h>
#include <SDL.h>
#include <xboxkrnl/xboxkrnl.h>
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
#ifdef NXDK
    uint32_t t0 = SDL_GetTicks();
#endif
    GameEngine::ProcessGfxCommands(data);
#ifdef NXDK
    uint32_t t1 = SDL_GetTicks();
    static int gpfCount = 0;
    gpfCount++;
    if (gpfCount <= 20 || (gpfCount % 200) == 0) {
        xbox_log("GPF[%d]: render=%ums\n", gpfCount, t1 - t0);
    }
#endif
}

extern "C" void Timer_Update();

#ifdef NXDK
extern "C" u8 gVIsPerFrame;
extern "C" s32 gGameState;
#endif

void push_frame() {
#ifdef NXDK
    static uint32_t lastFrameTime = 0;
    static int pfCount = 0;
    uint32_t targetMs = (gVIsPerFrame > 0) ? (gVIsPerFrame * 1000 / 60) : 33;

    uint32_t t0 = SDL_GetTicks();
    if (lastFrameTime != 0) {
        uint32_t elapsed = t0 - lastFrameTime;
        if (elapsed < targetMs) {
            SDL_Delay(targetMs - elapsed);
        }
    }
    uint32_t t1 = SDL_GetTicks();
    lastFrameTime = t1;
#endif

    Graphics_ThreadUpdate();

#ifdef NXDK
    uint32_t t2 = SDL_GetTicks();
#endif

#ifdef NXDK
    // Xbox: process audio synchronously on main thread (no audio thread)
    GameEngine::ProcessAudioFrameSync();
    uint32_t t3 = SDL_GetTicks();
#else
    GameEngine::StartAudioFrame();
#endif

    GameEngine::Instance->StartFrame();
    Timer_Update();
    // thread5_iteration();
#ifndef NXDK
    GameEngine::EndAudioFrame();
#endif

#ifdef NXDK
    uint32_t t4 = SDL_GetTicks();
    pfCount++;
    if (pfCount <= 20 || (pfCount % 100) == 0) {
        xbox_log("PF[%d] delay=%u gfx=%u audio=%u rest=%u total=%u fps=%u\n",
                 pfCount, t1 - t0, t2 - t1, t3 - t2, t4 - t3, t4 - t0,
                 (t4 - t0) > 0 ? 1000 / (t4 - t0) : 999);
    }
#endif
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
    xbox_log("=== SDL_main entered (build 7) ===\n");
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
        if (frameCount <= 10 || (frameCount % 500) == 0) {
            MM_STATISTICS memStats;
            memStats.Length = sizeof(MM_STATISTICS);
            MmQueryStatistics(&memStats);
            xbox_log("F%d t=%u gs=%d avail=%uKB pool=%uKB\n",
                     frameCount, SDL_GetTicks(), (int)gGameState,
                     memStats.AvailablePages * 4,
                     memStats.PoolPagesCommitted * 4);
        }
#endif
    }
    GameEngine::Instance->Destroy();
    return 0;
}