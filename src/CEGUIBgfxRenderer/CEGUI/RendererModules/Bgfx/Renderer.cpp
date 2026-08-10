#include "CEGUI/RendererModules/Bgfx/Renderer.h"
#include "CEGUI/RendererModules/Bgfx/GeometryBuffer.h"
#include "CEGUI/RendererModules/Bgfx/Texture.h"
#include "CEGUI/RendererModules/Bgfx/TextureTarget.h"
#include "CEGUI/RendererModules/Bgfx/RenderTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"

#include <bx/math.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>

#ifndef CEGUI_BGFX_DEFAULT_SHADER_ROOT
#define CEGUI_BGFX_DEFAULT_SHADER_ROOT "Shaders/bgfx"
#endif

namespace CEGUI
{
String BgfxRenderer::d_rendererID(
    "CEGUI::BgfxRenderer - bgfx based renderer module (host-owned bgfx context)." );

BgfxRenderer& BgfxRenderer::bootstrapSystem(const Sizef& displaySize,
                                             bgfx::ViewId viewIdBase,
                                             uint16_t viewIdCount,
                                             const char* shaderRoot,
                                             const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("CEGUI::System object is already initialised."));

    BgfxRenderer& renderer = create(displaySize, viewIdBase, viewIdCount, shaderRoot, abi);
    DefaultResourceProvider* provider = new DefaultResourceProvider();
    try
    {
        System::create(renderer, provider);
    }
    catch (...)
    {
        delete provider;
        destroy(renderer);
        throw;
    }
    return renderer;
}

void BgfxRenderer::destroySystem()
{
    System* system = System::getSingletonPtr();
    if (!system)
        CEGUI_THROW(InvalidRequestException("CEGUI::System object is not created or was already destroyed."));

    BgfxRenderer* renderer = static_cast<BgfxRenderer*>(system->getRenderer());
    DefaultResourceProvider* provider = static_cast<DefaultResourceProvider*>(system->getResourceProvider());
    System::destroy();
    delete provider;
    destroy(*renderer);
}

BgfxRenderer& BgfxRenderer::create(const Sizef& displaySize,
                                   bgfx::ViewId viewIdBase,
                                   uint16_t viewIdCount,
                                   const char* shaderRoot,
                                   const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);
    return *new BgfxRenderer(displaySize, viewIdBase, viewIdCount, shaderRoot);
}

void BgfxRenderer::destroy(BgfxRenderer& renderer) { delete &renderer; }
RenderTarget& BgfxRenderer::getDefaultRenderTarget() { return *d_defaultTarget; }

GeometryBuffer& BgfxRenderer::createGeometryBuffer()
{
    BgfxGeometryBuffer* buffer = new BgfxGeometryBuffer(*this);
    d_geometryBuffers.push_back(buffer);
    return *buffer;
}

void BgfxRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator it = std::find(d_geometryBuffers.begin(), d_geometryBuffers.end(), &buffer);
    if (it != d_geometryBuffers.end())
    {
        d_geometryBuffers.erase(it);
        delete &buffer;
    }
}

void BgfxRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

TextureTarget* BgfxRenderer::createTextureTarget()
{
    TextureTarget* target = new BgfxTextureTarget(*this);
    d_textureTargets.push_back(target);
    return target;
}

void BgfxRenderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator it = std::find(d_textureTargets.begin(), d_textureTargets.end(), target);
    if (it != d_textureTargets.end())
    {
        d_textureTargets.erase(it);
        delete target;
    }
}

void BgfxRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

Texture& BgfxRenderer::createTexture(const String& name)
{
    throwIfNameExists(name);
    BgfxTexture* texture = new BgfxTexture(*this, name);
    d_textures[name] = texture;
    logTextureCreation(name);
    return *texture;
}

Texture& BgfxRenderer::createTexture(const String& name, const String& filename,
                                     const String& resourceGroup)
{
    throwIfNameExists(name);
    BgfxTexture* texture = new BgfxTexture(*this, name, filename, resourceGroup);
    d_textures[name] = texture;
    logTextureCreation(name);
    return *texture;
}

