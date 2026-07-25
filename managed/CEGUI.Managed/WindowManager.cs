using System;

namespace CEGUI;

public static class WindowManager
{
    public static Window CreateWindow(string type, string? name = null)
    {
        if (type is null) throw new ArgumentNullException(nameof(type));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_CreateWindow(type, name, out IntPtr window));
        return Window.FromHandle(window);
    }

    public static T CreateWindow<T>(string type, string? name = null) where T : Window =>
        CreateWindow(type, name).As<T>();

    public static Window LoadLayoutFromFile(string filename, string? resourceGroup = null)
    {
        if (filename is null) throw new ArgumentNullException(nameof(filename));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_LoadLayoutFromFile(filename, resourceGroup, out IntPtr window));
        return Window.FromHandle(window);
    }

    public static T LoadLayoutFromFile<T>(string filename, string? resourceGroup = null) where T : Window =>
        LoadLayoutFromFile(filename, resourceGroup).As<T>();

    public static Window GetWindow(string name)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_GetWindow(name, out IntPtr window));
        return Window.FromHandle(window);
    }

    public static T GetWindow<T>(string name) where T : Window => GetWindow(name).As<T>();

    public static bool IsWindowPresent(string name)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_IsWindowPresent(name, out int present));
        return present != 0;
    }

    public static bool IsAlive(Window window)
    {
        if (window is null) throw new ArgumentNullException(nameof(window));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_IsAlive(window.Handle, out int alive));
        return alive != 0;
    }

    public static void DestroyWindow(Window window)
    {
        if (window is null) throw new ArgumentNullException(nameof(window));
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_DestroyWindow(window.Handle));
    }

    public static void DestroyAllWindows() =>
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_DestroyAllWindows());

    public static void CleanDeadPool() =>
        Interop.Check(NativeMethods.CEGUI_C_WindowManager_CleanDeadPool());
}
