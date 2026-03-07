// Xbox stub implementations for excluded source files
// Provides minimal implementations to satisfy the linker.
#ifdef NXDK

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

// ============================================================================
// Ship::CrashHandler stubs
// ============================================================================
#include "debug/CrashHandler.h"

namespace Ship {

CrashHandler::CrashHandler() {
    mOutBuffer = new char[gMaxBufferSize];
}

CrashHandler::CrashHandler(CrashHandlerCallback callback) {
    mCallback = callback;
    mOutBuffer = new char[gMaxBufferSize];
}

CrashHandler::~CrashHandler() {
    delete[] mOutBuffer;
}

void CrashHandler::RegisterCallback(CrashHandlerCallback callback) {
    mCallback = callback;
}

bool CrashHandler::CheckStrLen(const char* str) {
    return (strlen(str) + mOutBuffersize < gMaxBufferSize);
}

void CrashHandler::AppendStrTrunc(const char* str) {
    while (mOutBuffersize < gMaxBufferSize - 1 && *str) {
        mOutBuffer[mOutBuffersize++] = *str++;
    }
    mOutBuffer[mOutBuffersize] = '\0';
}

void CrashHandler::AppendStr(const char* str) {
    if (!CheckStrLen(str)) { AppendStrTrunc(str); return; }
    while (*str) mOutBuffer[mOutBuffersize++] = *str++;
}

void CrashHandler::AppendLine(const char* str) {
    AppendStr(str);
    if (mOutBuffersize < gMaxBufferSize - 1) mOutBuffer[mOutBuffersize++] = '\n';
}

void CrashHandler::PrintCommon() {
    if (mCallback) mCallback(mOutBuffer, &mOutBuffersize);
}

} // namespace Ship

// ============================================================================
// Ship::FileDropMgr stubs
// ============================================================================
#include "window/FileDropMgr.h"

namespace Ship {
FileDropMgr::~FileDropMgr() {}
} // namespace Ship

// ============================================================================
// Ship::Gui stubs
// ============================================================================
namespace Ship {

class GuiWindow;

class Window;

class Gui {
public:
    Gui();
    Gui(std::vector<std::shared_ptr<GuiWindow>> guiWindows);
    virtual ~Gui();
    bool GetMenuOrMenubarVisible();
    void StartDraw();
    void EndDraw();
    void ShutDownImGui(Window* w);
    bool IsMouseOverActivePopup();
    bool IsMouseOverAnyGuiItem();
    void SaveConsoleVariablesNextFrame();
};

Gui::Gui() {}
Gui::Gui(std::vector<std::shared_ptr<GuiWindow>> guiWindows) { (void)guiWindows; }
Gui::~Gui() {}
bool Gui::GetMenuOrMenubarVisible() { return false; }
void Gui::StartDraw() {}
void Gui::EndDraw() {}
void Gui::ShutDownImGui(Window* w) { (void)w; }
bool Gui::IsMouseOverActivePopup() { return false; }
bool Gui::IsMouseOverAnyGuiItem() { return false; }
void Gui::SaveConsoleVariablesNextFrame() {}

// ============================================================================
// Ship::GuiWindow stubs
// ============================================================================
class GuiWindow {
public:
    virtual void Draw();
protected:
    virtual void SetVisibility(bool visible);
};

void GuiWindow::Draw() {}
void GuiWindow::SetVisibility(bool visible) { (void)visible; }

// ============================================================================
// Ship::GyroMappingFactory stubs
// ============================================================================
} // namespace Ship

#include "controller/controldevice/controller/mapping/factories/GyroMappingFactory.h"
#include "controller/controldevice/controller/mapping/factories/LEDMappingFactory.h"

namespace Ship {

std::shared_ptr<ControllerGyroMapping> GyroMappingFactory::CreateGyroMappingFromConfig(uint8_t portIndex, std::string id) {
    (void)portIndex; (void)id; return nullptr;
}

std::shared_ptr<ControllerGyroMapping> GyroMappingFactory::CreateGyroMappingFromSDLInput(uint8_t portIndex) {
    (void)portIndex; return nullptr;
}

std::shared_ptr<ControllerLEDMapping> LEDMappingFactory::CreateLEDMappingFromConfig(uint8_t portIndex, std::string id) {
    (void)portIndex; (void)id; return nullptr;
}

std::shared_ptr<ControllerLEDMapping> LEDMappingFactory::CreateLEDMappingFromSDLInput(uint8_t portIndex) {
    (void)portIndex; return nullptr;
}

} // namespace Ship

// ============================================================================
// MSVC C++ EH runtime stubs
// nxdk's clang generates MSVC-style EH calls due to i386-pc-win32 target.
// nxdk doesn't provide the MSVC C++ runtime, so we stub these.
// ============================================================================
extern "C" {

void __std_terminate() {
    abort();
}

void __stdcall _CxxThrowException(void* pExceptionObject, void* pThrowInfo) {
    (void)pExceptionObject;
    (void)pThrowInfo;
    abort();
}

void* __RTDynamicCast(void* inptr, long VfDelta, void* SrcType, void* TargetType, int isReference) {
    (void)VfDelta; (void)SrcType; (void)TargetType; (void)isReference;
    return inptr;
}

int __CxxFrameHandler3(void* pExcept, void* pRN, void* pContext, void* pDC) {
    (void)pExcept; (void)pRN; (void)pContext; (void)pDC;
    abort();
    return 0;
}

} // extern "C"

// ============================================================================
// std::filesystem stubs
// nxdk libc++ has the <filesystem> header but no implementation.
// Xbox uses drive letters (D:\, E:\, etc.) for file access via nxdk's
// windows-compatible file APIs. We provide minimal stubs here.
// ============================================================================
#include <filesystem>
#include <system_error>

_LIBCPP_BEGIN_NAMESPACE_FILESYSTEM

file_status __status(const path& p, error_code* ec) {
    if (ec) ec->clear();
    // Return "not found" by default
    return file_status(file_type::not_found);
}

bool __create_directories(const path& p, error_code* ec) {
    if (ec) ec->clear();
    // Xbox: use CreateDirectoryA from nxdk's winapi
    // For now, return false (directory creation not supported)
    return false;
}

bool __remove(const path& p, error_code* ec) {
    if (ec) ec->clear();
    return false;
}

// recursive_directory_iterator stubs
class __dir_stream {
public:
    path __root_;
    bool __done_ = true;
    directory_entry __entry_;
};

recursive_directory_iterator::recursive_directory_iterator(const path& p, directory_options opts, error_code* ec) {
    if (ec) ec->clear();
    // Empty iterator - no files to iterate
}

const directory_entry& recursive_directory_iterator::__dereference() const {
    static directory_entry empty;
    return empty;
}

recursive_directory_iterator& recursive_directory_iterator::__increment(error_code* ec) {
    if (ec) ec->clear();
    // Make this an end iterator
    *this = recursive_directory_iterator();
    return *this;
}

// path::__filename() stub
string_view path::__filename() const {
    auto& s = native();
    auto pos = s.find_last_of("/\\");
    if (pos == string_type::npos) return string_view(s);
    return string_view(s).substr(pos + 1);
}

// __absolute() stub
path __absolute(const path& p, error_code* ec) {
    if (ec) ec->clear();
    // On Xbox, paths are already absolute (D:\, E:\, etc.)
    return p;
}

// path::__extension() stub
string_view path::__extension() const {
    auto name = filename().native();
    auto pos = name.rfind('.');
    if (pos == string_type::npos || pos == 0) return {};
    return string_view(name).substr(pos);
}

_LIBCPP_END_NAMESPACE_FILESYSTEM

#endif // NXDK