Texture& BgfxRenderer::createTexture(const String& name, const Sizef& size)
{
    throwIfNameExists(name);
    BgfxTexture* texture = new BgfxTexture(*this, name, size);
    d_textures[name] = texture;
    logTextureCreation(name);
    return *texture;
}

void BgfxRenderer::destroyTexture(Texture& texture) { destroyTexture(texture.getName()); }
void BgfxRenderer::destroyTexture(const String& name)
{
    TextureMap::iterator it = d_textures.find(name);
    if (it != d_textures.end())
    {
        logTextureDestruction(name);
        delete it->second;
        d_textures.erase(it);
    }
}
void BgfxRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

Texture& BgfxRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator it = d_textures.find(name);
    if (it == d_textures.end())
        CEGUI_THROW(UnknownObjectException("Texture does not exist: " + name));
    return *it->second;
}

bool BgfxRenderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
}

void BgfxRenderer::beginRendering() {}
void BgfxRenderer::endRendering() {}

void BgfxRenderer::setDisplaySize(const Sizef& size)
{
    if (size == d_displaySize)
        return;
    if (size.d_width <= 0.0f || size.d_height <= 0.0f)
        CEGUI_THROW(InvalidRequestException("CEGUI bgfx display size must be positive."));
    d_displaySize = size;
    Rectf area = d_defaultTarget->getArea();
    area.setSize(size);
    d_defaultTarget->setArea(area);
}

const Sizef& BgfxRenderer::getDisplaySize() const { return d_displaySize; }
const Vector2f& BgfxRenderer::getDisplayDPI() const { return d_displayDPI; }
uint BgfxRenderer::getMaxTextureSize() const
{
    const bgfx::Caps* caps = bgfx::getCaps();
    return caps ? static_cast<uint>(caps->limits.maxTextureSize) : 0u;
}
const String& BgfxRenderer::getIdentifierString() const { return d_rendererID; }
bool BgfxRenderer::isTexCoordSystemFlipped() const
{
    const bgfx::Caps* caps = bgfx::getCaps();
    return caps && caps->originBottomLeft;
}

bgfx::ViewId BgfxRenderer::acquireViewId()
{
    for (uint16_t i = 1; i < d_viewIdCount; ++i)
    {
        if (!d_viewIdsUsed[i])
        {
            d_viewIdsUsed[i] = true;
            return static_cast<bgfx::ViewId>(d_viewIdBase + i);
        }
    }
    CEGUI_THROW(RendererException("CEGUI bgfx renderer exhausted its reserved view-ID range."));
}

void BgfxRenderer::releaseViewId(bgfx::ViewId id)
{
    const uint32_t value = static_cast<uint32_t>(id);
    const uint32_t first = static_cast<uint32_t>(d_viewIdBase);
    const uint32_t end = first + static_cast<uint32_t>(d_viewIdCount);
    if (value < first || value >= end)
        return;
    const uint16_t index = static_cast<uint16_t>(id - d_viewIdBase);
    if (index != 0)
        d_viewIdsUsed[index] = false;
}

void BgfxRenderer::activateView(bgfx::ViewId id, bgfx::FrameBufferHandle framebuffer,
                                const Rectf& area, bool clear)
{
    d_currentViewId = id;
    const float maxU16 = static_cast<float>(std::numeric_limits<uint16_t>::max());
    const uint16_t x = static_cast<uint16_t>(std::min(maxU16, std::max(0.0f, area.left())));
    const uint16_t y = static_cast<uint16_t>(std::min(maxU16, std::max(0.0f, area.top())));
    const uint16_t width = static_cast<uint16_t>(std::min(maxU16, std::max(1.0f, area.getWidth())));
    const uint16_t height = static_cast<uint16_t>(std::min(maxU16, std::max(1.0f, area.getHeight())));

    bgfx::setViewMode(id, bgfx::ViewMode::Sequential);
    bgfx::setViewFrameBuffer(id, framebuffer);
    bgfx::setViewRect(id, x, y, width, height);

    float view[16];
    float projection[16];
    bx::mtxIdentity(view);
    const bgfx::Caps* caps = bgfx::getCaps();
    bx::mtxOrtho(projection, area.left(), area.right(), area.bottom(), area.top(),
                 -10000.0f, 10000.0f, 0.0f, caps && caps->homogeneousDepth);
    bgfx::setViewTransform(id, view, projection);

    bgfx::setViewClear(id, clear ? BGFX_CLEAR_COLOR : BGFX_CLEAR_NONE, 0x00000000u, 1.0f, 0);
    if (clear)
        bgfx::touch(id);
}

