// Xbox compatibility header - force-included before all source files.
//
// nxdk uses target i386-pc-win32, so _WIN32 and _MSC_VER are defined by clang.
// This activates Windows desktop code in libultraship (WASAPI, DXGI, WRL, COM).
//
// Strategy: Don't undef _WIN32 (it breaks nxdk's libc++), instead
// define XBOX-specific overrides that libultraship code can check.
//
// IMPORTANT: This header is force-included for ALL files including nxdk's own
// libraries. Keep it minimal - no standard header includes, and be careful with
// macros that could conflict with pdclib/libc++ declarations.

#pragma once

#ifdef NXDK

#ifndef PLATFORM_XBOX
#define PLATFORM_XBOX 1
#endif

// Prevent specific Windows-only features in libultraship.
#undef ENABLE_DX11
#undef ENABLE_OPENGL

// Block Windows-specific headers that don't exist on Xbox/nxdk
#define _WASAPI_AUDIO_PLAYER_H
#define __WRL_CLIENT_H__

// _TRUNCATE for MSVC compat (used by tinyxml2 etc.)
#define _TRUNCATE ((size_t)-1)

// SDL2 button constants not in nxdk's older SDL2
#ifndef SDL_CONTROLLER_BUTTON_MISC1
#define SDL_CONTROLLER_BUTTON_MISC1 15
#endif
#ifndef SDL_CONTROLLER_BUTTON_PADDLE1
#define SDL_CONTROLLER_BUTTON_PADDLE1 16
#endif
#ifndef SDL_CONTROLLER_BUTTON_PADDLE2
#define SDL_CONTROLLER_BUTTON_PADDLE2 17
#endif
#ifndef SDL_CONTROLLER_BUTTON_PADDLE3
#define SDL_CONTROLLER_BUTTON_PADDLE3 18
#endif
#ifndef SDL_CONTROLLER_BUTTON_PADDLE4
#define SDL_CONTROLLER_BUTTON_PADDLE4 19
#endif

// SDL hints not in nxdk SDL2
#ifndef SDL_HINT_JOYSTICK_THREAD
#define SDL_HINT_JOYSTICK_THREAD "SDL_JOYSTICK_THREAD"
#endif

// SDL2 functions not in nxdk's older SDL2
#ifndef SDL_GameControllerHasSensor
#define SDL_GameControllerHasSensor(gc, type) (0)
#endif
#ifndef SDL_GameControllerHasLED
#define SDL_GameControllerHasLED(gc) (0)
#endif
#ifndef SDL_GameControllerHasRumble
#define SDL_GameControllerHasRumble(gc) (0)
#endif

// MSVC secure CRT functions not in pdclib
#ifndef memcpy_s
#define memcpy_s(dest, destsz, src, count) memcpy(dest, src, count)
#endif

// miniz: skip time/utime features not available on nxdk
#define MINIZ_NO_TIME

#endif // NXDK
