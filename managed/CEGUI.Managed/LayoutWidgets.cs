using System;

namespace CEGUI;

public class LayoutContainer : Window
{
    internal LayoutContainer(IntPtr handle) : base(handle) { }

    public void MarkNeedsLayout() => ExecuteAction(WidgetAction.LayoutMarkDirty);
    public void Layout() => ExecuteAction(WidgetAction.LayoutNow);
    public void LayoutIfNeeded() => ExecuteAction(WidgetAction.LayoutIfNeeded);
}

public class SequentialLayoutContainer : LayoutContainer
{
    internal SequentialLayoutContainer(IntPtr handle) : base(handle) { }

    public long GetPosition(Window child)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        Interop.Check(NativeMethods.CEGUI_C_SequentialLayout_GetPosition(Handle, child.Handle, out UIntPtr position));
        return Interop.FromUIntPtr(position);
    }

    public Window GetChildAtPosition(long position)
    {
        Interop.Check(NativeMethods.CEGUI_C_SequentialLayout_GetChildAt(
            Handle, Interop.ToUIntPtr(position, nameof(position)), out IntPtr child));
        return FromHandle(child);
    }

    public void MoveChildTo(Window child, long position)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        Interop.Check(NativeMethods.CEGUI_C_SequentialLayout_MoveChildTo(
            Handle, child.Handle, Interop.ToUIntPtr(position, nameof(position))));
    }

    public void AddChildAt(Window child, long position)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        Interop.Check(NativeMethods.CEGUI_C_SequentialLayout_AddChildAt(
            Handle, child.Handle, Interop.ToUIntPtr(position, nameof(position))));
    }
}

public sealed class HorizontalLayoutContainer : SequentialLayoutContainer
{
    internal HorizontalLayoutContainer(IntPtr handle) : base(handle) { }
}

public sealed class VerticalLayoutContainer : SequentialLayoutContainer
{
    internal VerticalLayoutContainer(IntPtr handle) : base(handle) { }
}

public sealed class GridLayoutContainer : LayoutContainer
{
    internal GridLayoutContainer(IntPtr handle) : base(handle) { }

    public (long Width, long Height) Dimensions
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_GridLayout_GetDimensions(Handle, out UIntPtr width, out UIntPtr height));
            return (Interop.FromUIntPtr(width), Interop.FromUIntPtr(height));
        }
        set => SetDimensions(value.Width, value.Height);
    }

    public GridAutoPositioning AutoPositioning
    {
        get { Interop.Check(NativeMethods.CEGUI_C_GridLayout_GetAutoPositioning(Handle, out GridAutoPositioning value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_GridLayout_SetAutoPositioning(Handle, value));
    }

    public void SetDimensions(long width, long height) =>
        Interop.Check(NativeMethods.CEGUI_C_GridLayout_SetDimensions(
            Handle, Interop.ToUIntPtr(width, nameof(width)), Interop.ToUIntPtr(height, nameof(height))));

    public Window? GetChildAt(long x, long y)
    {
        Interop.Check(NativeMethods.CEGUI_C_GridLayout_GetChildAt(
            Handle, Interop.ToUIntPtr(x, nameof(x)), Interop.ToUIntPtr(y, nameof(y)), out IntPtr child));
        return FromNullableHandle(child);
    }

    public void AddChildAt(Window child, long x, long y)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        Interop.Check(NativeMethods.CEGUI_C_GridLayout_AddChildAt(
            Handle, child.Handle, Interop.ToUIntPtr(x, nameof(x)), Interop.ToUIntPtr(y, nameof(y))));
    }
}