void BgfxRenderer::submitGeometry(bgfx::DynamicVertexBufferHandle vertexBuffer,
                                  uint32_t startVertex, uint32_t vertexCount,
                                  bgfx::TextureHandle texture, const float* modelMatrix,
                                  BlendMode blendMode, bool clipped,
                                  const Rectf& clipRect) const
{
    if (!bgfx::isValid(vertexBuffer) || vertexCount == 0 || !bgfx::isValid(d_program))
        return;

    bgfx::setTransform(modelMatrix);
    bgfx::setVertexBuffer(0, vertexBuffer, startVertex, vertexCount);
    bgfx::setTexture(0, d_textureSampler,
                     bgfx::isValid(texture) ? texture : d_whiteTexture);

    uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
    if (blendMode == BM_RTT_PREMULTIPLIED)
        state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA);
    else
        state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
    bgfx::setState(state);

    if (clipped)
    {
        const float maxU16 = static_cast<float>(std::numeric_limits<uint16_t>::max());
        const float left = std::min(maxU16, std::max(0.0f, clipRect.left()));
        const float top = std::min(maxU16, std::max(0.0f, clipRect.top()));
        const float width = std::min(maxU16, std::max(0.0f, clipRect.getWidth()));
        const float height = std::min(maxU16, std::max(0.0f, clipRect.getHeight()));
        bgfx::setScissor(static_cast<uint16_t>(left), static_cast<uint16_t>(top),
                         static_cast<uint16_t>(width), static_cast<uint16_t>(height));
    }
    else
        bgfx::setScissor(UINT16_MAX);

    bgfx::submit(d_currentViewId, d_program);
}

BgfxRenderer::BgfxRenderer(const Sizef& displaySize, bgfx::ViewId viewIdBase,
                           uint16_t viewIdCount, const char* shaderRoot) :
    d_displaySize(displaySize), d_displayDPI(96, 96), d_defaultTarget(0),
    d_program(BGFX_INVALID_HANDLE), d_textureSampler(BGFX_INVALID_HANDLE),
    d_whiteTexture(BGFX_INVALID_HANDLE), d_viewIdBase(viewIdBase),
    d_viewIdCount(viewIdCount), d_currentViewId(viewIdBase),
    d_shaderRoot(shaderRoot ? shaderRoot : "")
{
    if (displaySize.d_width <= 0.0f || displaySize.d_height <= 0.0f)
        CEGUI_THROW(InvalidRequestException("CEGUI bgfx display size must be positive."));
    if (viewIdCount == 0)
        CEGUI_THROW(InvalidRequestException("CEGUI bgfx requires at least one reserved view ID."));
    const bgfx::Caps* caps = bgfx::getCaps();
    if (!caps)
        CEGUI_THROW(RendererException("bgfx is not initialised. Initialise bgfx before creating CEGUI::BgfxRenderer."));
    if (static_cast<uint32_t>(viewIdBase) + viewIdCount > caps->limits.maxViews)
        CEGUI_THROW(InvalidRequestException("CEGUI bgfx view-ID range exceeds bgfx::Caps::limits.maxViews."));

    d_viewIdsUsed.resize(viewIdCount, false);
    d_viewIdsUsed[0] = true;

    try
    {
        initialiseResources();
        d_defaultTarget = new BgfxRenderTarget<>(*this, d_viewIdBase);
        d_defaultTarget->setArea(Rectf(Vector2f(0, 0), d_displaySize));
    }
    catch (...)
    {
        destroyResources();
        throw;
    }
}

BgfxRenderer::~BgfxRenderer()
{
    destroyAllTextureTargets();
    destroyAllTextures();
    destroyAllGeometryBuffers();
    delete d_defaultTarget;
    d_defaultTarget = 0;
    destroyResources();
}

