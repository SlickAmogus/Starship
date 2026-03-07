// Minimal ImGui config stub for Xbox build
// ImGui is not used on Xbox, but gfx_rendering_api.h references ImTextureID
#pragma once

#ifndef ImTextureID
typedef void* ImTextureID;
#endif
