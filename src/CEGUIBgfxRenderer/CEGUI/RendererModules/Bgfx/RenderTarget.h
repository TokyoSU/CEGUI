#ifndef _CEGUIBgfxRenderTarget_h_
#define _CEGUIBgfxRenderTarget_h_

#include "CEGUI/RenderTarget.h"
#include "CEGUI/RendererModules/Bgfx/Renderer.h"
#include "CEGUI/Rect.h"

#include <bgfx/bgfx.h>

namespace CEGUI
{
template <typename T = RenderTarget>
class BGFX_GUIRENDERER_API BgfxRenderTarget : public T
{
public:
    BgfxRenderTarget(BgfxRenderer& owner, bgfx::ViewId viewId,
                     bgfx::FrameBufferHandle framebuffer = BGFX_INVALID_HANDLE);
    virtual ~BgfxRenderTarget() {}

    void draw(const GeometryBuffer& buffer);
    void draw(const RenderQueue& queue);
    void setArea(const Rectf& area);
    const Rectf& getArea() const;
    void activate();
    void deactivate();
    void unprojectPoint(const GeometryBuffer& buffer,
                        const Vector2f& pIn, Vector2f& pOut) const;
    bool isImageryCache() const { return false; }

protected:
    BgfxRenderer& d_owner;
    bgfx::ViewId d_viewId;
    bgfx::FrameBufferHandle d_framebuffer;
    Rectf d_area;
    bool d_clearPending;
};
}

#include "CEGUI/RendererModules/Bgfx/RenderTarget.inl"
#endif
