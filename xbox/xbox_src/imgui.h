// Minimal ImGui stub for Xbox build
// ImGui is not used on Xbox - provides type definitions needed by libultraship
#pragma once

#include "imconfig.h"
#include <cstddef>

// Minimal types that libultraship headers reference
typedef unsigned int ImGuiID;
typedef int ImGuiKey;
typedef int ImGuiCol;
typedef int ImGuiCond;
typedef int ImGuiDir;
typedef int ImGuiStyleVar;
typedef int ImGuiWindowFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiTreeNodeFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiPopupFlags;
typedef int ImGuiTableFlags;
typedef int ImGuiTableColumnFlags;
typedef int ImGuiTableRowFlags;
typedef int ImGuiSortDirection;
typedef int ImGuiSliderFlags;
typedef int ImGuiColorEditFlags;
typedef unsigned short ImWchar;
typedef unsigned int ImU32;

struct ImVec2 {
    float x, y;
    ImVec2() : x(0), y(0) {}
    ImVec2(float _x, float _y) : x(_x), y(_y) {}
};

struct ImVec4 {
    float x, y, z, w;
    ImVec4() : x(0), y(0), z(0), w(0) {}
    ImVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

struct ImFont {
    float FontSize;
};

struct ImGuiIO {
    ImVec2 DisplaySize;
    float DeltaTime;
    float Framerate;
    ImFont* FontDefault;
    bool WantCaptureMouse;
    bool WantCaptureKeyboard;
};

struct ImDrawVert {
    ImVec2 pos;
    ImVec2 uv;
    ImU32 col;
};

struct ImDrawCmd {
    unsigned int ElemCount;
    ImVec4 ClipRect;
    ImTextureID TextureId;
};

struct ImDrawList {};

struct ImDrawData {
    int CmdListsCount;
    int TotalIdxCount;
    int TotalVtxCount;
    ImVec2 DisplayPos;
    ImVec2 DisplaySize;
};

struct ImGuiContext {};

struct ImGuiInputTextCallbackData {
    int EventFlag;
    int EventChar;
    int EventKey;
    char* Buf;
    int BufTextLen;
    int BufSize;
    int CursorPos;
};

template<typename T>
struct ImVector {
    int Size;
    int Capacity;
    T* Data;
    ImVector() : Size(0), Capacity(0), Data(nullptr) {}
    int size() const { return Size; }
    T& operator[](int i) { return Data[i]; }
    const T& operator[](int i) const { return Data[i]; }
};

// Minimal ImGui namespace stubs
namespace ImGui {
    inline ImGuiContext* GetCurrentContext() { return nullptr; }
    inline ImGuiIO& GetIO() { static ImGuiIO io; return io; }
    inline void PushStyleColor(int, ImU32) {}
    inline void PopStyleColor(int = 1) {}
}
