# Starship Xbox Port - nxdk Makefile
#
# Prerequisites:
#   - nxdk installed and NXDK_DIR environment variable set
#   - OR nxdk cloned alongside this repo at ../nxdk
#
# Usage:
#   make            - Build the Xbox executable
#   make clean      - Clean build artifacts
#   make iso        - Build and create an Xbox ISO

# Locate nxdk
NXDK_DIR ?= $(realpath ../nxdk)
ifeq ($(wildcard $(NXDK_DIR)/Makefile),)
$(error Cannot find nxdk. Set NXDK_DIR or clone nxdk alongside the Starship directory)
endif

# Project settings
XBE_TITLE = "Starship"
GEN_XISO = starship.iso
NXDK_SDL = y
NXDK_CXX = y

# Source root
STARSHIP_ROOT = $(realpath ..)
LUS_ROOT = $(STARSHIP_ROOT)/libultraship

# ============================================================================
# Starship game sources (C)
# ============================================================================
SRCS += $(wildcard $(STARSHIP_ROOT)/src/audio/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/engine/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/overlays/**/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/sys/*.c)
SRCS += $(STARSHIP_ROOT)/src/buffers.c
SRCS += $(STARSHIP_ROOT)/src/dmatable.c
SRCS += $(STARSHIP_ROOT)/src/libc_math64.c
SRCS += $(STARSHIP_ROOT)/src/libc_sprintf.c
SRCS += $(STARSHIP_ROOT)/src/libultra/io/aisetfreq.c
SRCS += $(STARSHIP_ROOT)/src/libultra/gu/sqrtf.c
SRCS += $(STARSHIP_ROOT)/src/libultra/gu/mtxutil.c
SRCS += $(STARSHIP_ROOT)/src/libultra/gu/ortho.c

# Starship port layer (C/C++)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/*.cpp)

# Xbox-specific sources
SRCS += $(wildcard xbox_src/*.c)
SRCS += $(wildcard xbox_src/*.cpp)
SRCS += xbox_src/tinyxml2.cpp

# miniz (ZIP reading for O2R archives)
SRCS += xbox_src/miniz.c
SRCS += xbox_src/miniz_tdef.c
SRCS += xbox_src/miniz_tinfl.c
SRCS += xbox_src/miniz_zip.c

# libc++ filesystem stubs (nxdk doesn't support std::filesystem)
# Stub implementations are in xbox_stubs.cpp

# ============================================================================
# Exclusions (files not compatible with Xbox or not needed)
# ============================================================================
SRCS := $(filter-out %fox_colheaders.c, $(SRCS))
SRCS := $(filter-out %fox_edata_info.c, $(SRCS))
SRCS := $(filter-out %fox_rcp_setup.c, $(SRCS))
SRCS := $(filter-out %fox_load_inits.c, $(SRCS))
SRCS := $(filter-out %fox_end2_data.c, $(SRCS))
SRCS := $(filter-out %sys_timer.c, $(SRCS))
SRCS := $(filter-out %sys_fault.c, $(SRCS))
SRCS := $(filter-out %object_ram.c, $(SRCS))

# ============================================================================
# libultraship sources
# ============================================================================

# NV2A rendering backend
SRCS += $(LUS_ROOT)/src/graphic/Fast3D/backends/gfx_nv2a.cpp
SRCS += $(LUS_ROOT)/src/graphic/Fast3D/backends/gfx_nxdk_window.cpp
SRCS += $(LUS_ROOT)/src/graphic/Fast3D/interpreter.cpp
SRCS += $(LUS_ROOT)/src/graphic/Fast3D/Fast3dWindow.cpp
SRCS += $(LUS_ROOT)/src/graphic/Fast3D/debug/GfxDebugger.cpp

# libultraship core
SRCS += $(LUS_ROOT)/src/Context.cpp
SRCS += $(wildcard $(LUS_ROOT)/src/config/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/debug/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/log/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/utils/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/window/*.cpp)

# Resource system
SRCS += $(wildcard $(LUS_ROOT)/src/resource/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/resource/type/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/resource/factory/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/resource/archive/*.cpp)

# Public bridge (C API wrappers)
SRCS += $(wildcard $(LUS_ROOT)/src/public/bridge/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/public/libultra/*.cpp)

# Audio (SDL audio player only, not WASAPI)
SRCS += $(LUS_ROOT)/src/audio/Audio.cpp
SRCS += $(LUS_ROOT)/src/audio/AudioPlayer.cpp
SRCS += $(LUS_ROOT)/src/audio/SDLAudioPlayer.cpp

# Binary tools (BinaryReader, MemoryStream, etc.)
SRCS += $(wildcard $(LUS_ROOT)/src/utils/binarytools/*.cpp)

# Glob utility
SRCS += $(LUS_ROOT)/src/utils/glob.c

# Controller
SRCS += $(LUS_ROOT)/src/controller/controldevice/ControlDevice.cpp
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/mapping/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/mapping/keyboard/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/mapping/sdl/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/mapping/factories/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldevice/controller/mapping/mouse/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/controldeck/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/deviceindex/*.cpp)
SRCS += $(wildcard $(LUS_ROOT)/src/controller/physicaldevice/*.cpp)

# GUI stubs (minimal)
SRCS += $(wildcard $(LUS_ROOT)/src/window/gui/*.cpp)

# Exclude Windows-specific files
SRCS := $(filter-out %WasapiAudioPlayer.cpp, $(SRCS))
SRCS := $(filter-out %gfx_dxgi.cpp, $(SRCS))
SRCS := $(filter-out %gfx_direct3d11.cpp, $(SRCS))
SRCS := $(filter-out %gfx_direct3d_common.cpp, $(SRCS))
SRCS := $(filter-out %gfx_opengl.cpp, $(SRCS))
SRCS := $(filter-out %gfx_sdl2.cpp, $(SRCS))
SRCS := $(filter-out %gfx_metal.cpp, $(SRCS))
SRCS := $(filter-out %gfx_metal_shader.cpp, $(SRCS))
SRCS := $(filter-out %CrashHandler.cpp, $(SRCS))
SRCS := $(filter-out %StatsWindow.cpp, $(SRCS))

# Exclude GUI files - ImGui is not available on Xbox
SRCS := $(filter-out %/gui/Gui.cpp, $(SRCS))
SRCS := $(filter-out %/gui/GuiWindow.cpp, $(SRCS))
SRCS := $(filter-out %/gui/ConsoleWindow.cpp, $(SRCS))
SRCS := $(filter-out %/gui/GameOverlay.cpp, $(SRCS))
SRCS := $(filter-out %/gui/GfxDebuggerWindow.cpp, $(SRCS))
SRCS := $(filter-out %/gui/InputEditorWindow.cpp, $(SRCS))
SRCS := $(filter-out %FileDropMgr.cpp, $(SRCS))

# Exclude Starship port UI (requires ImGui)
SRCS := $(filter-out %ImguiUI.cpp, $(SRCS))
SRCS := $(filter-out %ResolutionEditor.cpp, $(SRCS))
SRCS := $(filter-out %UIWidgets.cpp, $(SRCS))
SRCS := $(filter-out %Notification.cpp, $(SRCS))

# Exclude SDL controller features not in nxdk SDL2
SRCS := $(filter-out %SDLGyroMapping.cpp, $(SRCS))
SRCS := $(filter-out %SDLLEDMapping.cpp, $(SRCS))
SRCS := $(filter-out %GyroMappingFactory.cpp, $(SRCS))
SRCS := $(filter-out %LEDMappingFactory.cpp, $(SRCS))

# Starship port extras
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/interpolation/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/mods/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/mods/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/patches/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/patches/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/importers/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/importers/audio/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/loaders/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/type/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/resource/type/audio/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/hooks/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/hooks/*.c)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/hooks/impl/*.cpp)
SRCS += $(wildcard $(STARSHIP_ROOT)/src/port/hooks/impl/*.c)
# UI files excluded - require ImGui (ImguiUI, ResolutionEditor, UIWidgets)
# Notification excluded - requires ImGui

# ============================================================================
# Compiler flags
# ============================================================================
# Xbox stubs directory MUST come first so our imgui/spdlog stubs take priority
# Use -iquote for quoted includes and -I for angle-bracket includes
CFLAGS += -iquote$(CURDIR)/xbox_src/stubs
CFLAGS += -I$(CURDIR)/xbox_src
CFLAGS += -I$(STARSHIP_ROOT)
CFLAGS += -I$(STARSHIP_ROOT)/include
CFLAGS += -I$(STARSHIP_ROOT)/src
CFLAGS += -I$(LUS_ROOT)/include
CFLAGS += -I$(LUS_ROOT)/include/libultraship
CFLAGS += -I$(LUS_ROOT)/src
CFLAGS += -I$(LUS_ROOT)/src/graphic
CFLAGS += -I$(LUS_ROOT)/src/graphic/Fast3D
CFLAGS += -I$(LUS_ROOT)/src/resource
CFLAGS += -I$(LUS_ROOT)/src/resource/type
CFLAGS += -I$(LUS_ROOT)/src/resource/factory
CFLAGS += -I$(LUS_ROOT)/src/public
CFLAGS += -I$(LUS_ROOT)/src/public/bridge
CFLAGS += -I$(LUS_ROOT)/src/public/libultra
CFLAGS += -I$(LUS_ROOT)/src/window
CFLAGS += -I$(LUS_ROOT)/src/window/gui
CFLAGS += -I$(LUS_ROOT)/src/config
CFLAGS += -I$(LUS_ROOT)/src/utils
CFLAGS += -I$(LUS_ROOT)/src/utils/binarytools
CFLAGS += -I$(LUS_ROOT)/src/audio
CFLAGS += -I$(LUS_ROOT)/src/log
CFLAGS += -I$(LUS_ROOT)/src/debug
CFLAGS += -I$(LUS_ROOT)/src/menu
CFLAGS += -I$(LUS_ROOT)/extern
CFLAGS += -I$(LUS_ROOT)/extern/tinyxml2

# Xbox-specific defines
# nxdk's clang target (i386-pc-win32) defines _WIN32, which activates
# Windows-specific code paths (WASAPI, DXGI, WRL). We use a force-include
# header to gate these with NXDK checks.
CFLAGS += -include $(CURDIR)/xbox_src/xbox_compat.h
CFLAGS += -DENABLE_NV2A=1
CFLAGS += -DVERSION_US=1
CFLAGS += -DENABLE_RUMBLE=1
CFLAGS += -DF3DEX_GBI=1
CFLAGS += -D_LANGUAGE_C
CFLAGS += -D_USE_MATH_DEFINES
CFLAGS += -DNON_MATCHING=1
CFLAGS += -DNON_EQUIVALENT=1
CFLAGS += -DAVOID_UB=1
CFLAGS += -DGBI_FLOATS=1
CFLAGS += -DPLATFORM_XBOX=1
CFLAGS += -DDR_WAV_NO_STDIO=1
CFLAGS += -DDR_MP3_NO_STDIO=1
CFLAGS += -DDR_MP3_NO_SIMD=1

# libultraship CVARs (normally set by CMake)
CFLAGS += '-DCVAR_VSYNC_ENABLED="gVsyncEnabled"'
CFLAGS += '-DCVAR_Z_FIGHTING_MODE="gZFightingMode"'
CFLAGS += '-DCVAR_INTERNAL_RESOLUTION="gInternalResolution"'
CFLAGS += '-DCVAR_MSAA_VALUE="gMSAAValue"'
CFLAGS += '-DCVAR_SDL_WINDOWED_FULLSCREEN="gSdlWindowedFullscreen"'
CFLAGS += '-DCVAR_TEXTURE_FILTER="gTextureFilter"'
CFLAGS += '-DCVAR_IMGUI_CONTROLLER_NAV="gControlNav"'
CFLAGS += '-DCVAR_CONSOLE_WINDOW_OPEN="gConsoleEnabled"'
CFLAGS += '-DCVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN="gControllerConfigurationEnabled"'
CFLAGS += '-DCVAR_CONTROLLER_DISCONNECTED_WINDOW_OPEN="gControllerDisconnectedWindowEnabled"'
CFLAGS += '-DCVAR_CONTROLLER_REORDERING_WINDOW_OPEN="gControllerReorderingWindowEnabled"'
CFLAGS += '-DCVAR_GFX_DEBUGGER_WINDOW_OPEN="gGfxDebuggerEnabled"'
CFLAGS += '-DCVAR_STATS_WINDOW_OPEN="gStatsEnabled"'
CFLAGS += '-DCVAR_ENABLE_MULTI_VIEWPORTS="gEnableMultiViewports"'
CFLAGS += '-DCVAR_LOW_RES_MODE="gLowResMode"'
CFLAGS += '-DCVAR_SIMULATED_INPUT_LAG="gSimulatedInputLag"'
CFLAGS += '-DCVAR_GAME_OVERLAY_FONT="gOverlayFont"'
CFLAGS += '-DCVAR_MENU_BAR_OPEN="gOpenMenuBar"'
CFLAGS += '-DCVAR_PREFIX_CONTROLLERS="gControllers"'
CFLAGS += '-DCVAR_PREFIX_ADVANCED_RESOLUTION="gAdvancedResolution"'
CFLAGS += '-DCVAR_AUDIO_CHANNELS_SETTING="gAudioChannelsSetting"'

# Suppress warnings for N64 decomp code
CFLAGS += -Wno-return-type
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -Wno-parentheses
CFLAGS += -Wno-narrowing
CFLAGS += -Wno-missing-braces
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-incompatible-function-pointer-types
CFLAGS += -fexceptions

CXXFLAGS += $(CFLAGS)
CXXFLAGS += -std=c++20
CXXFLAGS += -fpermissive
CXXFLAGS += -fexceptions

include $(NXDK_DIR)/Makefile
