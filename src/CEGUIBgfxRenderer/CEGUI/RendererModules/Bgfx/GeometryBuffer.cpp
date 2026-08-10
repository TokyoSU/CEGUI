#include "CEGUI/RendererModules/Bgfx/GeometryBuffer.h"
#include "CEGUI/RendererModules/Bgfx/Texture.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/Exceptions.h"

#include <bx/math.h>
#include <algorithm>
#include <cstring>

namespace CEGUI
{
BgfxGeometryBuffer::BgfxGeometryBuffer(BgfxRenderer& owner) :
    d_owner(owner), d_activeTexture(0), d_vertexBuffer(BGFX_INVALID_HANDLE),
    d_bufferSynced(false), d_clipRect(0, 0, 0, 0), d_clippingActive(true),
    d_translation(0, 0, 0), d_rotation(), d_pivot(0, 0, 0),
    d_effect(0), d_matrixValid(false)
{
    bx::mtxIdentity(d_matrix);
}

BgfxGeometryBuffer::~BgfxGeometryBuffer()
{
    if (bgfx::isValid(d_vertexBuffer))
        bgfx::destroy(d_vertexBuffer);
}

void BgfxGeometryBuffer::draw() const
{
    if (d_vertices.empty())
        return;
    if (!d_bufferSynced)
        syncHardwareBuffer();
    if (!d_matrixValid)
        updateMatrix();

    const int passCount = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < passCount; ++pass)
    {
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        uint32_t start = 0;
        for (std::vector<BatchInfo>::const_iterator it = d_batches.begin();
             it != d_batches.end(); ++it)
        {
            d_owner.submitGeometry(d_vertexBuffer, start, it->vertexCount,
                                   it->texture, d_matrix, d_blendMode,
                                   it->clip, d_clipRect);
            start += it->vertexCount;
        }
    }

    if (d_effect)
        d_effect->performPostRenderFunctions();
}

void BgfxGeometryBuffer::setTranslation(const Vector3f& v) { d_translation = v; d_matrixValid = false; }
void BgfxGeometryBuffer::setRotation(const Quaternion& r) { d_rotation = r; d_matrixValid = false; }
void BgfxGeometryBuffer::setPivot(const Vector3f& p) { d_pivot = p; d_matrixValid = false; }

void BgfxGeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(std::max(0.0f, region.top()));
    d_clipRect.bottom(std::max(0.0f, region.bottom()));
    d_clipRect.left(std::max(0.0f, region.left()));
    d_clipRect.right(std::max(0.0f, region.right()));
}

void BgfxGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

void BgfxGeometryBuffer::appendGeometry(const Vertex* const vertices, uint vertexCount)
{
    if (!vertices || vertexCount == 0)
        return;

    bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
    if (d_activeTexture)
        texture = d_activeTexture->getHandle();
    if (d_batches.empty() || d_batches.back().texture.idx != texture.idx ||
        d_batches.back().clip != d_clippingActive)
    {
        BatchInfo batch = { texture, 0u, d_clippingActive };
        d_batches.push_back(batch);
    }
    d_batches.back().vertexCount += vertexCount;

    d_vertices.reserve(d_vertices.size() + vertexCount);
    for (uint i = 0; i < vertexCount; ++i)
    {
        BgfxVertex out;
        out.x = vertices[i].position.d_x;
        out.y = vertices[i].position.d_y;
        out.z = vertices[i].position.d_z;
        out.abgr = colourToABGR(vertices[i].colour_val);
        out.u = vertices[i].tex_coords.d_x;
        out.v = vertices[i].tex_coords.d_y;
        d_vertices.push_back(out);
    }
    d_bufferSynced = false;
}

void BgfxGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<BgfxTexture*>(texture);
}

void BgfxGeometryBuffer::reset()
{
    d_batches.clear();
    d_vertices.clear();
    d_activeTexture = 0;
    d_bufferSynced = false;
}

Texture* BgfxGeometryBuffer::getActiveTexture() const { return d_activeTexture; }
uint BgfxGeometryBuffer::getVertexCount() const { return static_cast<uint>(d_vertices.size()); }
uint BgfxGeometryBuffer::getBatchCount() const { return static_cast<uint>(d_batches.size()); }
void BgfxGeometryBuffer::setRenderEffect(RenderEffect* effect) { d_effect = effect; }
RenderEffect* BgfxGeometryBuffer::getRenderEffect() { return d_effect; }
void BgfxGeometryBuffer::setClippingActive(bool active) { d_clippingActive = active; }
bool BgfxGeometryBuffer::isClippingActive() const { return d_clippingActive; }

const float* BgfxGeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();
    return d_matrix;
}

void BgfxGeometryBuffer::updateMatrix() const
{
    const bx::Quaternion rotation(d_rotation.d_x, d_rotation.d_y,
                                  d_rotation.d_z, d_rotation.d_w);
    float toOrigin[16];
    float rotate[16];
    float fromOrigin[16];
    float tmp[16];
    bx::mtxTranslate(toOrigin, -d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);
    bx::mtxFromQuaternion(rotate, rotation);
    bx::mtxTranslate(fromOrigin,
        d_pivot.d_x + d_translation.d_x,
        d_pivot.d_y + d_translation.d_y,
        d_pivot.d_z + d_translation.d_z);
    bx::mtxMul(tmp, toOrigin, rotate);
    bx::mtxMul(d_matrix, tmp, fromOrigin);
    d_matrixValid = true;
}

void BgfxGeometryBuffer::syncHardwareBuffer() const
{
    if (d_vertices.empty())
    {
        d_bufferSynced = true;
        return;
    }

    if (!bgfx::isValid(d_vertexBuffer))
    {
        d_vertexBuffer = bgfx::createDynamicVertexBuffer(
            static_cast<uint32_t>(d_vertices.size()), d_owner.getVertexLayout(),
            BGFX_BUFFER_ALLOW_RESIZE);
        if (!bgfx::isValid(d_vertexBuffer))
            CEGUI_THROW(RendererException("bgfx failed to create CEGUI dynamic vertex buffer."));
    }

    const uint32_t bytes = static_cast<uint32_t>(d_vertices.size() * sizeof(BgfxVertex));
    bgfx::update(d_vertexBuffer, 0, bgfx::copy(d_vertices.data(), bytes));
    d_bufferSynced = true;
}

uint32_t BgfxGeometryBuffer::colourToABGR(const Colour& colour)
{
    const uint32_t r = static_cast<uint32_t>(std::max(0.0f, std::min(1.0f, colour.getRed())) * 255.0f + 0.5f);
    const uint32_t g = static_cast<uint32_t>(std::max(0.0f, std::min(1.0f, colour.getGreen())) * 255.0f + 0.5f);
    const uint32_t b = static_cast<uint32_t>(std::max(0.0f, std::min(1.0f, colour.getBlue())) * 255.0f + 0.5f);
    const uint32_t a = static_cast<uint32_t>(std::max(0.0f, std::min(1.0f, colour.getAlpha())) * 255.0f + 0.5f);
    return (a << 24) | (b << 16) | (g << 8) | r;
}
}
