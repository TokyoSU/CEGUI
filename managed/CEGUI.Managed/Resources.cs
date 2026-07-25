using System;

namespace CEGUI;

public static class Resources
{
    public static void SetDefaultGroups(
        string schemes = "schemes",
        string imagesets = "imagesets",
        string fonts = "fonts",
        string layouts = "layouts",
        string lookNFeels = "looknfeels")
    {
        Interop.Check(NativeMethods.CEGUI_C_SetDefaultResourceGroups(
            schemes, imagesets, fonts, layouts, lookNFeels));
    }

    public static void LoadScheme(string filename, string? resourceGroup = null)
    {
        if (filename is null) throw new ArgumentNullException(nameof(filename));
        Interop.Check(NativeMethods.CEGUI_C_Scheme_LoadFromFile(filename, resourceGroup));
    }

    public static bool IsSchemeDefined(string name)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        Interop.Check(NativeMethods.CEGUI_C_Scheme_IsDefined(name, out int defined));
        return defined != 0;
    }

    public static void LoadFont(string filename, string? resourceGroup = null)
    {
        if (filename is null) throw new ArgumentNullException(nameof(filename));
        Interop.Check(NativeMethods.CEGUI_C_Font_LoadFromFile(filename, resourceGroup));
    }

    public static bool IsFontDefined(string name)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        Interop.Check(NativeMethods.CEGUI_C_Font_IsDefined(name, out int defined));
        return defined != 0;
    }

    public static void AddImageFromFile(string name, string filename, string? resourceGroup = null)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        if (filename is null) throw new ArgumentNullException(nameof(filename));
        Interop.Check(NativeMethods.CEGUI_C_Image_AddFromFile(name, filename, resourceGroup));
    }
}
