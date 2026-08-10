#ifndef _CEGUIBgfxGeometryBuffer_h_
#define _CEGUIBgfxGeometryBuffer_h_

#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RendererModules/Bgfx/Renderer.h"
#include "CEGUI/Quaternion.h"
#include "CEGUI/Rect.h"

#include <bgfx/bgfx.h>
#include <vector>

namespace CEGUI
{
class BgfxTexture;

class BGFX_GUIRENDERER_API BgfxGeometryBuffer : public GeometryBuffer
{
public:
    explicit BgfxGeometryBuffer(BgfxRenderer& owner);
    ~BgfxGeometryBuffer();

    void draw() const;
    void setTranslation(const Vector3f& v);
    void setRotation(const Quaternion& r);
    void setPivot(const Vector3f& p);
    void setClippingRegion(const Rectf& region);
    void appendVertex(const Vertex& vertex);
    void appendGeometry(const Vertex* const vbuff, uint vertex_count);
    void setActiveTexture(Texture* texture);
    void reset();
    Texture* getActiveTexture() const;
    uint getVertexCount() const;
    uint getBatchCount() const;
    void setRenderEffect(RenderEffect* effect);
    RenderEffect* getRenderEffect();
    void setClippingActive(bool active);
    bool isClippingActive() const;

    const float* getMatrix() const;

private:
    struct BgfxVertex
    {
        float x, y, z;
        uint32_t abgr;
        float u, v;
    };

    struct BatchInfo
    {
        bgfx::TextureHandle texture;
        uint32_t vertexCount;
        bool clip;
    };

    void updateMatrix() const;
    void syncHardwareBuffer() const;
    static uint32_t colourToABGR(const Colour& colour);

    BgfxRenderer& d_owner;
    BgfxTexture* d_activeTexture;
    mutable bgfx::DynamicVertexBufferHandle d_vertexBuffer;
    mutable bool d_bufferSynced;
    std::vector<BatchInfo> d_batches;
    std::vector<BgfxVertex> d_vertices;
    Rectf d_clipRect;
    bool d_clippingActive;
    Vector3f d_translation;
    Quaternion d_rotation;
    Vector3f d_pivot;
    RenderEffect* d_effect;
    mutable float d_matrix[16];
    mutable bool d_matrixValid;
};

} // namespace CEGUI

#endif
