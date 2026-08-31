#include "CEGUI/CEGUIC.h"

extern "C" void CEGUI_C_InternalSetError(const char* message);

#if defined(CEGUI_NATIVE_HAS_BGFX)

#include "CEGUI/System.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RendererModules/Bgfx/Renderer.h"

#include <exception>
#include <string>

namespace
{
std::string toUtf8CopyBgfx(const CEGUI::String& value)
{
#if defined(CEGUI_STRING_CLASS_UNICODE)
    return reinterpret_cast<const char*>(value.c_str());
#else
    return value.c_str();
#endif
}
}

extern "C"
{

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_BootstrapSystem(
    float width,
    float height,
    uint16_t view_id_base,
    uint16_t view_id_count,
    const char* shader_root_utf8,
    CEGUI_C_System* out_system)
{
    if (!out_system || width <= 0.0f || height <= 0.0f || view_id_count == 0)
    {
        CEGUI_C_InternalSetError(
            "bgfx display width/height and view count must be positive, and output system pointer must not be null.");
        return CEGUI_C_RESULT_INVALID_ARGUMENT;
    }

    try
    {
        CEGUI::BgfxRenderer::bootstrapSystem(
            CEGUI::Sizef(width, height),
            view_id_base,
            view_id_count,
            (shader_root_utf8 && *shader_root_utf8) ? shader_root_utf8 : 0);

        *out_system = reinterpret_cast<CEGUI_C_System>(CEGUI::System::getSingletonPtr());
        return CEGUI_C_RESULT_SUCCESS;
    }
    catch (const CEGUI::Exception& exception)
    {
        const std::string message = toUtf8CopyBgfx(exception.getMessage());
        CEGUI_C_InternalSetError(message.c_str());
        return CEGUI_C_RESULT_CEGUI_EXCEPTION;
    }
    catch (const std::exception& exception)
    {
        CEGUI_C_InternalSetError(exception.what());
        return CEGUI_C_RESULT_STD_EXCEPTION;
    }
    catch (...)
    {
        CEGUI_C_InternalSetError("Unknown native exception while bootstrapping the bgfx renderer.");
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION;
    }
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_DestroySystem(void)
{
    try
    {
        if (!CEGUI::System::getSingletonPtr())
        {
            CEGUI_C_InternalSetError("CEGUI::System has not been created.");
            return CEGUI_C_RESULT_NOT_INITIALISED;
        }

        CEGUI::BgfxRenderer::destroySystem();
        return CEGUI_C_RESULT_SUCCESS;
    }
    catch (const CEGUI::Exception& exception)
    {
        const std::string message = toUtf8CopyBgfx(exception.getMessage());
        CEGUI_C_InternalSetError(message.c_str());
        return CEGUI_C_RESULT_CEGUI_EXCEPTION;
    }
    catch (const std::exception& exception)
    {
        CEGUI_C_InternalSetError(exception.what());
        return CEGUI_C_RESULT_STD_EXCEPTION;
    }
    catch (...)
    {
        CEGUI_C_InternalSetError("Unknown native exception while destroying the bgfx renderer.");
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION;
    }
}

} // extern "C"

#else

extern "C"
{

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_BootstrapSystem(
    float, float, uint16_t, uint16_t, const char*, CEGUI_C_System*)
{
    CEGUI_C_InternalSetError("The CEGUI C binding was built without bgfx renderer support.");
    return CEGUI_C_RESULT_NOT_SUPPORTED;
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_DestroySystem(void)
{
    CEGUI_C_InternalSetError("The CEGUI C binding was built without bgfx renderer support.");
    return CEGUI_C_RESULT_NOT_SUPPORTED;
}

} // extern "C"

#endif
