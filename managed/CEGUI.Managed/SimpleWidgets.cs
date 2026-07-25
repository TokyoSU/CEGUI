using System;

namespace CEGUI;

public class DefaultWindow : Window { internal DefaultWindow(IntPtr handle) : base(handle) { } }

public class ButtonBase : Window
{
    internal ButtonBase(IntPtr handle) : base(handle) { }
}

public class PushButton : ButtonBase
{
    internal PushButton(IntPtr handle) : base(handle) { }
}

public class ToggleButton : ButtonBase
{
    internal ToggleButton(IntPtr handle) : base(handle) { }

    public bool Selected
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ToggleButton_IsSelected(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_ToggleButton_SetSelected(Handle, value ? 1 : 0));
    }
}

public sealed class RadioButton : ToggleButton
{
    internal RadioButton(IntPtr handle) : base(handle) { }

    public uint GroupId
    {
        get => GetUInt32Property("GroupID");
        set => SetUInt32Property("GroupID", value);
    }
}

public sealed class ProgressBar : Window
{
    internal ProgressBar(IntPtr handle) : base(handle) { }

    public float Progress
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ProgressBar_GetProgress(Handle, out float value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_ProgressBar_SetProgress(Handle, value));
    }

    public float StepSize
    {
        get => GetSingleProperty("StepSize");
        set => SetSingleProperty("StepSize", value);
    }

    public void Step() => ExecuteAction(WidgetAction.ProgressStep);
}

public sealed class Editbox : Window
{
    internal Editbox(IntPtr handle) : base(handle) { }

    public bool ReadOnly
    {
        get { Interop.Check(NativeMethods.CEGUI_C_Editbox_IsReadOnly(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_Editbox_SetReadOnly(Handle, value ? 1 : 0));
    }

    public long MaxTextLength
    {
        get { Interop.Check(NativeMethods.CEGUI_C_Editbox_GetMaxTextLength(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); }
        set => Interop.Check(NativeMethods.CEGUI_C_Editbox_SetMaxTextLength(Handle, Interop.ToUIntPtr(value, nameof(value))));
    }

    public (long Start, long End) Selection
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_Editbox_GetSelection(Handle, out UIntPtr start, out UIntPtr end));
            return (Interop.FromUIntPtr(start), Interop.FromUIntPtr(end));
        }
        set => SetSelection(value.Start, value.End);
    }

    public bool MaskText
    {
        get => GetBooleanProperty("MaskText");
        set => SetBooleanProperty("MaskText", value);
    }

    public void SetSelection(long start, long end) =>
        Interop.Check(NativeMethods.CEGUI_C_Editbox_SetSelection(
            Handle, Interop.ToUIntPtr(start, nameof(start)), Interop.ToUIntPtr(end, nameof(end))));
}

public sealed class MultiLineEditbox : Window
{
    internal MultiLineEditbox(IntPtr handle) : base(handle) { }

    public bool ReadOnly
    {
        get => GetBooleanProperty("ReadOnly");
        set => SetBooleanProperty("ReadOnly", value);
    }

    public bool WordWrap
    {
        get => GetBooleanProperty("WordWrap");
        set => SetBooleanProperty("WordWrap", value);
    }

    public Scrollbar VerticalScrollbar => GetComponent<Scrollbar>(WidgetComponent.MultiLineVerticalScrollbar)!;
    public Scrollbar HorizontalScrollbar => GetComponent<Scrollbar>(WidgetComponent.MultiLineHorizontalScrollbar)!;
}

public sealed class FrameWindow : Window
{
    internal FrameWindow(IntPtr handle) : base(handle) { }

    public Titlebar Titlebar => GetComponent<Titlebar>(WidgetComponent.FrameTitlebar)!;
    public PushButton CloseButton => GetComponent<PushButton>(WidgetComponent.FrameCloseButton)!;

    public bool SizingEnabled
    {
        get => GetBooleanProperty("SizingEnabled");
        set => SetBooleanProperty("SizingEnabled", value);
    }

