using System;

namespace CEGUI;

public static class Widgets
{
    private static IntPtr H(Window w) => w?.Handle ?? throw new ArgumentNullException(nameof(w));
    public static void SetProgress(Window w, float value) => Interop.Check(NativeMethods.CEGUI_C_ProgressBar_SetProgress(H(w), value));
    public static float GetProgress(Window w) { Interop.Check(NativeMethods.CEGUI_C_ProgressBar_GetProgress(H(w), out float v)); return v; }
    public static void SetSelected(Window w, bool value) => Interop.Check(NativeMethods.CEGUI_C_ToggleButton_SetSelected(H(w), value ? 1 : 0));
    public static bool IsSelected(Window w) { Interop.Check(NativeMethods.CEGUI_C_ToggleButton_IsSelected(H(w), out int v)); return v != 0; }
    public static void SetReadOnly(Window w, bool value) => Interop.Check(NativeMethods.CEGUI_C_Editbox_SetReadOnly(H(w), value ? 1 : 0));
    public static bool IsReadOnly(Window w) { Interop.Check(NativeMethods.CEGUI_C_Editbox_IsReadOnly(H(w), out int v)); return v != 0; }
    public static void SetMaxTextLength(Window w, long value) => Interop.Check(NativeMethods.CEGUI_C_Editbox_SetMaxTextLength(H(w), Interop.ToUIntPtr(value, nameof(value))));
    public static long GetMaxTextLength(Window w) { Interop.Check(NativeMethods.CEGUI_C_Editbox_GetMaxTextLength(H(w), out UIntPtr v)); return Interop.FromUIntPtr(v); }
    public static void SetSelection(Window w, long start, long end) => Interop.Check(NativeMethods.CEGUI_C_Editbox_SetSelection(H(w), Interop.ToUIntPtr(start, nameof(start)), Interop.ToUIntPtr(end, nameof(end))));
    public static (long Start, long End) GetSelection(Window w) { Interop.Check(NativeMethods.CEGUI_C_Editbox_GetSelection(H(w), out UIntPtr s, out UIntPtr e)); return (Interop.FromUIntPtr(s), Interop.FromUIntPtr(e)); }
    public static void SetSliderValue(Window w, float value) => Interop.Check(NativeMethods.CEGUI_C_Slider_SetCurrentValue(H(w), value));
    public static float GetSliderValue(Window w) { Interop.Check(NativeMethods.CEGUI_C_Slider_GetCurrentValue(H(w), out float v)); return v; }
    public static void SetSliderMaximum(Window w, float value) => Interop.Check(NativeMethods.CEGUI_C_Slider_SetMaxValue(H(w), value));
    public static float GetSliderMaximum(Window w) { Interop.Check(NativeMethods.CEGUI_C_Slider_GetMaxValue(H(w), out float v)); return v; }
    public static void SetScrollPosition(Window w, float value) => Interop.Check(NativeMethods.CEGUI_C_Scrollbar_SetScrollPosition(H(w), value));
    public static float GetScrollPosition(Window w) { Interop.Check(NativeMethods.CEGUI_C_Scrollbar_GetScrollPosition(H(w), out float v)); return v; }
}
