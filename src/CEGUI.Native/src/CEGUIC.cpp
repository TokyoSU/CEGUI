#include "CEGUI/CEGUIC.h"

#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Event.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Font.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/System.h"
#include "CEGUI/Window.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/Falagard/WidgetLookManager.h"
#include "CEGUI/Widgets/Editbox.h"
#include "CEGUI/Widgets/ProgressBar.h"
#include "CEGUI/Widgets/Scrollbar.h"
#include "CEGUI/Widgets/Slider.h"
#include "CEGUI/Widgets/ToggleButton.h"

#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace
{
thread_local std::string g_lastError;
thread_local std::string g_returnString;

void clearError()
{
    g_lastError.clear();
}

CEGUI_C_Result fail(CEGUI_C_Result result, const char* message)
{
    g_lastError = message ? message : "Unknown CEGUI C binding error.";
    return result;
}

std::string toUtf8Copy(const CEGUI::String& value)
{
#if defined(CEGUI_STRING_CLASS_UNICODE)
    return reinterpret_cast<const char*>(value.c_str());
#else
    return value.c_str();
#endif
}

CEGUI::String fromUtf8(const char* value)
{
    return CEGUI::String(value ? value : "");
}

const char* storeReturnString(const CEGUI::String& value)
{
    g_returnString = toUtf8Copy(value);
    return g_returnString.c_str();
}

CEGUI::System* asSystem(CEGUI_C_System value)
{
    return reinterpret_cast<CEGUI::System*>(value);
}

CEGUI::GUIContext* asContext(CEGUI_C_GUIContext value)
{
    return reinterpret_cast<CEGUI::GUIContext*>(value);
}

CEGUI::Window* asWindow(CEGUI_C_Window value)
{
    return reinterpret_cast<CEGUI::Window*>(value);
}

CEGUI_C_System toSystem(CEGUI::System* value)
{
    return reinterpret_cast<CEGUI_C_System>(value);
}

CEGUI_C_GUIContext toContext(CEGUI::GUIContext* value)
{
    return reinterpret_cast<CEGUI_C_GUIContext>(value);
}

CEGUI_C_Window toWindow(CEGUI::Window* value)
{
    return reinterpret_cast<CEGUI_C_Window>(value);
}

CEGUI_C_Result requirePointer(const void* value, const char* name)
{
    if (value)
        return CEGUI_C_RESULT_SUCCESS;

    g_lastError = std::string(name ? name : "Argument") + " must not be null.";
    return CEGUI_C_RESULT_INVALID_ARGUMENT;
}

CEGUI_C_Result requireSystem(CEGUI_C_System value, CEGUI::System*& out)
{
    if (!value)
        return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "System handle must not be null.");

    out = asSystem(value);
    return CEGUI_C_RESULT_SUCCESS;
}

CEGUI_C_Result requireContext(CEGUI_C_GUIContext value, CEGUI::GUIContext*& out)
{
    if (!value)
        return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "GUIContext handle must not be null.");

    out = asContext(value);
    return CEGUI_C_RESULT_SUCCESS;
}

CEGUI_C_Result requireWindow(CEGUI_C_Window value, CEGUI::Window*& out)
{
    if (!value)
        return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "Window handle must not be null.");

    out = asWindow(value);
    return CEGUI_C_RESULT_SUCCESS;
}

CEGUI::MouseButton toMouseButton(CEGUI_C_MouseButton value)
{
    switch (value)
    {
    case CEGUI_C_MOUSE_LEFT: return CEGUI::LeftButton;
    case CEGUI_C_MOUSE_RIGHT: return CEGUI::RightButton;
    case CEGUI_C_MOUSE_MIDDLE: return CEGUI::MiddleButton;
    case CEGUI_C_MOUSE_X1: return CEGUI::X1Button;
    case CEGUI_C_MOUSE_X2: return CEGUI::X2Button;
    default: return CEGUI::NoButton;
    }
}

CEGUI::UDim toUDim(const CEGUI_C_UDim& value)
{
    return CEGUI::UDim(value.scale, value.offset);
}

