#ifndef _CEGUIBgfxTextureTarget_h_
#define _CEGUIBgfxTextureTarget_h_

#include "CEGUI/TextureTarget.h"
#include "CEGUI/RendererModules/Bgfx/RenderTarget.h"

namespace CEGUI
{
class BgfxTexture;

class BGFX_GUIRENDERER_API BgfxTextureTarget : public BgfxRenderTarget<TextureTarget>
{
public:
    explicit BgfxTextureTarget(BgfxRenderer& owner);
    ~BgfxTextureTarget();

    bool isImageryCache() const;
    void clear();
    Texture& getTexture() const;
    void declareRenderSize(const Sizef& size);
    bool isRenderingInverted() const;

private:
    static String generateTextureName();
    void cleanupRenderTexture();
    void initialiseRenderTexture();

    static uint s_textureNumber;
    static const float DEFAULT_SIZE;
    BgfxTexture* d_ceguiTexture;
    bgfx::TextureHandle d_renderTexture;
};
}
#endif
