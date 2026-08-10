#include "CEGUI/RendererModules/Bgfx/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/ImageCodec.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace CEGUI
{
namespace
{
uint16_t checkedDimension(float value)
{
    if (value <= 0.0f || value > 65535.0f)
        CEGUI_THROW(InvalidRequestException("bgfx texture dimensions must be in the range 1..65535."));
    return static_cast<uint16_t>(value);
}
}

BgfxTexture::BgfxTexture(BgfxRenderer& owner, const String& name) :
    d_owner(owner), d_texture(BGFX_INVALID_HANDLE), d_size(0, 0),
    d_dataSize(0, 0), d_texelScaling(0, 0), d_name(name),
    d_pixelFormat(PF_RGBA), d_ownsTexture(true), d_renderTargetTexture(false)
{
}

BgfxTexture::BgfxTexture(BgfxRenderer& owner, const String& name,
                         const String& filename, const String& resourceGroup) :
    BgfxTexture(owner, name)
{
    loadFromFile(filename, resourceGroup);
}

BgfxTexture::BgfxTexture(BgfxRenderer& owner, const String& name,
                         const Sizef& size) :
    BgfxTexture(owner, name)
{
    const uint16_t width = checkedDimension(size.d_width);
    const uint16_t height = checkedDimension(size.d_height);
    createEmptyTexture(size, PF_RGBA);
    d_dataSize = size;
    d_cpuData.resize(static_cast<size_t>(width) * height * 4u, 0);
    updateCachedScaleValues();
}

BgfxTexture::~BgfxTexture()
{
    destroyTexture();
}

const String& BgfxTexture::getName() const { return d_name; }
const Sizef& BgfxTexture::getSize() const { return d_size; }
const Sizef& BgfxTexture::getOriginalDataSize() const { return d_dataSize; }
const Vector2f& BgfxTexture::getTexelScaling() const { return d_texelScaling; }

void BgfxTexture::loadFromFile(const String& filename, const String& resourceGroup)
{
    System* system = System::getSingletonPtr();
    if (!system)
        CEGUI_THROW(RendererException("CEGUI::System object has not been created!"));

    RawDataContainer data;
    system->getResourceProvider()->loadRawDataContainer(filename, data, resourceGroup);
    Texture* result = 0;
    try
    {
        result = system->getImageCodec().load(data, this);
    }
    catch (...)
    {
        system->getResourceProvider()->unloadRawDataContainer(data);
        throw;
    }
    system->getResourceProvider()->unloadRawDataContainer(data);

    if (!result)
        CEGUI_THROW(RendererException(system->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

void BgfxTexture::loadFromMemory(const void* buffer, const Sizef& bufferSize,
                                 PixelFormat pixelFormat)
{
    if (!buffer)
        CEGUI_THROW(InvalidRequestException("Null image buffer supplied to bgfx texture."));
    if (!isPixelFormatSupported(pixelFormat))
        CEGUI_THROW(InvalidRequestException("Data was supplied in an unsupported pixel format."));

    const uint16_t width = checkedDimension(bufferSize.d_width);
    const uint16_t height = checkedDimension(bufferSize.d_height);

    destroyTexture();
    d_pixelFormat = pixelFormat;
    d_ownsTexture = true;
    d_renderTargetTexture = false;

    std::vector<uint8_t> converted;
    const void* uploadData = buffer;
    size_t uploadSize = dataSizeFor(pixelFormat, width, height);
    PixelFormat uploadFormat = pixelFormat;

    if (pixelFormat == PF_RGB)
    {
        converted.resize(static_cast<size_t>(width) * height * 4u);
        const uint8_t* src = static_cast<const uint8_t*>(buffer);
        for (size_t i = 0, count = static_cast<size_t>(width) * height; i < count; ++i)
        {
            converted[i * 4 + 0] = src[i * 3 + 0];
            converted[i * 4 + 1] = src[i * 3 + 1];
            converted[i * 4 + 2] = src[i * 3 + 2];
            converted[i * 4 + 3] = 0xff;
        }
        uploadData = converted.data();
        uploadSize = converted.size();
        uploadFormat = PF_RGBA;
    }

    createEmptyTexture(bufferSize, uploadFormat);
    bgfx::updateTexture2D(d_texture, 0, 0, 0, 0, width, height,
                          bgfx::copy(uploadData, static_cast<uint32_t>(uploadSize)));

    d_pixelFormat = uploadFormat;
    d_dataSize = bufferSize;
    d_cpuData.assign(static_cast<const uint8_t*>(uploadData),
                     static_cast<const uint8_t*>(uploadData) + uploadSize);
    updateCachedScaleValues();
}

void BgfxTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    if (!sourceData || !bgfx::isValid(d_texture))
        return;
    if (d_renderTargetTexture)
        CEGUI_THROW(InvalidRequestException("Cannot CPU-blit into a CEGUI bgfx render-target texture."));
    if (d_pixelFormat != PF_RGBA)
        CEGUI_THROW(InvalidRequestException("Partial bgfx texture blits are supported for RGBA8 textures only."));

    const int left = static_cast<int>(area.left());
    const int top = static_cast<int>(area.top());
    const int width = static_cast<int>(area.getWidth());
    const int height = static_cast<int>(area.getHeight());
    if (left < 0 || top < 0 || width <= 0 || height <= 0 ||
        left + width > static_cast<int>(d_size.d_width) ||
        top + height > static_cast<int>(d_size.d_height))
        CEGUI_THROW(InvalidRequestException("bgfx texture blit rectangle lies outside the texture."));

    const size_t byteCount = static_cast<size_t>(width) * height * 4u;
    bgfx::updateTexture2D(d_texture, 0, 0, static_cast<uint16_t>(left),
                          static_cast<uint16_t>(top), static_cast<uint16_t>(width),
                          static_cast<uint16_t>(height),
                          bgfx::copy(sourceData, static_cast<uint32_t>(byteCount)),
                          static_cast<uint16_t>(width * 4));

    if (d_cpuData.size() == static_cast<size_t>(d_size.d_width * d_size.d_height) * 4u)
    {
        const uint8_t* src = static_cast<const uint8_t*>(sourceData);
        const size_t stride = static_cast<size_t>(d_size.d_width) * 4u;
        for (int y = 0; y < height; ++y)
            std::memcpy(&d_cpuData[(static_cast<size_t>(top + y) * stride) + static_cast<size_t>(left) * 4u],
                        src + static_cast<size_t>(y) * width * 4u,
                        static_cast<size_t>(width) * 4u);
    }
}

void BgfxTexture::blitToMemory(void* targetData)
{
    if (!targetData)
        CEGUI_THROW(InvalidRequestException("Null destination supplied to bgfx texture readback."));
    if (d_renderTargetTexture)
        CEGUI_THROW(RendererException(
            "Synchronous readback of CEGUI bgfx render targets is unavailable because bgfx readTexture is asynchronous and the host owns bgfx::frame()."));
    if (d_cpuData.empty())
        CEGUI_THROW(RendererException("No CPU-side copy is available for this bgfx texture."));

    std::memcpy(targetData, d_cpuData.data(), d_cpuData.size());
}

bool BgfxTexture::isPixelFormatSupported(PixelFormat fmt) const
{
    switch (fmt)
    {
    case PF_RGB:
    case PF_RGBA:
    case PF_RGB_DXT1:
    case PF_RGBA_DXT1:
    case PF_RGBA_DXT3:
    case PF_RGBA_DXT5:
        return true;
    default:
        return false;
    }
}

void BgfxTexture::setRenderTargetTexture(bgfx::TextureHandle handle, const Sizef& size)
{
    destroyTexture();
    d_texture = handle;
    d_size = size;
    d_dataSize = size;
    d_pixelFormat = PF_RGBA;
    d_ownsTexture = false;
    d_renderTargetTexture = true;
    d_cpuData.clear();
    updateCachedScaleValues();
}

void BgfxTexture::clearRenderTargetTexture()
{
    if (d_renderTargetTexture)
    {
        d_texture = BGFX_INVALID_HANDLE;
        d_size = Sizef(0, 0);
        d_dataSize = Sizef(0, 0);
        d_texelScaling = Vector2f(0, 0);
        d_ownsTexture = true;
        d_renderTargetTexture = false;
    }
}

void BgfxTexture::setOriginalDataSize(const Sizef& size)
{
    d_dataSize = size;
    updateCachedScaleValues();
}

void BgfxTexture::destroyTexture()
{
    if (d_ownsTexture && bgfx::isValid(d_texture))
        bgfx::destroy(d_texture);
    d_texture = BGFX_INVALID_HANDLE;
    d_size = Sizef(0, 0);
    d_cpuData.clear();
}

void BgfxTexture::createEmptyTexture(const Sizef& size, PixelFormat format,
                                     uint64_t extraFlags)
{
    const uint16_t width = checkedDimension(size.d_width);
    const uint16_t height = checkedDimension(size.d_height);
    const bgfx::TextureFormat::Enum bgfxFormat = toBgfxFormat(format);
    if (bgfxFormat == bgfx::TextureFormat::Unknown)
        CEGUI_THROW(InvalidRequestException("Unsupported bgfx texture format."));

    const uint64_t flags = extraFlags |
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP;
    d_texture = bgfx::createTexture2D(width, height, false, 1, bgfxFormat, flags);
    if (!bgfx::isValid(d_texture))
        CEGUI_THROW(RendererException("bgfx failed to create CEGUI texture."));

    d_size = size;
    d_pixelFormat = format;
    d_ownsTexture = true;
    d_renderTargetTexture = false;
}

void BgfxTexture::updateCachedScaleValues()
{
    const float width = (d_dataSize.d_width == d_size.d_width) ? d_dataSize.d_width : d_size.d_width;
    const float height = (d_dataSize.d_height == d_size.d_height) ? d_dataSize.d_height : d_size.d_height;
    d_texelScaling.d_x = width > 0.0f ? 1.0f / width : 0.0f;
    d_texelScaling.d_y = height > 0.0f ? 1.0f / height : 0.0f;
}

bgfx::TextureFormat::Enum BgfxTexture::toBgfxFormat(PixelFormat format)
{
    switch (format)
    {
    case PF_RGB:
    case PF_RGBA: return bgfx::TextureFormat::RGBA8;
    case PF_RGB_DXT1:
    case PF_RGBA_DXT1: return bgfx::TextureFormat::BC1;
    case PF_RGBA_DXT3: return bgfx::TextureFormat::BC2;
    case PF_RGBA_DXT5: return bgfx::TextureFormat::BC3;
    default: return bgfx::TextureFormat::Unknown;
    }
}

size_t BgfxTexture::dataSizeFor(PixelFormat format, uint32_t width, uint32_t height)
{
    switch (format)
    {
    case PF_RGB: return static_cast<size_t>(width) * height * 3u;
    case PF_RGBA: return static_cast<size_t>(width) * height * 4u;
    case PF_RGB_DXT1:
    case PF_RGBA_DXT1: return static_cast<size_t>((width + 3) / 4) * ((height + 3) / 4) * 8u;
    case PF_RGBA_DXT3:
    case PF_RGBA_DXT5: return static_cast<size_t>((width + 3) / 4) * ((height + 3) / 4) * 16u;
    default: return 0;
    }
}
}
