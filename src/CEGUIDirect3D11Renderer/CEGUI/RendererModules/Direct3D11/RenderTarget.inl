/***********************************************************************
    created:    Wed May 5 2010
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/RendererModules/Direct3D11/RenderTarget.h"
#include "CEGUI/RendererModules/Direct3D11/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template <typename T>
Direct3D11RenderTarget<T>::Direct3D11RenderTarget(Direct3D11Renderer& owner) :
    d_owner(owner),
    d_device(d_owner.getDirect3DDevice()),
    d_area(0, 0, 0, 0),
    d_viewDistance(0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
template <typename T>
const Rectf& Direct3D11RenderTarget<T>::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3D11_VIEWPORT vp;
    setupViewport(vp);
    d_device.d_context->RSSetViewports(1, &vp);

    d_owner.setProjectionMatrix(d_matrix);
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::deactivate()
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::unprojectPoint(const GeometryBuffer& buff,
                                            const Vector2f& p_in,
                                            Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const Direct3D11GeometryBuffer& gb =
        static_cast<const Direct3D11GeometryBuffer&>(buff);

    D3D11_VIEWPORT vp;
    setupViewport(vp);

    using DirectX::SimpleMath::Plane;
    using DirectX::SimpleMath::Vector3;

    const DirectX::XMMATRIX projection = d_matrix;
    const DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
    const DirectX::XMMATRIX world = *gb.getMatrix();

    const auto project = [&](const Vector3& value)
    {
        return Vector3(DirectX::XMVector3Project(
            value, vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height,
            vp.MinDepth, vp.MaxDepth, projection, view, world));
    };

    const auto unproject = [&](const Vector3& value)
    {
        return Vector3(DirectX::XMVector3Unproject(
            value, vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height,
            vp.MinDepth, vp.MaxDepth, projection, view, world));
    };

    // Project three local points to obtain the plane occupied by this
    // GeometryBuffer, then intersect the mouse ray with that plane.
    const Vector3 p1 = project(Vector3(0.0f, 0.0f, 0.0f));
    const Vector3 p2 = project(Vector3(1.0f, 0.0f, 0.0f));
    const Vector3 p3 = project(Vector3(0.0f, 1.0f, 0.0f));
    const Plane surfacePlane(p1, p2, p3);

    const Vector3 rayStart = unproject(Vector3(
        vp.Width * 0.5f, vp.Height * 0.5f, -d_viewDistance));
    const Vector3 rayEnd = unproject(Vector3(p_in.d_x, p_in.d_y, 0.0f));
    const Vector3 intersection(DirectX::XMPlaneIntersectLine(
        surfacePlane, rayStart, rayEnd));

    p_out.d_x = intersection.x;
    p_out.d_y = intersection.y;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::updateMatrix() const
{
    using DirectX::SimpleMath::Matrix;
    using DirectX::SimpleMath::Vector3;

    const float fov = 0.523598776f;
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

    const Vector3 eye(midx, midy, -d_viewDistance);
    const Vector3 at(midx, midy, 1.0f);
    const Vector3 up(0.0f, -1.0f, 0.0f);

    // Keep the existing right-handed camera/projection behaviour while
    // storing the result in DirectXTK's SimpleMath matrix type.
    const Matrix view(DirectX::XMMatrixLookAtRH(eye, at, up));
    const Matrix projection(DirectX::XMMatrixPerspectiveFovRH(
        fov, aspect, d_viewDistance * 0.5f, d_viewDistance * 2.0f));
    d_matrix = view * projection;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::setupViewport(D3D11_VIEWPORT& vp) const
{
    vp.TopLeftX = static_cast<FLOAT>(d_area.left());
    vp.TopLeftY = static_cast<FLOAT>(d_area.top());
    vp.Width = static_cast<FLOAT>(d_area.getWidth());
    vp.Height = static_cast<FLOAT>(d_area.getHeight());
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