    public bool TitlebarEnabled
    {
        get => GetBooleanProperty("TitlebarEnabled");
        set => SetBooleanProperty("TitlebarEnabled", value);
    }

    public bool CloseButtonEnabled
    {
        get => GetBooleanProperty("CloseButtonEnabled");
        set => SetBooleanProperty("CloseButtonEnabled", value);
    }

    public bool RollUpEnabled
    {
        get => GetBooleanProperty("RollUpEnabled");
        set => SetBooleanProperty("RollUpEnabled", value);
    }

    public void ToggleRollup() => ExecuteAction(WidgetAction.FrameToggleRollup);
}

public sealed class Titlebar : Window
{
    internal Titlebar(IntPtr handle) : base(handle) { }
}

public sealed class Tooltip : Window
{
    internal Tooltip(IntPtr handle) : base(handle) { }

    public Window? TargetWindow => GetComponent(WidgetComponent.TooltipTarget);
    public void ResetTimer() => ExecuteAction(WidgetAction.TooltipResetTimer);
    public void PositionSelf() => ExecuteAction(WidgetAction.TooltipPositionSelf);
    public void SizeSelf() => ExecuteAction(WidgetAction.TooltipSizeSelf);
}

public sealed class DragContainer : Window
{
    internal DragContainer(IntPtr handle) : base(handle) { }

    public Window? CurrentDropTarget => GetComponent(WidgetComponent.DragDropTarget);

    public bool DraggingEnabled
    {
        get => GetBooleanProperty("DraggingEnabled");
        set => SetBooleanProperty("DraggingEnabled", value);
    }
}

public class ClippedContainer : Window
{
    internal ClippedContainer(IntPtr handle) : base(handle) { }
}

public sealed class Scrollbar : Window
{
    internal Scrollbar(IntPtr handle) : base(handle) { }

    public float ScrollPosition
    {
        get { Interop.Check(NativeMethods.CEGUI_C_Scrollbar_GetScrollPosition(Handle, out float value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_Scrollbar_SetScrollPosition(Handle, value));
    }

    public PushButton IncreaseButton => GetComponent<PushButton>(WidgetComponent.ScrollbarIncreaseButton)!;
    public PushButton DecreaseButton => GetComponent<PushButton>(WidgetComponent.ScrollbarDecreaseButton)!;
    public Thumb Thumb => GetComponent<Thumb>(WidgetComponent.ScrollbarThumb)!;

    public float DocumentSize
    {
        get => GetSingleProperty("DocumentSize");
        set => SetSingleProperty("DocumentSize", value);
    }

    public float PageSize
    {
        get => GetSingleProperty("PageSize");
        set => SetSingleProperty("PageSize", value);
    }

    public float StepSize
    {
        get => GetSingleProperty("StepSize");
        set => SetSingleProperty("StepSize", value);
    }
}

public sealed class Slider : Window
{
    internal Slider(IntPtr handle) : base(handle) { }

    public float Value
    {
        get { Interop.Check(NativeMethods.CEGUI_C_Slider_GetCurrentValue(Handle, out float value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_Slider_SetCurrentValue(Handle, value));
    }

    public float Maximum
    {
        get { Interop.Check(NativeMethods.CEGUI_C_Slider_GetMaxValue(Handle, out float value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_Slider_SetMaxValue(Handle, value));
    }

    public Thumb Thumb => GetComponent<Thumb>(WidgetComponent.SliderThumb)!;
}

public sealed class Thumb : PushButton
{
    internal Thumb(IntPtr handle) : base(handle) { }
}

public sealed class Spinner : Window
{
    internal Spinner(IntPtr handle) : base(handle) { }

    public Editbox Editbox => GetComponent<Editbox>(WidgetComponent.SpinnerEditbox)!;
    public PushButton IncreaseButton => GetComponent<PushButton>(WidgetComponent.SpinnerIncreaseButton)!;
    public PushButton DecreaseButton => GetComponent<PushButton>(WidgetComponent.SpinnerDecreaseButton)!;

    public float Value
    {
        get => GetSingleProperty("CurrentValue");
        set => SetSingleProperty("CurrentValue", value);
    }

    public float Minimum
    {
        get => GetSingleProperty("MinimumValue");
        set => SetSingleProperty("MinimumValue", value);
    }

    public float Maximum
    {
        get => GetSingleProperty("MaximumValue");
        set => SetSingleProperty("MaximumValue", value);
    }

    public float StepSize
    {
        get => GetSingleProperty("StepSize");
        set => SetSingleProperty("StepSize", value);
    }
}

public sealed class ListHeader : Window
{
    internal ListHeader(IntPtr handle) : base(handle) { }

    public uint ColumnCount { get { Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetColumnCount(Handle, out uint value)); return value; } }

    public uint SortColumn
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetSortColumn(Handle, out uint value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListHeader_SetSortColumn(Handle, value));
    }

    public ListSortDirection SortDirection
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetSortDirection(Handle, out ListSortDirection value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListHeader_SetSortDirection(Handle, value));
    }

    public ListHeaderSegment GetSegmentAt(uint column)
    {
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetSegmentAt(Handle, column, out IntPtr segment));
        return FromHandle(segment).As<ListHeaderSegment>();
    }

    public ListHeaderSegment GetSegmentById(uint id)
    {
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetSegmentByID(Handle, id, out IntPtr segment));
        return FromHandle(segment).As<ListHeaderSegment>();
    }

    public uint GetColumnFromId(uint id)
    {
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetColumnFromID(Handle, id, out uint column));
        return column;
    }

