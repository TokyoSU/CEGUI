#include "CEGUI/CEGUIC.h"

extern "C" void CEGUI_C_InternalSetError(const char* message);

#if defined(_WIN32) && defined(CEGUI_NATIVE_HAS_D3D11)

#include "CEGUI/System.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"

#include <d3d11.h>
#include <exception>
#include <string>

namespace
{
std::string toUtf8CopyD3D(const CEGUI::String& value)
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

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_BootstrapSystem(
    void* device,
    void* device_context,
    CEGUI_C_System* out_system)
{
    if (!device || !device_context || !out_system)
    {
        CEGUI_C_InternalSetError("D3D11 device, context, and output system pointer must not be null.");
        return CEGUI_C_RESULT_INVALID_ARGUMENT;
    }

    try
    {
        CEGUI::Direct3D11Renderer::bootstrapSystem(
            static_cast<ID3D11Device*>(device),
            static_cast<ID3D11DeviceContext*>(device_context));

        *out_system = reinterpret_cast<CEGUI_C_System>(CEGUI::System::getSingletonPtr());
        return CEGUI_C_RESULT_SUCCESS;
    }
    catch (const CEGUI::Exception& exception)
    {
        const std::string message = toUtf8CopyD3D(exception.getMessage());
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
        CEGUI_C_InternalSetError("Unknown native exception while bootstrapping the D3D11 renderer.");
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION;
    }
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_DestroySystem(void)
{
    try
    {
        if (!CEGUI::System::getSingletonPtr())
        {
            CEGUI_C_InternalSetError("CEGUI::System has not been created.");
            return CEGUI_C_RESULT_NOT_INITIALISED;
        }

        CEGUI::Direct3D11Renderer::destroySystem();
        return CEGUI_C_RESULT_SUCCESS;
    }
    catch (const CEGUI::Exception& exception)
    {
        const std::string message = toUtf8CopyD3D(exception.getMessage());
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
        CEGUI_C_InternalSetError("Unknown native exception while destroying the D3D11 renderer.");
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION;
    }
}

} // extern "C"

#else

extern "C"
{

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_BootstrapSystem(
    void*, void*, CEGUI_C_System*)
{
    CEGUI_C_InternalSetError("The CEGUI C binding was built without Direct3D 11 renderer support.");
    return CEGUI_C_RESULT_NOT_SUPPORTED;
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_DestroySystem(void)
{
    CEGUI_C_InternalSetError("The CEGUI C binding was built without Direct3D 11 renderer support.");
    return CEGUI_C_RESULT_NOT_SUPPORTED;
}

} // extern "C"

#endif
