#ifndef _CEGUIBgfxTexture_h_
#define _CEGUIBgfxTexture_h_

#include "CEGUI/Texture.h"
#include "CEGUI/RendererModules/Bgfx/Renderer.h"
#include "CEGUI/Size.h"
#include "CEGUI/Vector.h"

#include <bgfx/bgfx.h>
#include <vector>

namespace CEGUI
{
class BGFX_GUIRENDERER_API BgfxTexture : public Texture
{
public:
    const String& getName() const;
    const Sizef& getSize() const;
    const Sizef& getOriginalDataSize() const;
    const Vector2f& getTexelScaling() const;
    void loadFromFile(const String& filename, const String& resourceGroup);
    void loadFromMemory(const void* buffer, const Sizef& bufferSize,
                        PixelFormat pixelFormat);
    void blitFromMemory(const void* sourceData, const Rectf& area);
    void blitToMemory(void* targetData);
    bool isPixelFormatSupported(PixelFormat fmt) const;

    bgfx::TextureHandle getHandle() const { return d_texture; }
    void setRenderTargetTexture(bgfx::TextureHandle handle, const Sizef& size);
    void clearRenderTargetTexture();
    void setOriginalDataSize(const Sizef& size);

protected:
    friend class BgfxRenderer;
    friend class BgfxTextureTarget;

    BgfxTexture(BgfxRenderer& owner, const String& name);
    BgfxTexture(BgfxRenderer& owner, const String& name,
                const String& filename, const String& resourceGroup);
    BgfxTexture(BgfxRenderer& owner, const String& name, const Sizef& size);
    ~BgfxTexture();

private:
    void destroyTexture();
    void createEmptyTexture(const Sizef& size, PixelFormat format,
                            uint64_t extraFlags = BGFX_TEXTURE_NONE);
    void updateCachedScaleValues();
    static bgfx::TextureFormat::Enum toBgfxFormat(PixelFormat format);
    static size_t dataSizeFor(PixelFormat format, uint32_t width, uint32_t height);

    BgfxRenderer& d_owner;
    bgfx::TextureHandle d_texture;
    Sizef d_size;
    Sizef d_dataSize;
    Vector2f d_texelScaling;
    const String d_name;
    PixelFormat d_pixelFormat;
    bool d_ownsTexture;
    bool d_renderTargetTexture;
    std::vector<uint8_t> d_cpuData;
};

} // namespace CEGUI

#endif
