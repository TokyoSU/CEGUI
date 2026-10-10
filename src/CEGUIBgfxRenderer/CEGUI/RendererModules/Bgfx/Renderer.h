#ifndef _CEGUIBgfxRenderer_h_
#define _CEGUIBgfxRenderer_h_

#include "CEGUI/Renderer.h"
#include "CEGUI/Size.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Rect.h"

#include <bgfx/bgfx.h>

#include <map>
#include <string>
#include <vector>

#if defined(_WIN32) && !defined(CEGUI_STATIC)
#   ifdef CEGUIBGFXRENDERER_EXPORTS
#       define BGFX_GUIRENDERER_API __declspec(dllexport)
#   else
#       define BGFX_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define BGFX_GUIRENDERER_API
#endif

namespace CEGUI
{
class BgfxGeometryBuffer;
class BgfxTexture;

template <typename T> class BgfxRenderTarget;

class BGFX_GUIRENDERER_API BgfxRenderer : public Renderer
{
public:
    static BgfxRenderer& bootstrapSystem(
        const Sizef& displaySize,
        bgfx::ViewId viewIdBase = 240,
        uint16_t viewIdCount = 16,
        const char* shaderRoot = 0,
        const int abi = CEGUI_VERSION_ABI);
    static void destroySystem();

    static BgfxRenderer& create(
        const Sizef& displaySize,
        bgfx::ViewId viewIdBase = 240,
        uint16_t viewIdCount = 16,
        const char* shaderRoot = 0,
        const int abi = CEGUI_VERSION_ABI);
    static void destroy(BgfxRenderer& renderer);

    RenderTarget& getDefaultRenderTarget();
    GeometryBuffer& createGeometryBuffer();
    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();
    Texture& createTexture(const String& name);
    Texture& createTexture(const String& name, const String& filename,
                           const String& resourceGroup);
    Texture& createTexture(const String& name, const Sizef& size);
    void destroyTexture(Texture& texture);
    void destroyTexture(const String& name);
    void destroyAllTextures();
    Texture& getTexture(const String& name) const;
    bool isTextureDefined(const String& name) const;
    void beginRendering();
    void endRendering();
    void setDisplaySize(const Sizef& size);
    const Sizef& getDisplaySize() const;
    const Vector2f& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;

    bool isTexCoordSystemFlipped() const;

    const bgfx::VertexLayout& getVertexLayout() const { return d_vertexLayout; }
    bgfx::ProgramHandle getProgram() const { return d_program; }
    bgfx::UniformHandle getTextureSampler() const { return d_textureSampler; }
    bgfx::TextureHandle getWhiteTexture() const { return d_whiteTexture; }
    bgfx::ViewId getCurrentViewId() const { return d_currentViewId; }

    bgfx::ViewId acquireViewId();
    void releaseViewId(bgfx::ViewId id);
    void activateView(bgfx::ViewId id, bgfx::FrameBufferHandle framebuffer,
                      const Rectf& area, bool clear);

    void submitGeometry(bgfx::DynamicVertexBufferHandle vertexBuffer,
                        uint32_t startVertex, uint32_t vertexCount,
                        bgfx::TextureHandle texture, const float* modelMatrix,
                        BlendMode blendMode, bool clipped,
                        const Rectf& clipRect) const;

    // Five vec4 attributes. Matches vs_cegui_instanced.sc exactly.
    struct QuadInstance { float data[20]; };
    bool submitQuads(const QuadInstance* instances, uint32_t count,
                     bgfx::TextureHandle texture, const float* modelMatrix,
                     BlendMode blendMode, bool clipped, const Rectf& clipRect) const;
    bool supportsInstancing() const { return d_instancingEnabled && bgfx::isValid(d_instancedProgram); }
    void setInstancingEnabled(bool enabled) { d_instancingEnabled = enabled; }

protected:
    BgfxRenderer(const Sizef& displaySize, bgfx::ViewId viewIdBase,
                 uint16_t viewIdCount, const char* shaderRoot);
    ~BgfxRenderer();

private:
    BgfxRenderer(const BgfxRenderer&) = delete;
    BgfxRenderer& operator=(const BgfxRenderer&) = delete;

    bgfx::ShaderHandle loadShader(const char* name) const;
    std::string getBackendDirectory() const;
    void initialiseResources();
    void destroyResources();
    void throwIfNameExists(const String& name) const;
    static void logTextureCreation(const String& name);
    static void logTextureDestruction(const String& name);

    static String d_rendererID;

    Sizef d_displaySize;
    Vector2f d_displayDPI;
    RenderTarget* d_defaultTarget;

    typedef std::vector<TextureTarget*> TextureTargetList;
    TextureTargetList d_textureTargets;
    typedef std::vector<BgfxGeometryBuffer*> GeometryBufferList;
    GeometryBufferList d_geometryBuffers;
    typedef std::map<String, BgfxTexture*, StringFastLessCompare
                     CEGUI_MAP_ALLOC(String, BgfxTexture*)> TextureMap;
    TextureMap d_textures;

    bgfx::VertexLayout d_vertexLayout;
    void applyDrawState(bgfx::TextureHandle texture, const float* modelMatrix,
                        BlendMode blendMode, bool clipped, const Rectf& clipRect) const;
    bgfx::ProgramHandle d_program;
    bool d_instancingEnabled = true;
    bgfx::ProgramHandle d_instancedProgram;
    bgfx::VertexBufferHandle d_quadBuffer;
    bgfx::UniformHandle d_textureSampler;
    bgfx::TextureHandle d_whiteTexture;

    bgfx::ViewId d_viewIdBase;
    uint16_t d_viewIdCount;
    std::vector<bool> d_viewIdsUsed;
    bgfx::ViewId d_currentViewId;
    std::string d_shaderRoot;
};

} // namespace CEGUI

#endif
