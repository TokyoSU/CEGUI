using System;
using System.Collections.Generic;
using System.Globalization;

namespace CEGUI;

public class Window
{
    internal Window(IntPtr handle)
    {
        if (handle == IntPtr.Zero) throw new ArgumentException("Window handle must not be null.", nameof(handle));
        Handle = handle;
    }

    internal IntPtr Handle { get; }

    internal static Window FromHandle(IntPtr handle) => WidgetFactory.Create(handle);
    internal static Window? FromNullableHandle(IntPtr handle) => handle == IntPtr.Zero ? null : WidgetFactory.Create(handle);

    public WidgetKind WidgetKind
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_Window_GetWidgetKind(Handle, out WidgetKind kind));
            return kind;
        }
    }

    public string Name { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetName(Handle, out IntPtr p)); return Interop.PtrToUtf8(p); } }
    public string Type { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetType(Handle, out IntPtr p)); return Interop.PtrToUtf8(p); } }
    public string Text { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetText(Handle, out IntPtr p)); return Interop.PtrToUtf8(p); } set { if (value is null) throw new ArgumentNullException(nameof(value)); Interop.Check(NativeMethods.CEGUI_C_Window_SetText(Handle, value)); } }
    public bool Visible { get { Interop.Check(NativeMethods.CEGUI_C_Window_IsVisible(Handle, out int v)); return v != 0; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetVisible(Handle, value ? 1 : 0)); }
    public bool Enabled { get { Interop.Check(NativeMethods.CEGUI_C_Window_IsEnabled(Handle, out int v)); return v != 0; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetEnabled(Handle, value ? 1 : 0)); }
    public bool Active { get { Interop.Check(NativeMethods.CEGUI_C_Window_IsActive(Handle, out int v)); return v != 0; } }
    public float Alpha { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetAlpha(Handle, out float v)); return v; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetAlpha(Handle, value)); }
    public uint Id { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetID(Handle, out uint v)); return v; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetID(Handle, value)); }
    public UVector2 Position { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetPosition(Handle, out UVector2 v)); return v; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetPosition(Handle, value)); }
    public USize Size { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetSize(Handle, out USize v)); return v; } set => Interop.Check(NativeMethods.CEGUI_C_Window_SetSize(Handle, value)); }
    public Window? Parent { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetParent(Handle, out IntPtr p)); return FromNullableHandle(p); } }
    public long ChildCount { get { Interop.Check(NativeMethods.CEGUI_C_Window_GetChildCount(Handle, out UIntPtr n)); return Interop.FromUIntPtr(n); } }

    public void SetArea(UVector2 position, USize size) => Interop.Check(NativeMethods.CEGUI_C_Window_SetArea(Handle, position, size));
    public void SetFont(string fontName) { if (fontName is null) throw new ArgumentNullException(nameof(fontName)); Interop.Check(NativeMethods.CEGUI_C_Window_SetFont(Handle, fontName)); }
    public void SetProperty(string property, string value) { if (property is null) throw new ArgumentNullException(nameof(property)); if (value is null) throw new ArgumentNullException(nameof(value)); Interop.Check(NativeMethods.CEGUI_C_Window_SetProperty(Handle, property, value)); }
    public string GetProperty(string property) { if (property is null) throw new ArgumentNullException(nameof(property)); Interop.Check(NativeMethods.CEGUI_C_Window_GetProperty(Handle, property, out IntPtr p)); return Interop.PtrToUtf8(p); }
    public bool HasProperty(string property) { if (property is null) throw new ArgumentNullException(nameof(property)); Interop.Check(NativeMethods.CEGUI_C_Window_IsPropertyPresent(Handle, property, out int v)); return v != 0; }

    public bool GetBooleanProperty(string property) => ParseBoolean(GetProperty(property), property);
    public void SetBooleanProperty(string property, bool value) => SetProperty(property, value ? "True" : "False");
    public int GetInt32Property(string property) => int.Parse(GetProperty(property), NumberStyles.Integer, CultureInfo.InvariantCulture);
    public void SetInt32Property(string property, int value) => SetProperty(property, value.ToString(CultureInfo.InvariantCulture));
    public uint GetUInt32Property(string property) => uint.Parse(GetProperty(property), NumberStyles.Integer, CultureInfo.InvariantCulture);
    public void SetUInt32Property(string property, uint value) => SetProperty(property, value.ToString(CultureInfo.InvariantCulture));
    public float GetSingleProperty(string property) => float.Parse(GetProperty(property), NumberStyles.Float, CultureInfo.InvariantCulture);
    public void SetSingleProperty(string property, float value) => SetProperty(property, value.ToString("R", CultureInfo.InvariantCulture));
    public double GetDoubleProperty(string property) => double.Parse(GetProperty(property), NumberStyles.Float, CultureInfo.InvariantCulture);
    public void SetDoubleProperty(string property, double value) => SetProperty(property, value.ToString("R", CultureInfo.InvariantCulture));

    public void AddChild(Window child) { if (child is null) throw new ArgumentNullException(nameof(child)); Interop.Check(NativeMethods.CEGUI_C_Window_AddChild(Handle, child.Handle)); }
    public void RemoveChild(Window child) { if (child is null) throw new ArgumentNullException(nameof(child)); Interop.Check(NativeMethods.CEGUI_C_Window_RemoveChild(Handle, child.Handle)); }
    public Window GetChild(string namePath) { if (namePath is null) throw new ArgumentNullException(nameof(namePath)); Interop.Check(NativeMethods.CEGUI_C_Window_GetChild(Handle, namePath, out IntPtr p)); return FromHandle(p); }
    public T GetChild<T>(string namePath) where T : Window => Cast<T>(GetChild(namePath));
    public Window? FindChildRecursive(string name) { if (name is null) throw new ArgumentNullException(nameof(name)); Interop.Check(NativeMethods.CEGUI_C_Window_GetChildRecursive(Handle, name, out IntPtr p)); return FromNullableHandle(p); }
    public T? FindChildRecursive<T>(string name) where T : Window => FindChildRecursive(name) as T;
    public Window GetChildAt(long index) { Interop.Check(NativeMethods.CEGUI_C_Window_GetChildAtIndex(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr p)); return FromHandle(p); }
    public IEnumerable<Window> Children { get { long count = ChildCount; for (long i = 0; i < count; ++i) yield return GetChildAt(i); } }
    public void Activate() => Interop.Check(NativeMethods.CEGUI_C_Window_Activate(Handle));
    public void Deactivate() => Interop.Check(NativeMethods.CEGUI_C_Window_Deactivate(Handle));
    public EventConnection SubscribeEvent(string eventName, Func<CeguiEvent, bool> handler) => EventConnection.Create(this, eventName, handler);

    public T As<T>() where T : Window => Cast<T>(this);

    internal Window? GetComponent(WidgetComponent component)
    {
        Interop.Check(NativeMethods.CEGUI_C_Widget_GetComponent(Handle, component, out IntPtr handle));
        return FromNullableHandle(handle);
    }

    internal T? GetComponent<T>(WidgetComponent component) where T : Window
    {
        Window? value = GetComponent(component);
        if (value is null) return null;
        return Cast<T>(value);
    }

    internal void ExecuteAction(WidgetAction action) =>
        Interop.Check(NativeMethods.CEGUI_C_Widget_ExecuteAction(Handle, action));

    private static bool ParseBoolean(string value, string property)
    {
        if (bool.TryParse(value, out bool result)) return result;
        if (value == "1") return true;
        if (value == "0") return false;
        throw new FormatException($"CEGUI property '{property}' value '{value}' is not a Boolean.");
    }

    private static T Cast<T>(Window value) where T : Window
    {
        if (value is T typed) return typed;
        throw new InvalidCastException($"CEGUI window '{value.Name}' is {value.GetType().Name}, not {typeof(T).Name}.");
    }
}
