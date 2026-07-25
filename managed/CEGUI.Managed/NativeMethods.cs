using System;
using System.Runtime.InteropServices;

namespace CEGUI;

internal static class NativeMethods
{
    internal const string LibraryName = "CEGUI.Native";

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate int EventCallback(ref NativeEventData eventData, IntPtr userData);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr CEGUI_C_GetLastError();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern uint CEGUI_C_GetVersionMajor();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern uint CEGUI_C_GetVersionMinor();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern uint CEGUI_C_GetVersionPatch();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr CEGUI_C_GetVersionString();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int CEGUI_C_IsUnicodeStringBuild();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_D3D11_BootstrapSystem(IntPtr device, IntPtr deviceContext, out IntPtr system);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_D3D11_DestroySystem();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_System_GetExisting(out IntPtr system);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_System_GetDefaultGUIContext(IntPtr system, out IntPtr context);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_System_RenderAllGUIContexts(IntPtr system);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_System_InjectTimePulse(IntPtr system, float elapsedSeconds, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_System_NotifyDisplaySizeChanged(IntPtr system, float width, float height);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ResourceProvider_SetGroupDirectory(
        IntPtr system,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceGroup,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string directory);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ResourceProvider_GetGroupDirectory(
        IntPtr system,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceGroup,
        out IntPtr directory);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ResourceProvider_ClearGroupDirectory(
        IntPtr system,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceGroup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Scheme_LoadFromFile(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string filename,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? resourceGroup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Scheme_IsDefined(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        out int defined);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Font_LoadFromFile(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string filename,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? resourceGroup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Font_IsDefined(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        out int defined);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Image_AddFromFile(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string filename,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? resourceGroup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_SetDefaultResourceGroups(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? schemes,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? imagesets,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? fonts,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? layouts,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? lookNFeels);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_CreateWindow(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string type,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? name,
        out IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_LoadLayoutFromFile(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string filename,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string? resourceGroup,
        out IntPtr rootWindow);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_GetWindow(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        out IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_IsWindowPresent(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        out int present);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_IsAlive(IntPtr window, out int alive);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_DestroyWindow(IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_DestroyAllWindows();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_WindowManager_CleanDeadPool();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_GetRootWindow(IntPtr context, out IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_SetRootWindow(IntPtr context, IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_Draw(IntPtr context);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_SetDefaultFont(
        IntPtr context,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string fontName);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectMousePosition(IntPtr context, float x, float y, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectMouseMove(IntPtr context, float deltaX, float deltaY, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectMouseWheelChange(IntPtr context, float delta, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectMouseButtonDown(IntPtr context, MouseButton button, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectMouseButtonUp(IntPtr context, MouseButton button, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectKeyDown(IntPtr context, KeyScan scanCode, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectKeyUp(IntPtr context, KeyScan scanCode, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectChar(IntPtr context, uint codepoint, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectTimePulse(IntPtr context, float elapsedSeconds, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectCopyRequest(IntPtr context, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectCutRequest(IntPtr context, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GUIContext_InjectPasteRequest(IntPtr context, out int handled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetName(IntPtr window, out IntPtr name);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetType(IntPtr window, out IntPtr type);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetText(IntPtr window, out IntPtr text);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetText(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetProperty(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string property,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetProperty(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string property,
        out IntPtr value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_IsPropertyPresent(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string property,
        out int present);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_AddChild(IntPtr parent, IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_RemoveChild(IntPtr parent, IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetParent(IntPtr window, out IntPtr parent);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetChild(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string namePath,
        out IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetChildRecursive(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name,
        out IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetChildCount(IntPtr window, out UIntPtr count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetChildAtIndex(IntPtr window, UIntPtr index, out IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetVisible(IntPtr window, int visible);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_IsVisible(IntPtr window, out int visible);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetEnabled(IntPtr window, int enabled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_IsEnabled(IntPtr window, out int enabled);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_Activate(IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_Deactivate(IntPtr window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_IsActive(IntPtr window, out int active);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetPosition(IntPtr window, UVector2 position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetPosition(IntPtr window, out UVector2 position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetSize(IntPtr window, USize size);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetSize(IntPtr window, out USize size);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetArea(IntPtr window, UVector2 position, USize size);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetAlpha(IntPtr window, float alpha);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetAlpha(IntPtr window, out float alpha);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetID(IntPtr window, uint id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetID(IntPtr window, out uint id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SetFont(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string fontName);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_SubscribeEvent(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string eventName,
        EventCallback callback,
        IntPtr userData,
        out IntPtr connection);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_EventConnection_IsConnected(IntPtr connection, out int connected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_EventConnection_Disconnect(IntPtr connection);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void CEGUI_C_EventConnection_Release(IntPtr connection);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ProgressBar_SetProgress(IntPtr window, float progress);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ProgressBar_GetProgress(IntPtr window, out float progress);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ToggleButton_SetSelected(IntPtr window, int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ToggleButton_IsSelected(IntPtr window, out int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_SetReadOnly(IntPtr window, int readOnly);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_IsReadOnly(IntPtr window, out int readOnly);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_SetMaxTextLength(IntPtr window, UIntPtr maxLength);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_GetMaxTextLength(IntPtr window, out UIntPtr maxLength);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_SetSelection(IntPtr window, UIntPtr start, UIntPtr end);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Editbox_GetSelection(IntPtr window, out UIntPtr start, out UIntPtr end);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Slider_SetCurrentValue(IntPtr window, float value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Slider_GetCurrentValue(IntPtr window, out float value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Slider_SetMaxValue(IntPtr window, float value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Slider_GetMaxValue(IntPtr window, out float value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Scrollbar_SetScrollPosition(IntPtr window, float value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Scrollbar_GetScrollPosition(IntPtr window, out float value);

    // BEGIN GENERATED WIDGET INTEROP
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Window_GetWidgetKind(IntPtr window, out WidgetKind out_kind);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Widget_GetComponent(
        IntPtr window,
        WidgetComponent component,
        out IntPtr out_window);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Widget_ExecuteAction(IntPtr window, WidgetAction action);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxTextItem_Create(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint id,
        int disabled,
        int auto_delete,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_Destroy(IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_GetText(IntPtr item, out IntPtr out_text_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetText(
        IntPtr item,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_GetID(IntPtr item, out uint out_id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetID(IntPtr item, uint id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_GetUserData(IntPtr item, out UIntPtr out_data);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetUserData(IntPtr item, UIntPtr data);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_IsSelected(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetSelected(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_IsDisabled(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetDisabled(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_IsAutoDeleted(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetAutoDeleted(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxTextItem_SetFont(
        IntPtr item,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string font_name_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListboxItem_SetSelectionBrushImage(
        IntPtr item,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string image_name_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_GetItemCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_GetSelectedCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_GetItemAt(
        IntPtr window,
        UIntPtr index,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_GetFirstSelected(IntPtr window, out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_GetNextSelected(
        IntPtr window,
        IntPtr start_item,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_AddItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_InsertItem(
        IntPtr window,
        IntPtr item,
        IntPtr before_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_RemoveItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_SetItemSelected(
        IntPtr window,
        IntPtr item,
        int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Listbox_EnsureItemVisible(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_GetItemCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_GetItemAt(
        IntPtr window,
        UIntPtr index,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_GetSelectedItem(IntPtr window, out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_AddItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_InsertItem(
        IntPtr window,
        IntPtr item,
        IntPtr before_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_RemoveItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Combobox_SetItemSelected(
        IntPtr window,
        IntPtr item,
        int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetColumnCount(IntPtr window, out uint out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetSegmentAt(
        IntPtr window,
        uint column,
        out IntPtr out_segment);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetSegmentByID(
        IntPtr window,
        uint id,
        out IntPtr out_segment);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetColumnFromID(
        IntPtr window,
        uint id,
        out uint out_column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetColumnWidth(
        IntPtr window,
        uint column,
        out UDim out_width);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_AddColumn(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint id,
        UDim width);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_InsertColumn(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint id,
        UDim width,
        uint position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_RemoveColumn(IntPtr window, uint column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_MoveColumn(
        IntPtr window,
        uint column,
        uint position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetSortColumn(IntPtr window, out uint out_column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_SetSortColumn(IntPtr window, uint column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_GetSortDirection(IntPtr window, out ListSortDirection out_direction);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ListHeader_SetSortDirection(IntPtr window, ListSortDirection direction);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetColumnCount(IntPtr window, out uint out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetRowCount(IntPtr window, out uint out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_AddColumn(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint column_id,
        UDim width);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_InsertColumn(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint column_id,
        UDim width,
        uint position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_RemoveColumn(IntPtr window, uint column_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_AddRow(
        IntPtr window,
        uint row_id,
        out uint out_row_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_InsertRow(
        IntPtr window,
        uint row_index,
        uint row_id,
        out uint out_actual_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_RemoveRow(IntPtr window, uint row_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_SetItem(
        IntPtr window,
        IntPtr item,
        uint column_id,
        uint row_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetItem(
        IntPtr window,
        MultiColumnGridReference position,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetSelectedCount(IntPtr window, out uint out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetFirstSelected(IntPtr window, out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetNextSelected(
        IntPtr window,
        IntPtr start_item,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_SetItemSelected(
        IntPtr window,
        IntPtr item,
        int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_EnsureItemVisible(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetSelectionMode(IntPtr window, out MultiColumnSelectionMode out_mode);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_SetSelectionMode(IntPtr window, MultiColumnSelectionMode mode);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetSortColumn(IntPtr window, out uint out_column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_SetSortColumn(IntPtr window, uint column);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_GetSortDirection(IntPtr window, out ListSortDirection out_direction);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MultiColumnList_SetSortDirection(IntPtr window, ListSortDirection direction);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListBase_GetItemCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListBase_GetItemAt(
        IntPtr window,
        UIntPtr index,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListBase_AddItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListBase_InsertItem(
        IntPtr window,
        IntPtr item,
        IntPtr before_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListBase_RemoveItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListbox_GetSelectedCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListbox_GetFirstSelected(
        IntPtr window,
        UIntPtr start_index,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListbox_GetNextSelected(
        IntPtr window,
        IntPtr start_item,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_ItemListbox_SelectRange(
        IntPtr window,
        UIntPtr first,
        UIntPtr last);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_Create(
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8,
        uint id,
        int disabled,
        int auto_delete,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_Destroy(IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_GetText(IntPtr item, out IntPtr out_text_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetText(
        IntPtr item,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string text_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_GetID(IntPtr item, out uint out_id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetID(IntPtr item, uint id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_GetUserData(IntPtr item, out UIntPtr out_data);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetUserData(IntPtr item, UIntPtr data);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_IsSelected(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetSelected(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_IsDisabled(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetDisabled(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_IsAutoDeleted(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_SetAutoDeleted(IntPtr item, int value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_IsOpen(IntPtr item, out int out_value);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_ToggleOpen(IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_GetChildCount(IntPtr item, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_GetChildAt(
        IntPtr item,
        UIntPtr index,
        out IntPtr out_child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_AddChild(IntPtr item, IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TreeItem_RemoveChild(IntPtr item, IntPtr child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_GetSelectedCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_GetFirstSelected(IntPtr window, out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_GetNextSelected(
        IntPtr window,
        IntPtr start_item,
        out IntPtr out_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_AddItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_InsertItem(
        IntPtr window,
        IntPtr item,
        IntPtr before_item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_RemoveItem(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_SetItemSelected(
        IntPtr window,
        IntPtr item,
        int selected);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_Tree_EnsureItemVisible(IntPtr window, IntPtr item);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_GetTabCount(IntPtr window, out UIntPtr out_count);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_GetSelectedIndex(IntPtr window, out UIntPtr out_index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_SetSelectedIndex(IntPtr window, UIntPtr index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_GetContentAt(
        IntPtr window,
        UIntPtr index,
        out IntPtr out_content);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_AddTab(IntPtr window, IntPtr content);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_RemoveTabByName(
        IntPtr window,
        [MarshalAs(UnmanagedType.LPUTF8Str)] string name_utf8);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_RemoveTabByID(IntPtr window, uint id);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_MakeVisible(IntPtr window, UIntPtr index);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_GetPanePosition(IntPtr window, out TabPanePosition out_position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_TabControl_SetPanePosition(IntPtr window, TabPanePosition position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MenuItem_GetPopupMenu(IntPtr window, out IntPtr out_popup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MenuItem_SetPopupMenu(IntPtr window, IntPtr popup);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_MenuItem_TogglePopupMenu(IntPtr window, out int out_opened);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_SequentialLayout_GetPosition(
        IntPtr window,
        IntPtr child,
        out UIntPtr out_position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_SequentialLayout_GetChildAt(
        IntPtr window,
        UIntPtr position,
        out IntPtr out_child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_SequentialLayout_MoveChildTo(
        IntPtr window,
        IntPtr child,
        UIntPtr position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_SequentialLayout_AddChildAt(
        IntPtr window,
        IntPtr child,
        UIntPtr position);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_SetDimensions(
        IntPtr window,
        UIntPtr width,
        UIntPtr height);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_GetDimensions(
        IntPtr window,
        out UIntPtr out_width,
        out UIntPtr out_height);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_GetAutoPositioning(IntPtr window, out GridAutoPositioning out_mode);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_SetAutoPositioning(IntPtr window, GridAutoPositioning mode);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_GetChildAt(
        IntPtr window,
        UIntPtr x,
        UIntPtr y,
        out IntPtr out_child);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeResult CEGUI_C_GridLayout_AddChildAt(
        IntPtr window,
        IntPtr child,
        UIntPtr x,
        UIntPtr y);
    // END GENERATED WIDGET INTEROP
}
