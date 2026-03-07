// Stub: WRL (Windows Runtime Library) is not available on Xbox/nxdk
#pragma once

// Provide minimal stubs so WasapiAudioPlayer.h compiles but is non-functional
namespace Microsoft { namespace WRL {
    template<typename T>
    class ComPtr {
    public:
        ComPtr() : ptr(nullptr) {}
        T* Get() const { return ptr; }
        T** GetAddressOf() { return &ptr; }
        T** ReleaseAndGetAddressOf() { return &ptr; }
        void Reset() { ptr = nullptr; }
        T* operator->() const { return ptr; }
        operator bool() const { return ptr != nullptr; }
    private:
        T* ptr;
    };
}}
