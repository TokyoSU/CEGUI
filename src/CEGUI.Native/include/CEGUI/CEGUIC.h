#ifndef CEGUI_C_BINDING_H
#define CEGUI_C_BINDING_H

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#  if defined(CEGUI_NATIVE_EXPORTS)
#    define CEGUI_C_API __declspec(dllexport)
#  elif defined(CEGUICBINDING_STATIC)
#    define CEGUI_C_API
#  else
#    define CEGUI_C_API __declspec(dllimport)
#  endif
#  define CEGUI_C_CALL __cdecl
#else
#  define CEGUI_C_API __attribute__((visibility("default")))
#  define CEGUI_C_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CEGUI_C_System_t* CEGUI_C_System;
typedef struct CEGUI_C_GUIContext_t* CEGUI_C_GUIContext;
typedef struct CEGUI_C_Window_t* CEGUI_C_Window;
typedef struct CEGUI_C_EventConnection_t* CEGUI_C_EventConnection;

typedef int32_t CEGUI_C_Bool;

typedef enum CEGUI_C_Result
{
    CEGUI_C_RESULT_SUCCESS = 0,
    CEGUI_C_RESULT_INVALID_ARGUMENT = 1,
    CEGUI_C_RESULT_NOT_INITIALISED = 2,
    CEGUI_C_RESULT_CEGUI_EXCEPTION = 3,
    CEGUI_C_RESULT_STD_EXCEPTION = 4,
    CEGUI_C_RESULT_UNKNOWN_EXCEPTION = 5,
    CEGUI_C_RESULT_NOT_SUPPORTED = 6
} CEGUI_C_Result;

typedef enum CEGUI_C_MouseButton
{
    CEGUI_C_MOUSE_LEFT = 0,
    CEGUI_C_MOUSE_RIGHT = 1,
    CEGUI_C_MOUSE_MIDDLE = 2,
    CEGUI_C_MOUSE_X1 = 3,
    CEGUI_C_MOUSE_X2 = 4,
    CEGUI_C_MOUSE_COUNT = 5,
    CEGUI_C_MOUSE_NONE = 6
} CEGUI_C_MouseButton;

typedef enum CEGUI_C_EventKind
{
    CEGUI_C_EVENT_GENERIC = 0,
    CEGUI_C_EVENT_WINDOW = 1,
    CEGUI_C_EVENT_MOUSE = 2,
    CEGUI_C_EVENT_KEY = 3,
    CEGUI_C_EVENT_ACTIVATION = 4,
    CEGUI_C_EVENT_UPDATE = 5
} CEGUI_C_EventKind;

typedef struct CEGUI_C_UDim
{
    float scale;
    float offset;
} CEGUI_C_UDim;

typedef struct CEGUI_C_UVector2
{
    CEGUI_C_UDim x;
    CEGUI_C_UDim y;
} CEGUI_C_UVector2;

typedef struct CEGUI_C_USize
{
    CEGUI_C_UDim width;
    CEGUI_C_UDim height;
} CEGUI_C_USize;

typedef struct CEGUI_C_Sizef
{
    float width;
    float height;
} CEGUI_C_Sizef;

typedef struct CEGUI_C_EventData
{
    CEGUI_C_EventKind kind;
    CEGUI_C_Window window;
    CEGUI_C_Window other_window;
    uint32_t handled_count;
    float position_x;
    float position_y;
    float move_delta_x;
    float move_delta_y;
    float wheel_change;
    uint32_t mouse_button;
    uint32_t click_count;
    uint32_t system_keys;
    uint32_t codepoint;
    uint32_t scancode;
    float elapsed;
} CEGUI_C_EventData;

typedef CEGUI_C_Bool (CEGUI_C_CALL *CEGUI_C_EventCallback)(
    const CEGUI_C_EventData* event_data,
    void* user_data);

/* Error and version information. Returned strings remain valid until the next
   C binding call on the same thread. */
CEGUI_C_API const char* CEGUI_C_CALL CEGUI_C_GetLastError(void);
CEGUI_C_API void CEGUI_C_CALL CEGUI_C_ClearLastError(void);
CEGUI_C_API uint32_t CEGUI_C_CALL CEGUI_C_GetVersionMajor(void);
CEGUI_C_API uint32_t CEGUI_C_CALL CEGUI_C_GetVersionMinor(void);
CEGUI_C_API uint32_t CEGUI_C_CALL CEGUI_C_GetVersionPatch(void);
CEGUI_C_API const char* CEGUI_C_CALL CEGUI_C_GetVersionString(void);
CEGUI_C_API CEGUI_C_Bool CEGUI_C_CALL CEGUI_C_IsUnicodeStringBuild(void);