void BgfxRenderer::initialiseResources()
{
    destroyResources();

    d_vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    bgfx::ShaderHandle vs = loadShader("vs_cegui.bin");
    bgfx::ShaderHandle fs = loadShader("fs_cegui.bin");
    if (!bgfx::isValid(vs) || !bgfx::isValid(fs))
    {
        if (bgfx::isValid(vs)) bgfx::destroy(vs);
        if (bgfx::isValid(fs)) bgfx::destroy(fs);
        CEGUI_THROW(RendererException("Unable to load compiled CEGUI bgfx shaders."));
    }

    d_program = bgfx::createProgram(vs, fs, true);
    if (!bgfx::isValid(d_program))
        CEGUI_THROW(RendererException("bgfx failed to link the CEGUI shader program."));

    d_textureSampler = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
    if (!bgfx::isValid(d_textureSampler))
    {
        destroyResources();
        CEGUI_THROW(RendererException("bgfx failed to create CEGUI texture sampler uniform."));
    }

    const uint32_t whitePixel = 0xffffffffu;
    d_whiteTexture = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
        bgfx::copy(&whitePixel, sizeof(whitePixel)));
    if (!bgfx::isValid(d_whiteTexture))
    {
        destroyResources();
        CEGUI_THROW(RendererException("bgfx failed to create CEGUI white fallback texture."));
    }
}

void BgfxRenderer::destroyResources()
{
    if (bgfx::isValid(d_whiteTexture)) bgfx::destroy(d_whiteTexture);
    if (bgfx::isValid(d_textureSampler)) bgfx::destroy(d_textureSampler);
    if (bgfx::isValid(d_program)) bgfx::destroy(d_program);
    d_whiteTexture = BGFX_INVALID_HANDLE;
    d_textureSampler = BGFX_INVALID_HANDLE;
    d_program = BGFX_INVALID_HANDLE;
}

std::string BgfxRenderer::getBackendDirectory() const
{
    switch (bgfx::getRendererType())
    {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: return "dx11";
    case bgfx::RendererType::OpenGL: return "glsl";
    case bgfx::RendererType::Vulkan: return "spirv";
    default: return std::string();
    }
}

bgfx::ShaderHandle BgfxRenderer::loadShader(const char* name) const
{
    const std::string backend = getBackendDirectory();
    if (backend.empty())
        CEGUI_THROW(RendererException("The active bgfx renderer backend has no compiled CEGUI shader set."));

    std::vector<std::filesystem::path> candidates;
    const auto addRoot = [&](const std::filesystem::path& root)
    {
        if (!root.empty()) candidates.push_back(root / backend / name);
    };
    if (!d_shaderRoot.empty()) addRoot(d_shaderRoot);
    addRoot(CEGUI_BGFX_DEFAULT_SHADER_ROOT);
    addRoot(std::filesystem::path("Shaders") / "bgfx");

    for (std::vector<std::filesystem::path>::const_iterator it = candidates.begin(); it != candidates.end(); ++it)
    {
        std::ifstream file(*it, std::ios::binary | std::ios::ate);
        if (!file) continue;
        const std::streamsize size = file.tellg();
        if (size <= 0 || size > static_cast<std::streamsize>(std::numeric_limits<uint32_t>::max()))
            continue;
        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> bytes(static_cast<size_t>(size));
        if (!file.read(reinterpret_cast<char*>(bytes.data()), size))
            continue;
        bgfx::ShaderHandle shader = bgfx::createShader(bgfx::copy(bytes.data(), static_cast<uint32_t>(bytes.size())));
        if (bgfx::isValid(shader))
        {
            bgfx::setName(shader, name);
            return shader;
        }
    }

    CEGUI_THROW(RendererException(String("Unable to locate CEGUI bgfx shader '") + name + "'."));
}

void BgfxRenderer::throwIfNameExists(const String& name) const
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException("[BgfxRenderer] Texture already exists: " + name));
}

void BgfxRenderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger) logger->logEvent("[BgfxRenderer] Created texture: " + name);
}
void BgfxRenderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger) logger->logEvent("[BgfxRenderer] Destroyed texture: " + name);
}
}
