# Starship Xbox Port - Technical Notes

## Architecture Overview

The port works by implementing two new backends for libultraship's Fast3D renderer:

1. **GfxRenderingAPINV2A** (`gfx_nv2a.cpp`) - Implements the `GfxRenderingAPI` interface
   using the Xbox NV2A GPU via nxdk's pbkit library (push buffer GPU commands).

2. **GfxWindowBackendNXDK** (`gfx_nxdk_window.cpp`) - Implements the `GfxWindowBackend`
   interface for Xbox's fixed display output, using SDL2 for controller input.

## Build System

The Xbox build uses nxdk's Makefile-based build system instead of CMake.
Run `make` from the `xbox/` directory with `NXDK_DIR` set to your nxdk installation.

## Key Technical Challenges

### 1. NV2A Register Combiners (CRITICAL)

The N64 RDP uses a color combiner with the formula: `(A - B) * C + D`
The NV2A has register combiners (NV20-class) that can emulate this.

Fast3D generates shader programs dynamically based on N64 display list commands.
Each unique color combiner configuration gets a shader ID pair (shaderId0, shaderId1).
The OpenGL backend compiles GLSL shaders on the fly.

For NV2A, we must map these to register combiner configurations:
- Parse CCFeatures to determine what inputs are used
- Configure the appropriate number of combiner stages
- Map N64 color combiner inputs to NV2A register combiner inputs

### 2. Texture Management

- NV2A requires textures in GPU-accessible contiguous memory
- Texture swizzling improves GPU cache performance (optional but recommended)
- Xbox has ~64MB RAM total - need to manage texture cache carefully
- N64 textures are small (typically 32x32 to 64x64) which helps

### 3. Memory Constraints

- 64MB shared RAM between CPU and GPU
- Starship PC version may use significantly more
- May need to reduce texture resolution or implement streaming
- Consider using Xbox HDD for texture cache

### 4. CPU Constraints

- 733MHz Pentium III - no SSE2, only SSE1
- The sse2neon.h dependency needs to be replaced/removed
- Some math operations may need optimization

### 5. Missing Dependencies

Libraries that need porting or stubbing for Xbox:
- **spdlog**: Replace with debugPrint or stub
- **ImGui**: Stub out (no debug UI on Xbox)
- **libzip/libpng**: May need nxdk-compatible builds
- **tinyxml2**: Should compile as-is (pure C++)
- **nlohmann-json**: Should compile as-is (header-only C++)
- **SDL2**: Available in nxdk
- **libogg/libvorbis**: Need cross-compilation for Xbox

### 6. File I/O

Xbox file paths use drive letters:
- `D:\` - DVD drive (read-only, game disc)
- `E:\` - HDD partition (read-write)
- `C:\` - System partition
- `F:\` and `G:\` - Additional HDD partitions

The .o2r asset files should be placed on the HDD or included on the disc.

## Implementation Phases

### Phase 1: Skeleton (DONE)
- [x] Backend interface implementations
- [x] Build system scaffolding
- [x] Xbox entry point

### Phase 2: Basic Rendering
- [ ] pbkit initialization and frame buffer setup
- [ ] Simple register combiner config (texture * vertex color)
- [ ] Texture upload to GPU memory
- [ ] Inline vertex submission
- [ ] Clear screen and depth buffer

### Phase 3: Full Color Combiner
- [ ] Map all N64 CC modes to register combiner configs
- [ ] Two-cycle mode support
- [ ] Fog support
- [ ] Alpha blending

### Phase 4: System Integration
- [ ] Stub/port spdlog
- [ ] Stub ImGui
- [ ] Port or stub resource loading (libzip)
- [ ] Controller mapping (Xbox controller -> N64 controller)
- [ ] Audio via SDL2

### Phase 5: Optimization
- [ ] Texture swizzling
- [ ] Vertex buffer objects (instead of inline)
- [ ] Memory management / texture cache
- [ ] Frame rate optimization