CEGUI::UVector2 toUVector2(const CEGUI_C_UVector2& value)
{
    return CEGUI::UVector2(toUDim(value.x), toUDim(value.y));
}

CEGUI::USize toUSize(const CEGUI_C_USize& value)
{
    return CEGUI::USize(toUDim(value.width), toUDim(value.height));
}

CEGUI_C_UDim fromUDim(const CEGUI::UDim& value)
{
    CEGUI_C_UDim result{};
    result.scale = value.d_scale;
    result.offset = value.d_offset;
    return result;
}

CEGUI_C_UVector2 fromUVector2(const CEGUI::UVector2& value)
{
    CEGUI_C_UVector2 result{};
    result.x = fromUDim(value.d_x);
    result.y = fromUDim(value.d_y);
    return result;
}

CEGUI_C_USize fromUSize(const CEGUI::USize& value)
{
    CEGUI_C_USize result{};
    result.width = fromUDim(value.d_width);
    result.height = fromUDim(value.d_height);
    return result;
}

CEGUI::Window* findWindowByName(const CEGUI::String& name)
{
    CEGUI::WindowManager::WindowIterator iterator = CEGUI::WindowManager::getSingleton().getIterator();
    while (!iterator.isAtEnd())
    {
        CEGUI::Window* window = iterator.getCurrentValue();
        if (window && window->getName() == name)
            return window;
        ++iterator;
    }
    return nullptr;
}

CEGUI_C_EventData makeEventData(const CEGUI::EventArgs& args)
{
    CEGUI_C_EventData data{};
    data.kind = CEGUI_C_EVENT_GENERIC;
    data.handled_count = args.handled;

    if (const auto* windowArgs = dynamic_cast<const CEGUI::WindowEventArgs*>(&args))
    {
        data.kind = CEGUI_C_EVENT_WINDOW;
        data.window = toWindow(windowArgs->window);
    }

    if (const auto* mouseArgs = dynamic_cast<const CEGUI::MouseEventArgs*>(&args))
    {
        data.kind = CEGUI_C_EVENT_MOUSE;
        data.window = toWindow(mouseArgs->window);
        data.position_x = mouseArgs->position.d_x;
        data.position_y = mouseArgs->position.d_y;
        data.move_delta_x = mouseArgs->moveDelta.d_x;
        data.move_delta_y = mouseArgs->moveDelta.d_y;
        data.wheel_change = mouseArgs->wheelChange;
        data.mouse_button = static_cast<uint32_t>(mouseArgs->button);
        data.click_count = mouseArgs->clickCount;
        data.system_keys = mouseArgs->sysKeys;
    }
    else if (const auto* keyArgs = dynamic_cast<const CEGUI::KeyEventArgs*>(&args))
    {
        data.kind = CEGUI_C_EVENT_KEY;
        data.window = toWindow(keyArgs->window);
        data.codepoint = static_cast<uint32_t>(keyArgs->codepoint);
        data.scancode = static_cast<uint32_t>(keyArgs->scancode);
        data.system_keys = keyArgs->sysKeys;
    }
    else if (const auto* activationArgs = dynamic_cast<const CEGUI::ActivationEventArgs*>(&args))
    {
        data.kind = CEGUI_C_EVENT_ACTIVATION;
        data.window = toWindow(activationArgs->window);
        data.other_window = toWindow(activationArgs->otherWindow);
    }
    else if (const auto* updateArgs = dynamic_cast<const CEGUI::UpdateEventArgs*>(&args))
    {
        data.kind = CEGUI_C_EVENT_UPDATE;
        data.window = toWindow(updateArgs->window);
        data.elapsed = updateArgs->d_timeSinceLastFrame;
    }

    return data;
}

struct EventConnectionHolder
{
    CEGUI::Event::Connection connection;
};


CEGUI_C_EventConnection toConnection(EventConnectionHolder* value)
{
    return reinterpret_cast<CEGUI_C_EventConnection>(value);
}

EventConnectionHolder* asConnection(CEGUI_C_EventConnection value)
{
    return reinterpret_cast<EventConnectionHolder*>(value);
}