    public UDim GetColumnWidth(uint column)
    {
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_GetColumnWidth(Handle, column, out UDim width));
        return width;
    }

    public void AddColumn(string text, uint id, UDim width)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_AddColumn(Handle, text, id, width));
    }

    public void InsertColumn(string text, uint id, UDim width, uint position)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_ListHeader_InsertColumn(Handle, text, id, width, position));
    }

    public void RemoveColumn(uint column) => Interop.Check(NativeMethods.CEGUI_C_ListHeader_RemoveColumn(Handle, column));
    public void MoveColumn(uint column, uint position) => Interop.Check(NativeMethods.CEGUI_C_ListHeader_MoveColumn(Handle, column, position));
}

public sealed class ListHeaderSegment : Window
{
    internal ListHeaderSegment(IntPtr handle) : base(handle) { }

    public bool Sizable { get => GetBooleanProperty("Sizable"); set => SetBooleanProperty("Sizable", value); }
    public bool Clickable { get => GetBooleanProperty("Clickable"); set => SetBooleanProperty("Clickable", value); }
    public bool Dragable { get => GetBooleanProperty("Dragable"); set => SetBooleanProperty("Dragable", value); }
}

public sealed class TabButton : ButtonBase
{
    internal TabButton(IntPtr handle) : base(handle) { }
}

public sealed class ScrollablePane : Window
{
    internal ScrollablePane(IntPtr handle) : base(handle) { }

    public ScrolledContainer ContentPane => GetComponent<ScrolledContainer>(WidgetComponent.ScrollableContentPane)!;
    public Scrollbar VerticalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ScrollableVerticalScrollbar)!;
    public Scrollbar HorizontalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ScrollableHorizontalScrollbar)!;
}

public sealed class ScrolledContainer : ClippedContainer
{
    internal ScrolledContainer(IntPtr handle) : base(handle) { }
}

public sealed class LayoutCell : Window
{
    internal LayoutCell(IntPtr handle) : base(handle) { }
}

public sealed class GroupBox : Window
{
    internal GroupBox(IntPtr handle) : base(handle) { }
    public Window ContentPane => GetComponent(WidgetComponent.GroupBoxContentPane)!;
}

public sealed class RoundImage : Window
{
    internal RoundImage(IntPtr handle) : base(handle) { }
}

public sealed class SlotImageItem : Window
{
    internal SlotImageItem(IntPtr handle) : base(handle) { }
}
