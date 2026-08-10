#include "CEGUI/RendererModules/Bgfx/TextureTarget.h"
#include "CEGUI/RendererModules/Bgfx/Texture.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Exceptions.h"

#include <algorithm>

namespace CEGUI
{
uint BgfxTextureTarget::s_textureNumber = 0;
const float BgfxTextureTarget::DEFAULT_SIZE = 128.0f;

BgfxTextureTarget::BgfxTextureTarget(BgfxRenderer& owner) :
    BgfxRenderTarget<TextureTarget>(owner, owner.acquireViewId()),
    d_ceguiTexture(0), d_renderTexture(BGFX_INVALID_HANDLE)
{
    try
    {
        d_ceguiTexture = &static_cast<BgfxTexture&>(
            d_owner.createTexture(generateTextureName()));
        declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
    }
    catch (...)
    {
        cleanupRenderTexture();
        if (d_ceguiTexture)
        {
            d_owner.destroyTexture(*d_ceguiTexture);
            d_ceguiTexture = 0;
        }
        d_owner.releaseViewId(d_viewId);
        throw;
    }
}

BgfxTextureTarget::~BgfxTextureTarget()
{
    cleanupRenderTexture();
    if (d_ceguiTexture)
        d_owner.destroyTexture(*d_ceguiTexture);
    d_owner.releaseViewId(d_viewId);
}

bool BgfxTextureTarget::isImageryCache() const
{
    return true;
}

void BgfxTextureTarget::clear()
{
    d_clearPending = true;
}

Texture& BgfxTextureTarget::getTexture() const
{
    return *d_ceguiTexture;
}

void BgfxTextureTarget::declareRenderSize(const Sizef& size)
{
    if (size.d_width <= 0.0f || size.d_height <= 0.0f)
        CEGUI_THROW(InvalidRequestException("bgfx texture target size must be positive."));
    const uint maxSize = d_owner.getMaxTextureSize();
    if (size.d_width > 65535.0f || size.d_height > 65535.0f ||
        (maxSize != 0u && (size.d_width > maxSize || size.d_height > maxSize)))
        CEGUI_THROW(InvalidRequestException("bgfx texture target size exceeds the renderer limit."));

    if (d_area.getWidth() >= size.d_width && d_area.getHeight() >= size.d_height)
        return;

    setArea(Rectf(Vector2f(0, 0), size));
    cleanupRenderTexture();
    initialiseRenderTexture();
    clear();
}

bool BgfxTextureTarget::isRenderingInverted() const
{
    const bgfx::Caps* caps = bgfx::getCaps();
    return caps && caps->originBottomLeft;
}

void BgfxTextureTarget::cleanupRenderTexture()
{
    if (d_ceguiTexture)
        d_ceguiTexture->clearRenderTargetTexture();

    if (bgfx::isValid(d_framebuffer))
    {
        bgfx::destroy(d_framebuffer);
        d_framebuffer = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(d_renderTexture))
    {
        bgfx::destroy(d_renderTexture);
        d_renderTexture = BGFX_INVALID_HANDLE;
    }
}

void BgfxTextureTarget::initialiseRenderTexture()
{
    const uint16_t width = static_cast<uint16_t>(std::max(1.0f, d_area.getWidth()));
    const uint16_t height = static_cast<uint16_t>(std::max(1.0f, d_area.getHeight()));
    const uint64_t flags = BGFX_TEXTURE_RT |
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP;

    d_renderTexture = bgfx::createTexture2D(width, height, false, 1,
        bgfx::TextureFormat::RGBA8, flags);
    if (!bgfx::isValid(d_renderTexture))
        CEGUI_THROW(RendererException("bgfx failed to create CEGUI render-target texture."));

    d_framebuffer = bgfx::createFrameBuffer(1, &d_renderTexture, false);
    if (!bgfx::isValid(d_framebuffer))
    {
        bgfx::destroy(d_renderTexture);
        d_renderTexture = BGFX_INVALID_HANDLE;
        CEGUI_THROW(RendererException("bgfx failed to create CEGUI render-target framebuffer."));
    }

    d_ceguiTexture->setRenderTargetTexture(d_renderTexture, d_area.getSize());
}

String BgfxTextureTarget::generateTextureName()
{
    String name("_bgfx_tt_tex_");
    name.append(PropertyHelper<uint>::toString(s_textureNumber++));
    return name;
}
}