/* System lifecycle. D3D11 pointers are ID3D11Device* and
   ID3D11DeviceContext*. CEGUI retains its original ownership semantics. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_BootstrapSystem(
    void* device,
    void* device_context,
    CEGUI_C_System* out_system);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_D3D11_DestroySystem(void);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_BootstrapSystem(
    float width,
    float height,
    uint16_t view_id_base,
    uint16_t view_id_count,
    const char* shader_root_utf8,
    CEGUI_C_System* out_system);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Bgfx_DestroySystem(void);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_GetExisting(
    CEGUI_C_System* out_system);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_GetDefaultGUIContext(
    CEGUI_C_System system,
    CEGUI_C_GUIContext* out_context);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_RenderAllGUIContexts(
    CEGUI_C_System system);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_InjectTimePulse(
    CEGUI_C_System system,
    float elapsed_seconds,
    CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_System_NotifyDisplaySizeChanged(
    CEGUI_C_System system,
    float width,
    float height);

/* DefaultResourceProvider. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_SetGroupDirectory(
    CEGUI_C_System system,
    const char* resource_group_utf8,
    const char* directory_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_GetGroupDirectory(
    CEGUI_C_System system,
    const char* resource_group_utf8,
    const char** out_directory_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ResourceProvider_ClearGroupDirectory(
    CEGUI_C_System system,
    const char* resource_group_utf8);

/* Resource loading. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scheme_LoadFromFile(
    const char* filename_utf8,
    const char* resource_group_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scheme_IsDefined(
    const char* name_utf8,
    CEGUI_C_Bool* out_defined);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Font_LoadFromFile(
    const char* filename_utf8,
    const char* resource_group_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Font_IsDefined(
    const char* name_utf8,
    CEGUI_C_Bool* out_defined);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Image_AddFromFile(
    const char* name_utf8,
    const char* filename_utf8,
    const char* resource_group_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SetDefaultResourceGroups(
    const char* schemes_utf8,
    const char* imagesets_utf8,
    const char* fonts_utf8,
    const char* layouts_utf8,
    const char* looknfeels_utf8);

/* WindowManager. Window handles are borrowed and are invalid after the native
   WindowManager destroys the corresponding window. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_CreateWindow(
    const char* type_utf8,
    const char* name_utf8,
    CEGUI_C_Window* out_window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_LoadLayoutFromFile(
    const char* filename_utf8,
    const char* resource_group_utf8,
    CEGUI_C_Window* out_root_window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_GetWindow(
    const char* name_utf8,
    CEGUI_C_Window* out_window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_IsWindowPresent(
    const char* name_utf8,
    CEGUI_C_Bool* out_present);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_IsAlive(
    CEGUI_C_Window window,
    CEGUI_C_Bool* out_alive);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_DestroyWindow(
    CEGUI_C_Window window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_DestroyAllWindows(void);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_WindowManager_CleanDeadPool(void);

/* GUIContext and input injection. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_GetRootWindow(
    CEGUI_C_GUIContext context,
    CEGUI_C_Window* out_window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_SetRootWindow(
    CEGUI_C_GUIContext context,
    CEGUI_C_Window window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_Draw(
    CEGUI_C_GUIContext context);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_SetDefaultFont(
    CEGUI_C_GUIContext context,
    const char* font_name_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMousePosition(
    CEGUI_C_GUIContext context, float x, float y, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseMove(
    CEGUI_C_GUIContext context, float delta_x, float delta_y, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseWheelChange(
    CEGUI_C_GUIContext context, float delta, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseButtonDown(
    CEGUI_C_GUIContext context, CEGUI_C_MouseButton button, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectMouseButtonUp(
    CEGUI_C_GUIContext context, CEGUI_C_MouseButton button, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectKeyDown(
    CEGUI_C_GUIContext context, uint32_t scan_code, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectKeyUp(
    CEGUI_C_GUIContext context, uint32_t scan_code, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectChar(
    CEGUI_C_GUIContext context, uint32_t codepoint, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectTimePulse(
    CEGUI_C_GUIContext context, float elapsed_seconds, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectCopyRequest(
    CEGUI_C_GUIContext context, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectCutRequest(
    CEGUI_C_GUIContext context, CEGUI_C_Bool* out_handled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GUIContext_InjectPasteRequest(
    CEGUI_C_GUIContext context, CEGUI_C_Bool* out_handled);

/* Generic Window operations. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetName(
    CEGUI_C_Window window, const char** out_name_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetType(
    CEGUI_C_Window window, const char** out_type_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetText(
    CEGUI_C_Window window, const char** out_text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetText(
    CEGUI_C_Window window, const char* text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetProperty(
    CEGUI_C_Window window, const char* property_utf8, const char* value_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetProperty(
    CEGUI_C_Window window, const char* property_utf8, const char** out_value_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsPropertyPresent(
    CEGUI_C_Window window, const char* property_utf8, CEGUI_C_Bool* out_present);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_AddChild(
    CEGUI_C_Window parent, CEGUI_C_Window child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_RemoveChild(
    CEGUI_C_Window parent, CEGUI_C_Window child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetParent(
    CEGUI_C_Window window, CEGUI_C_Window* out_parent);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChild(
    CEGUI_C_Window window, const char* name_path_utf8, CEGUI_C_Window* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildRecursive(
    CEGUI_C_Window window, const char* name_utf8, CEGUI_C_Window* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildCount(
    CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetChildAtIndex(
    CEGUI_C_Window window, size_t index, CEGUI_C_Window* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetVisible(
    CEGUI_C_Window window, CEGUI_C_Bool visible);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsVisible(
    CEGUI_C_Window window, CEGUI_C_Bool* out_visible);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetEnabled(
    CEGUI_C_Window window, CEGUI_C_Bool enabled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsEnabled(
    CEGUI_C_Window window, CEGUI_C_Bool* out_enabled);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_Activate(CEGUI_C_Window window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_Deactivate(CEGUI_C_Window window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_IsActive(
    CEGUI_C_Window window, CEGUI_C_Bool* out_active);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetPosition(
    CEGUI_C_Window window, CEGUI_C_UVector2 position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetPosition(
    CEGUI_C_Window window, CEGUI_C_UVector2* out_position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetSize(
    CEGUI_C_Window window, CEGUI_C_USize size);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetSize(
    CEGUI_C_Window window, CEGUI_C_USize* out_size);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetArea(
    CEGUI_C_Window window, CEGUI_C_UVector2 position, CEGUI_C_USize size);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetAlpha(
    CEGUI_C_Window window, float alpha);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetAlpha(
    CEGUI_C_Window window, float* out_alpha);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetID(
    CEGUI_C_Window window, uint32_t id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetID(
    CEGUI_C_Window window, uint32_t* out_id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SetFont(
    CEGUI_C_Window window, const char* font_name_utf8);

/* Events. The returned connection owns the subscription and must be released. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_SubscribeEvent(
    CEGUI_C_Window window,
    const char* event_name_utf8,
    CEGUI_C_EventCallback callback,
    void* user_data,
    CEGUI_C_EventConnection* out_connection);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_EventConnection_IsConnected(
    CEGUI_C_EventConnection connection,
    CEGUI_C_Bool* out_connected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_EventConnection_Disconnect(
    CEGUI_C_EventConnection connection);
CEGUI_C_API void CEGUI_C_CALL CEGUI_C_EventConnection_Release(
    CEGUI_C_EventConnection connection);

/* Common typed widget helpers. Each function validates the native widget type. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ProgressBar_SetProgress(
    CEGUI_C_Window window, float progress);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ProgressBar_GetProgress(
    CEGUI_C_Window window, float* out_progress);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ToggleButton_SetSelected(
    CEGUI_C_Window window, CEGUI_C_Bool selected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ToggleButton_IsSelected(
    CEGUI_C_Window window, CEGUI_C_Bool* out_selected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetReadOnly(
    CEGUI_C_Window window, CEGUI_C_Bool read_only);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_IsReadOnly(
    CEGUI_C_Window window, CEGUI_C_Bool* out_read_only);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetMaxTextLength(
    CEGUI_C_Window window, size_t max_length);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_GetMaxTextLength(
    CEGUI_C_Window window, size_t* out_max_length);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_SetSelection(
    CEGUI_C_Window window, size_t start, size_t end);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Editbox_GetSelection(
    CEGUI_C_Window window, size_t* out_start, size_t* out_end);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_SetCurrentValue(
    CEGUI_C_Window window, float value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_GetCurrentValue(
    CEGUI_C_Window window, float* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_SetMaxValue(
    CEGUI_C_Window window, float value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Slider_GetMaxValue(
    CEGUI_C_Window window, float* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scrollbar_SetScrollPosition(
    CEGUI_C_Window window, float value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Scrollbar_GetScrollPosition(
    CEGUI_C_Window window, float* out_value);

#ifdef __cplusplus
}
#endif

#include "CEGUIWidgets.h"

#endif
