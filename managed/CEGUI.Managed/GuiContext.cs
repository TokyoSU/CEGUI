using System;

namespace CEGUI;

public sealed class GuiContext
{
    internal GuiContext(IntPtr handle)
    {
        if (handle == IntPtr.Zero)
            throw new ArgumentException("GUIContext handle must not be null.", nameof(handle));
        Handle = handle;
    }

    internal IntPtr Handle { get; }

    public Window? RootWindow
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_GUIContext_GetRootWindow(Handle, out IntPtr window));
            return Window.FromNullableHandle(window);
        }
        set => Interop.Check(NativeMethods.CEGUI_C_GUIContext_SetRootWindow(Handle, value?.Handle ?? IntPtr.Zero));
    }

    public void Draw() => Interop.Check(NativeMethods.CEGUI_C_GUIContext_Draw(Handle));

    public void SetDefaultFont(string fontName)
    {
        if (fontName is null) throw new ArgumentNullException(nameof(fontName));
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_SetDefaultFont(Handle, fontName));
    }

    public bool InjectMousePosition(float x, float y)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectMousePosition(Handle, x, y, out int handled));
        return handled != 0;
    }

    public bool InjectMouseMove(float deltaX, float deltaY)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectMouseMove(Handle, deltaX, deltaY, out int handled));
        return handled != 0;
    }

    public bool InjectMouseWheelChange(float delta)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectMouseWheelChange(Handle, delta, out int handled));
        return handled != 0;
    }

    public bool InjectMouseButtonDown(MouseButton button)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectMouseButtonDown(Handle, button, out int handled));
        return handled != 0;
    }

    public bool InjectMouseButtonUp(MouseButton button)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectMouseButtonUp(Handle, button, out int handled));
        return handled != 0;
    }

    public bool InjectKeyDown(KeyScan key)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectKeyDown(Handle, key, out int handled));
        return handled != 0;
    }

    public bool InjectKeyUp(KeyScan key)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectKeyUp(Handle, key, out int handled));
        return handled != 0;
    }

    public bool InjectChar(uint unicodeCodepoint)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectChar(Handle, unicodeCodepoint, out int handled));
        return handled != 0;
    }

    public bool InjectTimePulse(float elapsedSeconds)
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectTimePulse(Handle, elapsedSeconds, out int handled));
        return handled != 0;
    }

    public bool InjectCopyRequest()
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectCopyRequest(Handle, out int handled));
        return handled != 0;
    }

    public bool InjectCutRequest()
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectCutRequest(Handle, out int handled));
        return handled != 0;
    }

    public bool InjectPasteRequest()
    {
        Interop.Check(NativeMethods.CEGUI_C_GUIContext_InjectPasteRequest(Handle, out int handled));
        return handled != 0;
    }
}