template<typename T>
T* checkedWidget(CEGUI_C_Window handle, const char* expectedType)
{
    CEGUI::Window* window = asWindow(handle);
    T* widget = dynamic_cast<T*>(window);
    if (!widget)
        throw std::invalid_argument(std::string("Window is not a ") + expectedType + ".");
    return widget;
}

#define CEGUI_C_BEGIN() \
    clearError(); \
    try

#define CEGUI_C_END() \
    catch (const CEGUI::Exception& exception) \
    { \
        g_lastError = toUtf8Copy(exception.getMessage()); \
        return CEGUI_C_RESULT_CEGUI_EXCEPTION; \
    } \
    catch (const std::exception& exception) \
    { \
        g_lastError = exception.what(); \
        return CEGUI_C_RESULT_STD_EXCEPTION; \
    } \
    catch (...) \
    { \
        g_lastError = "Unknown native exception."; \
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION; \
    }

#define CEGUI_C_REQUIRE(value, name) \
    do { const CEGUI_C_Result _r = requirePointer((value), (name)); if (_r != CEGUI_C_RESULT_SUCCESS) return _r; } while (0)

} // namespace

extern "C"
{

void CEGUI_C_InternalSetError(const char* message)
{
    g_lastError = message ? message : "Unknown CEGUI C binding error.";
}

const char* CEGUI_C_CALL CEGUI_C_GetLastError(void)
{
    return g_lastError.c_str();
}

void CEGUI_C_CALL CEGUI_C_ClearLastError(void)
{
    clearError();
}

uint32_t CEGUI_C_CALL CEGUI_C_GetVersionMajor(void) { return CEGUI_VERSION_MAJOR; }
uint32_t CEGUI_C_CALL CEGUI_C_GetVersionMinor(void) { return CEGUI_VERSION_MINOR; }
uint32_t CEGUI_C_CALL CEGUI_C_GetVersionPatch(void) { return CEGUI_VERSION_PATCH; }

const char* CEGUI_C_CALL CEGUI_C_GetVersionString(void)
{
    g_returnString = std::to_string(CEGUI_VERSION_MAJOR) + "." +
                     std::to_string(CEGUI_VERSION_MINOR) + "." +
                     std::to_string(CEGUI_VERSION_PATCH);
    return g_returnString.c_str();
}

CEGUI_C_Bool CEGUI_C_CALL CEGUI_C_IsUnicodeStringBuild(void)
{
#if defined(CEGUI_STRING_CLASS_UNICODE)
    return 1;
#else
    return 0;
#endif
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_GetExisting(CEGUI_C_System* out_system)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_system, "out_system");
        CEGUI::System* system = CEGUI::System::getSingletonPtr();
        if (!system)
            return fail(CEGUI_C_RESULT_NOT_INITIALISED, "CEGUI::System has not been created.");
        *out_system = toSystem(system);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_GetDefaultGUIContext(
    CEGUI_C_System systemHandle,
    CEGUI_C_GUIContext* out_context)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_context, "out_context");
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_context = toContext(&system->getDefaultGUIContext());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_RenderAllGUIContexts(CEGUI_C_System systemHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        system->renderAllGUIContexts();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_InjectTimePulse(
    CEGUI_C_System systemHandle,
    float elapsed_seconds,
    CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = system->injectTimePulse(elapsed_seconds) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_NotifyDisplaySizeChanged(
    CEGUI_C_System systemHandle,
    float width,
    float height)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        system->notifyDisplaySizeChanged(CEGUI::Sizef(width, height));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_SetGroupDirectory(
    CEGUI_C_System systemHandle,
    const char* resource_group_utf8,
    const char* directory_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(resource_group_utf8, "resource_group_utf8");
        CEGUI_C_REQUIRE(directory_utf8, "directory_utf8");
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        auto* provider = dynamic_cast<CEGUI::DefaultResourceProvider*>(system->getResourceProvider());
        if (!provider)
            return fail(CEGUI_C_RESULT_NOT_SUPPORTED, "The active resource provider is not CEGUI::DefaultResourceProvider.");
        provider->setResourceGroupDirectory(fromUtf8(resource_group_utf8), fromUtf8(directory_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_GetGroupDirectory(
    CEGUI_C_System systemHandle,
    const char* resource_group_utf8,
    const char** out_directory_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(resource_group_utf8, "resource_group_utf8");
        CEGUI_C_REQUIRE(out_directory_utf8, "out_directory_utf8");
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        auto* provider = dynamic_cast<CEGUI::DefaultResourceProvider*>(system->getResourceProvider());
        if (!provider)
            return fail(CEGUI_C_RESULT_NOT_SUPPORTED, "The active resource provider is not CEGUI::DefaultResourceProvider.");
        *out_directory_utf8 = storeReturnString(provider->getResourceGroupDirectory(fromUtf8(resource_group_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_ClearGroupDirectory(
    CEGUI_C_System systemHandle,
    const char* resource_group_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(resource_group_utf8, "resource_group_utf8");
        CEGUI::System* system = nullptr;
        const CEGUI_C_Result result = requireSystem(systemHandle, system);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        auto* provider = dynamic_cast<CEGUI::DefaultResourceProvider*>(system->getResourceProvider());
        if (!provider)
            return fail(CEGUI_C_RESULT_NOT_SUPPORTED, "The active resource provider is not CEGUI::DefaultResourceProvider.");
        provider->clearResourceGroupDirectory(fromUtf8(resource_group_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scheme_LoadFromFile(const char* filename_utf8, const char* resource_group_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(filename_utf8, "filename_utf8");
        CEGUI::SchemeManager::getSingleton().createFromFile(fromUtf8(filename_utf8), fromUtf8(resource_group_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scheme_IsDefined(const char* name_utf8, CEGUI_C_Bool* out_defined)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(out_defined, "out_defined");
        *out_defined = CEGUI::SchemeManager::getSingleton().isDefined(fromUtf8(name_utf8)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Font_LoadFromFile(const char* filename_utf8, const char* resource_group_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(filename_utf8, "filename_utf8");
        CEGUI::FontManager::getSingleton().createFromFile(fromUtf8(filename_utf8), fromUtf8(resource_group_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Font_IsDefined(const char* name_utf8, CEGUI_C_Bool* out_defined)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(out_defined, "out_defined");
        *out_defined = CEGUI::FontManager::getSingleton().isDefined(fromUtf8(name_utf8)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Image_AddFromFile(
    const char* name_utf8,
    const char* filename_utf8,
    const char* resource_group_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(filename_utf8, "filename_utf8");
        CEGUI::ImageManager::getSingleton().addFromImageFile(
            fromUtf8(name_utf8), fromUtf8(filename_utf8), fromUtf8(resource_group_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SetDefaultResourceGroups(
    const char* schemes_utf8,
    const char* imagesets_utf8,
    const char* fonts_utf8,
    const char* layouts_utf8,
    const char* looknfeels_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Scheme::setDefaultResourceGroup(fromUtf8(schemes_utf8));
        CEGUI::ImageManager::setImagesetDefaultResourceGroup(fromUtf8(imagesets_utf8));
        CEGUI::Font::setDefaultResourceGroup(fromUtf8(fonts_utf8));
        CEGUI::WindowManager::setDefaultResourceGroup(fromUtf8(layouts_utf8));
        CEGUI::WidgetLookManager::setDefaultResourceGroup(fromUtf8(looknfeels_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_CreateWindow(
    const char* type_utf8,
    const char* name_utf8,
    CEGUI_C_Window* out_window)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(type_utf8, "type_utf8");
        CEGUI_C_REQUIRE(out_window, "out_window");
        *out_window = toWindow(CEGUI::WindowManager::getSingleton().createWindow(fromUtf8(type_utf8), fromUtf8(name_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_LoadLayoutFromFile(
    const char* filename_utf8,
    const char* resource_group_utf8,
    CEGUI_C_Window* out_root_window)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(filename_utf8, "filename_utf8");
        CEGUI_C_REQUIRE(out_root_window, "out_root_window");
        *out_root_window = toWindow(CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
            fromUtf8(filename_utf8), fromUtf8(resource_group_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_GetWindow(const char* name_utf8, CEGUI_C_Window* out_window)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(out_window, "out_window");
        CEGUI::Window* window = findWindowByName(fromUtf8(name_utf8));
        if (!window)
            return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "No live CEGUI window has the requested name.");
        *out_window = toWindow(window);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_IsWindowPresent(const char* name_utf8, CEGUI_C_Bool* out_present)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(out_present, "out_present");
        *out_present = findWindowByName(fromUtf8(name_utf8)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_IsAlive(CEGUI_C_Window windowHandle, CEGUI_C_Bool* out_alive)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_alive, "out_alive");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_alive = CEGUI::WindowManager::getSingleton().isAlive(window) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_DestroyWindow(CEGUI_C_Window windowHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        CEGUI::WindowManager::getSingleton().destroyWindow(window);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_DestroyAllWindows(void)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::WindowManager::getSingleton().destroyAllWindows();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_CleanDeadPool(void)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::WindowManager::getSingleton().cleanDeadPool();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_GetRootWindow(CEGUI_C_GUIContext contextHandle, CEGUI_C_Window* out_window)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_window, "out_window");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_window = toWindow(context->getRootWindow());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_SetRootWindow(CEGUI_C_GUIContext contextHandle, CEGUI_C_Window windowHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        context->setRootWindow(asWindow(windowHandle));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_Draw(CEGUI_C_GUIContext contextHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        context->draw();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_SetDefaultFont(CEGUI_C_GUIContext contextHandle, const char* font_name_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(font_name_utf8, "font_name_utf8");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        context->setDefaultFont(fromUtf8(font_name_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

#define CEGUI_CONTEXT_BOOL_INJECTOR(functionName, expression) \
CEGUI_C_Result CEGUI_C_CALL functionName(CEGUI_C_GUIContext contextHandle, CEGUI_C_Bool* out_handled) \
{ \
    CEGUI_C_BEGIN() \
    { \
        CEGUI_C_REQUIRE(out_handled, "out_handled"); \
        CEGUI::GUIContext* context = nullptr; \
        const CEGUI_C_Result result = requireContext(contextHandle, context); \
        if (result != CEGUI_C_RESULT_SUCCESS) return result; \
        *out_handled = (expression) ? 1 : 0; \
        return CEGUI_C_RESULT_SUCCESS; \
    } \
    CEGUI_C_END() \
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMousePosition(
    CEGUI_C_GUIContext contextHandle, float x, float y, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectMousePosition(x, y) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseMove(
    CEGUI_C_GUIContext contextHandle, float delta_x, float delta_y, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectMouseMove(delta_x, delta_y) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseWheelChange(
    CEGUI_C_GUIContext contextHandle, float delta, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectMouseWheelChange(delta) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseButtonDown(
    CEGUI_C_GUIContext contextHandle, CEGUI_C_MouseButton button, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectMouseButtonDown(toMouseButton(button)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseButtonUp(
    CEGUI_C_GUIContext contextHandle, CEGUI_C_MouseButton button, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectMouseButtonUp(toMouseButton(button)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectKeyDown(
    CEGUI_C_GUIContext contextHandle, uint32_t scan_code, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectKeyDown(static_cast<CEGUI::Key::Scan>(scan_code)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectKeyUp(
    CEGUI_C_GUIContext contextHandle, uint32_t scan_code, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectKeyUp(static_cast<CEGUI::Key::Scan>(scan_code)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectChar(
    CEGUI_C_GUIContext contextHandle, uint32_t codepoint, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
#if !defined(CEGUI_STRING_CLASS_UNICODE)
        if (codepoint > 0xFFu)
            return fail(CEGUI_C_RESULT_NOT_SUPPORTED,
                "This CEGUI build uses an 8-bit String type. Rebuild with CEGUI_STRING_CLASS=UNICODE for full Unicode input.");
#endif
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectChar(static_cast<CEGUI::String::value_type>(codepoint)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectTimePulse(
    CEGUI_C_GUIContext contextHandle, float elapsed_seconds, CEGUI_C_Bool* out_handled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_handled, "out_handled");
        CEGUI::GUIContext* context = nullptr;
        const CEGUI_C_Result result = requireContext(contextHandle, context);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_handled = context->injectTimePulse(elapsed_seconds) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_CONTEXT_BOOL_INJECTOR(CEGUI_C_GUIContext_InjectCopyRequest, context->injectCopyRequest())
CEGUI_CONTEXT_BOOL_INJECTOR(CEGUI_C_GUIContext_InjectCutRequest, context->injectCutRequest())
CEGUI_CONTEXT_BOOL_INJECTOR(CEGUI_C_GUIContext_InjectPasteRequest, context->injectPasteRequest())

#undef CEGUI_CONTEXT_BOOL_INJECTOR

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetName(CEGUI_C_Window windowHandle, const char** out_name_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_name_utf8, "out_name_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_name_utf8 = storeReturnString(window->getName());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetType(CEGUI_C_Window windowHandle, const char** out_type_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_type_utf8, "out_type_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_type_utf8 = storeReturnString(window->getType());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetText(CEGUI_C_Window windowHandle, const char** out_text_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_text_utf8, "out_text_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_text_utf8 = storeReturnString(window->getText());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetText(CEGUI_C_Window windowHandle, const char* text_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(text_utf8, "text_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setText(fromUtf8(text_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetProperty(
    CEGUI_C_Window windowHandle, const char* property_utf8, const char* value_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(property_utf8, "property_utf8");
        CEGUI_C_REQUIRE(value_utf8, "value_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setProperty(fromUtf8(property_utf8), fromUtf8(value_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetProperty(
    CEGUI_C_Window windowHandle, const char* property_utf8, const char** out_value_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(property_utf8, "property_utf8");
        CEGUI_C_REQUIRE(out_value_utf8, "out_value_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_value_utf8 = storeReturnString(window->getProperty(fromUtf8(property_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsPropertyPresent(
    CEGUI_C_Window windowHandle, const char* property_utf8, CEGUI_C_Bool* out_present)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(property_utf8, "property_utf8");
        CEGUI_C_REQUIRE(out_present, "out_present");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_present = window->isPropertyPresent(fromUtf8(property_utf8)) ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_AddChild(CEGUI_C_Window parentHandle, CEGUI_C_Window childHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* parent = nullptr;
        CEGUI::Window* child = nullptr;
        CEGUI_C_Result result = requireWindow(parentHandle, parent);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        result = requireWindow(childHandle, child);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        parent->addChild(child);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_RemoveChild(CEGUI_C_Window parentHandle, CEGUI_C_Window childHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* parent = nullptr;
        CEGUI::Window* child = nullptr;
        CEGUI_C_Result result = requireWindow(parentHandle, parent);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        result = requireWindow(childHandle, child);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        parent->removeChild(child);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetParent(CEGUI_C_Window windowHandle, CEGUI_C_Window* out_parent)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_parent, "out_parent");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_parent = toWindow(window->getParent());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChild(
    CEGUI_C_Window windowHandle, const char* name_path_utf8, CEGUI_C_Window* out_child)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_path_utf8, "name_path_utf8");
        CEGUI_C_REQUIRE(out_child, "out_child");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_child = toWindow(window->getChild(fromUtf8(name_path_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildRecursive(
    CEGUI_C_Window windowHandle, const char* name_utf8, CEGUI_C_Window* out_child)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(name_utf8, "name_utf8");
        CEGUI_C_REQUIRE(out_child, "out_child");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_child = toWindow(window->getChildRecursive(fromUtf8(name_utf8)));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildCount(CEGUI_C_Window windowHandle, size_t* out_count)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_count, "out_count");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_count = window->getChildCount();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildAtIndex(
    CEGUI_C_Window windowHandle, size_t index, CEGUI_C_Window* out_child)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_child, "out_child");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        if (index >= window->getChildCount())
            return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "Child index is out of range.");
        *out_child = toWindow(window->getChildAtIdx(index));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetVisible(CEGUI_C_Window windowHandle, CEGUI_C_Bool visible)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setVisible(visible != 0);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsVisible(CEGUI_C_Window windowHandle, CEGUI_C_Bool* out_visible)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_visible, "out_visible");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_visible = window->isVisible() ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetEnabled(CEGUI_C_Window windowHandle, CEGUI_C_Bool enabled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setEnabled(enabled != 0);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsEnabled(CEGUI_C_Window windowHandle, CEGUI_C_Bool* out_enabled)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_enabled, "out_enabled");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_enabled = window->isDisabled() ? 0 : 1;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_Activate(CEGUI_C_Window windowHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->activate();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_Deactivate(CEGUI_C_Window windowHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->deactivate();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsActive(CEGUI_C_Window windowHandle, CEGUI_C_Bool* out_active)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_active, "out_active");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_active = window->isActive() ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetPosition(CEGUI_C_Window windowHandle, CEGUI_C_UVector2 position)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setPosition(toUVector2(position));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetPosition(CEGUI_C_Window windowHandle, CEGUI_C_UVector2* out_position)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_position, "out_position");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_position = fromUVector2(window->getPosition());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetSize(CEGUI_C_Window windowHandle, CEGUI_C_USize size)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setSize(toUSize(size));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetSize(CEGUI_C_Window windowHandle, CEGUI_C_USize* out_size)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_size, "out_size");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_size = fromUSize(window->getSize());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetArea(
    CEGUI_C_Window windowHandle, CEGUI_C_UVector2 position, CEGUI_C_USize size)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setArea(toUVector2(position), toUSize(size));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetAlpha(CEGUI_C_Window windowHandle, float alpha)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setAlpha(alpha);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetAlpha(CEGUI_C_Window windowHandle, float* out_alpha)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_alpha, "out_alpha");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_alpha = window->getAlpha();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetID(CEGUI_C_Window windowHandle, uint32_t id)
{
    CEGUI_C_BEGIN()
    {
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setID(id);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetID(CEGUI_C_Window windowHandle, uint32_t* out_id)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(out_id, "out_id");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        *out_id = window->getID();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetFont(CEGUI_C_Window windowHandle, const char* font_name_utf8)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(font_name_utf8, "font_name_utf8");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;
        window->setFont(fromUtf8(font_name_utf8));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SubscribeEvent(
    CEGUI_C_Window windowHandle,
    const char* event_name_utf8,
    CEGUI_C_EventCallback callback,
    void* user_data,
    CEGUI_C_EventConnection* out_connection)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(event_name_utf8, "event_name_utf8");
        if (!callback) return fail(CEGUI_C_RESULT_INVALID_ARGUMENT, "callback must not be null.");
        CEGUI_C_REQUIRE(out_connection, "out_connection");
        CEGUI::Window* window = nullptr;
        const CEGUI_C_Result result = requireWindow(windowHandle, window);
        if (result != CEGUI_C_RESULT_SUCCESS) return result;

        auto holder = std::make_unique<EventConnectionHolder>();
        holder->connection = window->subscribeEvent(
            fromUtf8(event_name_utf8),
            CEGUI::Event::Subscriber([callback, user_data](const CEGUI::EventArgs& args) -> bool
            {
                const CEGUI_C_EventData data = makeEventData(args);
                return callback(&data, user_data) != 0;
            }));

        *out_connection = toConnection(holder.release());
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_EventConnection_IsConnected(
    CEGUI_C_EventConnection connectionHandle,
    CEGUI_C_Bool* out_connected)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(connectionHandle, "connection");
        CEGUI_C_REQUIRE(out_connected, "out_connected");
        EventConnectionHolder* holder = asConnection(connectionHandle);
        *out_connected = holder->connection.isValid() && holder->connection->connected() ? 1 : 0;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_EventConnection_Disconnect(CEGUI_C_EventConnection connectionHandle)
{
    CEGUI_C_BEGIN()
    {
        CEGUI_C_REQUIRE(connectionHandle, "connection");
        EventConnectionHolder* holder = asConnection(connectionHandle);
        if (holder->connection.isValid())
            holder->connection->disconnect();
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_C_END()
}

void CEGUI_C_CALL CEGUI_C_EventConnection_Release(CEGUI_C_EventConnection connectionHandle)
{
    if (!connectionHandle)
        return;

    EventConnectionHolder* holder = asConnection(connectionHandle);
    try
    {
        if (holder->connection.isValid())
            holder->connection->disconnect();
    }
    catch (const CEGUI::Exception& exception)
    {
        g_lastError = toUtf8Copy(exception.getMessage());
    }
    catch (const std::exception& exception)
    {
        g_lastError = exception.what();
    }
    catch (...)
    {
        g_lastError = "Unknown native exception while releasing an event connection.";
    }

    delete holder;
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ProgressBar_SetProgress(CEGUI_C_Window window, float progress)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::ProgressBar>(window, "CEGUI::ProgressBar")->setProgress(progress); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ProgressBar_GetProgress(CEGUI_C_Window window, float* out_progress)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_progress, "out_progress"); *out_progress = checkedWidget<CEGUI::ProgressBar>(window, "CEGUI::ProgressBar")->getProgress(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ToggleButton_SetSelected(CEGUI_C_Window window, CEGUI_C_Bool selected)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::ToggleButton>(window, "CEGUI::ToggleButton")->setSelected(selected != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ToggleButton_IsSelected(CEGUI_C_Window window, CEGUI_C_Bool* out_selected)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_selected, "out_selected"); *out_selected = checkedWidget<CEGUI::ToggleButton>(window, "CEGUI::ToggleButton")->isSelected() ? 1 : 0; return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetReadOnly(CEGUI_C_Window window, CEGUI_C_Bool read_only)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox")->setReadOnly(read_only != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_IsReadOnly(CEGUI_C_Window window, CEGUI_C_Bool* out_read_only)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_read_only, "out_read_only"); *out_read_only = checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox")->isReadOnly() ? 1 : 0; return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetMaxTextLength(CEGUI_C_Window window, size_t max_length)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox")->setMaxTextLength(max_length); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_GetMaxTextLength(CEGUI_C_Window window, size_t* out_max_length)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_max_length, "out_max_length"); *out_max_length = checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox")->getMaxTextLength(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetSelection(CEGUI_C_Window window, size_t start, size_t end)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox")->setSelection(start, end); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_GetSelection(CEGUI_C_Window window, size_t* out_start, size_t* out_end)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_start, "out_start"); CEGUI_C_REQUIRE(out_end, "out_end"); auto* widget = checkedWidget<CEGUI::Editbox>(window, "CEGUI::Editbox"); *out_start = widget->getSelectionStartIndex(); *out_end = widget->getSelectionEndIndex(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_SetCurrentValue(CEGUI_C_Window window, float value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Slider>(window, "CEGUI::Slider")->setCurrentValue(value); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_GetCurrentValue(CEGUI_C_Window window, float* out_value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_value, "out_value"); *out_value = checkedWidget<CEGUI::Slider>(window, "CEGUI::Slider")->getCurrentValue(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_SetMaxValue(CEGUI_C_Window window, float value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Slider>(window, "CEGUI::Slider")->setMaxValue(value); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_GetMaxValue(CEGUI_C_Window window, float* out_value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_value, "out_value"); *out_value = checkedWidget<CEGUI::Slider>(window, "CEGUI::Slider")->getMaxValue(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scrollbar_SetScrollPosition(CEGUI_C_Window window, float value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); checkedWidget<CEGUI::Scrollbar>(window, "CEGUI::Scrollbar")->setScrollPosition(value); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scrollbar_GetScrollPosition(CEGUI_C_Window window, float* out_value)
{
    CEGUI_C_BEGIN() { CEGUI_C_REQUIRE(window, "window"); CEGUI_C_REQUIRE(out_value, "out_value"); *out_value = checkedWidget<CEGUI::Scrollbar>(window, "CEGUI::Scrollbar")->getScrollPosition(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_C_END()
}

} // extern "C"
