// Stub: imgui_internal.h for Xbox build
#pragma once
#include "imgui.h"

struct ImRect {
    ImVec2 Min, Max;
    ImRect() {}
    ImRect(const ImVec2& min, const ImVec2& max) : Min(min), Max(max) {}
};
