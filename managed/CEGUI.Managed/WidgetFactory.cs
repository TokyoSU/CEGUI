using System;

namespace CEGUI;

internal static class WidgetFactory
{
    internal static Window Create(IntPtr handle)
    {
        if (handle == IntPtr.Zero)
            throw new ArgumentException("Window handle must not be null.", nameof(handle));

        Interop.Check(NativeMethods.CEGUI_C_Window_GetWidgetKind(handle, out WidgetKind kind));
        return kind switch
        {
            WidgetKind.DefaultWindow => new DefaultWindow(handle),
            WidgetKind.ButtonBase => new ButtonBase(handle),
            WidgetKind.PushButton => new PushButton(handle),
            WidgetKind.ToggleButton => new ToggleButton(handle),
            WidgetKind.RadioButton => new RadioButton(handle),
            WidgetKind.ProgressBar => new ProgressBar(handle),
            WidgetKind.Editbox => new Editbox(handle),
            WidgetKind.MultiLineEditbox => new MultiLineEditbox(handle),
            WidgetKind.FrameWindow => new FrameWindow(handle),
            WidgetKind.Titlebar => new Titlebar(handle),
            WidgetKind.Tooltip => new Tooltip(handle),
            WidgetKind.DragContainer => new DragContainer(handle),
            WidgetKind.ClippedContainer => new ClippedContainer(handle),
            WidgetKind.Scrollbar => new Scrollbar(handle),
            WidgetKind.Slider => new Slider(handle),
            WidgetKind.Thumb => new Thumb(handle),
            WidgetKind.Spinner => new Spinner(handle),
            WidgetKind.Combobox => new Combobox(handle),
            WidgetKind.ComboDropList => new ComboDropList(handle),
            WidgetKind.Listbox => new Listbox(handle),
            WidgetKind.MultiColumnList => new MultiColumnList(handle),
            WidgetKind.ListHeader => new ListHeader(handle),
            WidgetKind.ListHeaderSegment => new ListHeaderSegment(handle),
            WidgetKind.Tree => new Tree(handle),
            WidgetKind.TabControl => new TabControl(handle),
            WidgetKind.TabButton => new TabButton(handle),
            WidgetKind.MenuBase => new MenuBase(handle),
            WidgetKind.Menubar => new Menubar(handle),
            WidgetKind.PopupMenu => new PopupMenu(handle),
            WidgetKind.MenuItem => new MenuItem(handle),
            WidgetKind.ScrollablePane => new ScrollablePane(handle),
            WidgetKind.ScrolledContainer => new ScrolledContainer(handle),
            WidgetKind.LayoutContainer => new LayoutContainer(handle),
            WidgetKind.LayoutCell => new LayoutCell(handle),
            WidgetKind.SequentialLayoutContainer => new SequentialLayoutContainer(handle),
            WidgetKind.HorizontalLayoutContainer => new HorizontalLayoutContainer(handle),
            WidgetKind.VerticalLayoutContainer => new VerticalLayoutContainer(handle),
            WidgetKind.GridLayoutContainer => new GridLayoutContainer(handle),
            WidgetKind.ItemEntry => new ItemEntry(handle),
            WidgetKind.ItemListBase => new ItemListBase(handle),
            WidgetKind.ScrolledItemListBase => new ScrolledItemListBase(handle),
            WidgetKind.ItemListbox => new ItemListbox(handle),
            WidgetKind.GroupBox => new GroupBox(handle),
            WidgetKind.RoundImage => new RoundImage(handle),
            WidgetKind.SlotImageItem => new SlotImageItem(handle),
            _ => new Window(handle)
        };
    }
}
