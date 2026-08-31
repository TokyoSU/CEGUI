#include "CEGUI/RendererModules/Bgfx/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"
#include <bx/math.h>
#include <cmath>

namespace CEGUI
{
template <typename T>
BgfxRenderTarget<T>::BgfxRenderTarget(BgfxRenderer& owner, bgfx::ViewId viewId,
                                     bgfx::FrameBufferHandle framebuffer) :
    d_owner(owner), d_viewId(viewId), d_framebuffer(framebuffer),
    d_area(0, 0, 0, 0), d_clearPending(false)
{
}

template <typename T>
void BgfxRenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

template <typename T>
void BgfxRenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

template <typename T>
void BgfxRenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

template <typename T>
const Rectf& BgfxRenderTarget<T>::getArea() const
{
    return d_area;
}

template <typename T>
void BgfxRenderTarget<T>::activate()
{
    d_owner.activateView(d_viewId, d_framebuffer, d_area, d_clearPending);
    d_clearPending = false;
}

template <typename T>
void BgfxRenderTarget<T>::deactivate()
{
}

template <typename T>
void BgfxRenderTarget<T>::unprojectPoint(const GeometryBuffer& buffer,
                                         const Vector2f& pIn,
                                         Vector2f& pOut) const
{
    const BgfxGeometryBuffer& gb = static_cast<const BgfxGeometryBuffer&>(buffer);
    float inverse[16];
    bx::mtxInverse(inverse, gb.getMatrix());

    const bx::Vec3 rayA = bx::mul(bx::Vec3(pIn.d_x, pIn.d_y, -10000.0f), inverse);
    const bx::Vec3 rayB = bx::mul(bx::Vec3(pIn.d_x, pIn.d_y,  10000.0f), inverse);
    const float dz = rayB.z - rayA.z;
    if (std::fabs(dz) < 1.0e-6f)
    {
        pOut.d_x = rayA.x;
        pOut.d_y = rayA.y;
        return;
    }

    const float t = -rayA.z / dz;
    pOut.d_x = rayA.x + (rayB.x - rayA.x) * t;
    pOut.d_y = rayA.y + (rayB.y - rayA.y) * t;
}
}
