using System;
using System.Collections.Generic;

namespace CEGUI;

public class Listbox : Window
{
    internal Listbox(IntPtr handle) : base(handle) { }

    public long ItemCount { get { Interop.Check(NativeMethods.CEGUI_C_Listbox_GetItemCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }
    public long SelectedCount { get { Interop.Check(NativeMethods.CEGUI_C_Listbox_GetSelectedCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }
    public Scrollbar VerticalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ListboxVerticalScrollbar)!;
    public Scrollbar HorizontalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ListboxHorizontalScrollbar)!;

    public IEnumerable<ListboxItem> Items
    {
        get
        {
            long count = ItemCount;
            for (long index = 0; index < count; ++index)
                yield return GetItemAt(index);
        }
    }

    public IEnumerable<ListboxItem> SelectedItems
    {
        get
        {
            ListboxItem? item = FirstSelectedItem;
            while (item is not null)
            {
                yield return item;
                item = GetNextSelectedItem(item);
            }
        }
    }

    public ListboxItem? FirstSelectedItem
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_Listbox_GetFirstSelected(Handle, out IntPtr item));
            return ListboxItem.FromBorrowed(item);
        }
    }

    public ListboxItem GetItemAt(long index)
    {
        Interop.Check(NativeMethods.CEGUI_C_Listbox_GetItemAt(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr item));
        return ListboxItem.FromBorrowed(item) ?? throw new CeguiException(NativeResult.UnknownException, "CEGUI returned a null ListboxItem.");
    }

    public ListboxItem? GetNextSelectedItem(ListboxItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Listbox_GetNextSelected(Handle, item.Handle, out IntPtr next));
        return ListboxItem.FromBorrowed(next);
    }

    public void AddItem(ListboxItem item, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, IntPtr.Zero, false, transferOwnership);
    }

    public void InsertItem(ListboxItem item, ListboxItem? beforeItem, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, beforeItem?.Handle ?? IntPtr.Zero, true, transferOwnership);
    }

    private void AddItemCore(ListboxItem item, IntPtr beforeItem, bool insert, bool transferOwnership)
    {
        item.PrepareForCollection(transferOwnership);
        try
        {
            NativeResult result = insert
                ? NativeMethods.CEGUI_C_Listbox_InsertItem(Handle, item.Handle, beforeItem)
                : NativeMethods.CEGUI_C_Listbox_AddItem(Handle, item.Handle);
            Interop.Check(result);
            item.CompleteOwnershipTransfer(transferOwnership);
        }
        catch
        {
            if (transferOwnership) item.PrepareForCollection(false);
            throw;
        }
    }

    public void RemoveItem(ListboxItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        bool deleted = item.NativeAutoDelete;
        Interop.Check(NativeMethods.CEGUI_C_Listbox_RemoveItem(Handle, item.Handle));
        if (deleted) item.InvalidateBorrowedHandle();
    }

    public void SetItemSelected(ListboxItem item, bool selected)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Listbox_SetItemSelected(Handle, item.Handle, selected ? 1 : 0));
    }

    public void EnsureItemVisible(ListboxItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Listbox_EnsureItemVisible(Handle, item.Handle));
    }

    public void ResetList() => ExecuteAction(WidgetAction.ListboxReset);
    public void ClearSelections() => ExecuteAction(WidgetAction.ListboxClearSelections);
}

public sealed class ComboDropList : Listbox
{
    internal ComboDropList(IntPtr handle) : base(handle) { }
}

public sealed class Combobox : Window
{
    internal Combobox(IntPtr handle) : base(handle) { }

    public long ItemCount { get { Interop.Check(NativeMethods.CEGUI_C_Combobox_GetItemCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }
    public Editbox Editbox => GetComponent<Editbox>(WidgetComponent.ComboboxEditbox)!;
    public PushButton DropButton => GetComponent<PushButton>(WidgetComponent.ComboboxButton)!;
    public ComboDropList DropList => GetComponent<ComboDropList>(WidgetComponent.ComboboxDropList)!;

    public ListboxItem? SelectedItem
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_Combobox_GetSelectedItem(Handle, out IntPtr item));
            return ListboxItem.FromBorrowed(item);
        }
    }

    public ListboxItem GetItemAt(long index)
    {
        Interop.Check(NativeMethods.CEGUI_C_Combobox_GetItemAt(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr item));
        return ListboxItem.FromBorrowed(item) ?? throw new CeguiException(NativeResult.UnknownException, "CEGUI returned a null ListboxItem.");
    }

    public void AddItem(ListboxItem item, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, IntPtr.Zero, false, transferOwnership);
    }

    public void InsertItem(ListboxItem item, ListboxItem? beforeItem, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, beforeItem?.Handle ?? IntPtr.Zero, true, transferOwnership);
    }

    private void AddItemCore(ListboxItem item, IntPtr beforeItem, bool insert, bool transferOwnership)
    {
        item.PrepareForCollection(transferOwnership);
        try
        {
            NativeResult result = insert
                ? NativeMethods.CEGUI_C_Combobox_InsertItem(Handle, item.Handle, beforeItem)
                : NativeMethods.CEGUI_C_Combobox_AddItem(Handle, item.Handle);
            Interop.Check(result);
            item.CompleteOwnershipTransfer(transferOwnership);
        }
        catch
        {
            if (transferOwnership) item.PrepareForCollection(false);
            throw;
        }
    }

    public void RemoveItem(ListboxItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        bool deleted = item.NativeAutoDelete;
        Interop.Check(NativeMethods.CEGUI_C_Combobox_RemoveItem(Handle, item.Handle));
        if (deleted) item.InvalidateBorrowedHandle();
    }

    public void SetItemSelected(ListboxItem item, bool selected)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Combobox_SetItemSelected(Handle, item.Handle, selected ? 1 : 0));
    }

    public void ShowDropList() => ExecuteAction(WidgetAction.ComboboxShowDropList);
    public void HideDropList() => ExecuteAction(WidgetAction.ComboboxHideDropList);
    public void ResetList() => ExecuteAction(WidgetAction.ComboboxResetList);
    public void ClearSelections() => ExecuteAction(WidgetAction.ComboboxClearSelections);
}

public sealed class MultiColumnList : Window
{
    internal MultiColumnList(IntPtr handle) : base(handle) { }

    public uint ColumnCount { get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetColumnCount(Handle, out uint value)); return value; } }
    public uint RowCount { get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetRowCount(Handle, out uint value)); return value; } }
    public uint SelectedCount { get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetSelectedCount(Handle, out uint value)); return value; } }
    public ListHeader Header => GetComponent<ListHeader>(WidgetComponent.MultiColumnListHeader)!;
    public Scrollbar VerticalScrollbar => GetComponent<Scrollbar>(WidgetComponent.MultiColumnVerticalScrollbar)!;
    public Scrollbar HorizontalScrollbar => GetComponent<Scrollbar>(WidgetComponent.MultiColumnHorizontalScrollbar)!;

    public MultiColumnSelectionMode SelectionMode
    {
        get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetSelectionMode(Handle, out MultiColumnSelectionMode value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_SetSelectionMode(Handle, value));
    }

    public uint SortColumn
    {
        get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetSortColumn(Handle, out uint value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_SetSortColumn(Handle, value));
    }

    public ListSortDirection SortDirection
    {
        get { Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetSortDirection(Handle, out ListSortDirection value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_SetSortDirection(Handle, value));
    }

    public IEnumerable<ListboxItem> SelectedItems
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetFirstSelected(Handle, out IntPtr value));
            ListboxItem? item = ListboxItem.FromBorrowed(value);
            while (item is not null)
            {
                yield return item;
                Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetNextSelected(Handle, item.Handle, out value));
                item = ListboxItem.FromBorrowed(value);
            }
        }
    }

    public void AddColumn(string text, uint columnId, UDim width)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_AddColumn(Handle, text, columnId, width));
    }

    public void InsertColumn(string text, uint columnId, UDim width, uint position)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_InsertColumn(Handle, text, columnId, width, position));
    }

    public void RemoveColumn(uint columnIndex) =>
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_RemoveColumn(Handle, columnIndex));

    public uint AddRow(uint rowId = 0)
    {
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_AddRow(Handle, rowId, out uint index));
        return index;
    }

    public uint InsertRow(uint rowIndex, uint rowId = 0)
    {
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_InsertRow(Handle, rowIndex, rowId, out uint actualIndex));
        return actualIndex;
    }

    public void RemoveRow(uint rowIndex) =>
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_RemoveRow(Handle, rowIndex));

    public void SetItem(ListboxItem item, uint columnId, uint rowIndex, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        item.PrepareForCollection(transferOwnership);
        try
        {
            Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_SetItem(Handle, item.Handle, columnId, rowIndex));
            item.CompleteOwnershipTransfer(transferOwnership);
        }
        catch
        {
            if (transferOwnership) item.PrepareForCollection(false);
            throw;
        }
    }

    public ListboxItem? GetItem(MultiColumnGridReference position)
    {
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_GetItem(Handle, position, out IntPtr item));
        return ListboxItem.FromBorrowed(item);
    }

    public void SetItemSelected(ListboxItem item, bool selected)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_SetItemSelected(Handle, item.Handle, selected ? 1 : 0));
    }

    public void EnsureItemVisible(ListboxItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_MultiColumnList_EnsureItemVisible(Handle, item.Handle));
    }

    public void ResetList() => ExecuteAction(WidgetAction.MultiColumnReset);
    public void ClearSelections() => ExecuteAction(WidgetAction.MultiColumnClearSelections);
}

public sealed class Tree : Window
{
    internal Tree(IntPtr handle) : base(handle) { }

    public long SelectedCount { get { Interop.Check(NativeMethods.CEGUI_C_Tree_GetSelectedCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }

    public IEnumerable<TreeItem> SelectedItems
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_Tree_GetFirstSelected(Handle, out IntPtr value));
            TreeItem? item = TreeItem.FromBorrowed(value);
            while (item is not null)
            {
                yield return item;
                Interop.Check(NativeMethods.CEGUI_C_Tree_GetNextSelected(Handle, item.Handle, out value));
                item = TreeItem.FromBorrowed(value);
            }
        }
    }

    public void AddItem(TreeItem item, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, IntPtr.Zero, false, transferOwnership);
    }

    public void InsertItem(TreeItem item, TreeItem? beforeItem, bool transferOwnership = true)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        AddItemCore(item, beforeItem?.Handle ?? IntPtr.Zero, true, transferOwnership);
    }

    private void AddItemCore(TreeItem item, IntPtr beforeItem, bool insert, bool transferOwnership)
    {
        item.PrepareForCollection(transferOwnership);
        try
        {
            NativeResult result = insert
                ? NativeMethods.CEGUI_C_Tree_InsertItem(Handle, item.Handle, beforeItem)
                : NativeMethods.CEGUI_C_Tree_AddItem(Handle, item.Handle);
            Interop.Check(result);
            item.CompleteOwnershipTransfer(transferOwnership);
        }
        catch
        {
            if (transferOwnership) item.PrepareForCollection(false);
            throw;
        }
    }

    public void RemoveItem(TreeItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        bool deleted = item.NativeAutoDelete;
        Interop.Check(NativeMethods.CEGUI_C_Tree_RemoveItem(Handle, item.Handle));
        if (deleted) item.InvalidateBorrowedHandle();
    }

    public void SetItemSelected(TreeItem item, bool selected)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Tree_SetItemSelected(Handle, item.Handle, selected ? 1 : 0));
    }

    public void EnsureItemVisible(TreeItem item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_Tree_EnsureItemVisible(Handle, item.Handle));
    }

    public void ResetList() => ExecuteAction(WidgetAction.TreeReset);
    public void ClearSelections() => ExecuteAction(WidgetAction.TreeClearSelections);
}

public sealed class TabControl : Window
{
    internal TabControl(IntPtr handle) : base(handle) { }

    public long TabCount { get { Interop.Check(NativeMethods.CEGUI_C_TabControl_GetTabCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }

    public long SelectedIndex
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TabControl_GetSelectedIndex(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); }
        set => Interop.Check(NativeMethods.CEGUI_C_TabControl_SetSelectedIndex(Handle, Interop.ToUIntPtr(value, nameof(value))));
    }

    public TabPanePosition PanePosition
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TabControl_GetPanePosition(Handle, out TabPanePosition value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_TabControl_SetPanePosition(Handle, value));
    }

    public Window TabButtonPane => GetComponent(WidgetComponent.TabButtonPane)!;

    public Window GetContentAt(long index)
    {
        Interop.Check(NativeMethods.CEGUI_C_TabControl_GetContentAt(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr content));
        return FromHandle(content);
    }

    public void AddTab(Window content)
    {
        if (content is null) throw new ArgumentNullException(nameof(content));
        Interop.Check(NativeMethods.CEGUI_C_TabControl_AddTab(Handle, content.Handle));
    }

    public void RemoveTab(string name)
    {
        if (name is null) throw new ArgumentNullException(nameof(name));
        Interop.Check(NativeMethods.CEGUI_C_TabControl_RemoveTabByName(Handle, name));
    }

    public void RemoveTab(uint id) => Interop.Check(NativeMethods.CEGUI_C_TabControl_RemoveTabByID(Handle, id));
    public void MakeVisible(long index) => Interop.Check(NativeMethods.CEGUI_C_TabControl_MakeVisible(Handle, Interop.ToUIntPtr(index, nameof(index))));
}

public class ItemEntry : Window
{
    internal ItemEntry(IntPtr handle) : base(handle) { }
}

public class ItemListBase : Window
{
    internal ItemListBase(IntPtr handle) : base(handle) { }

    public long ItemCount { get { Interop.Check(NativeMethods.CEGUI_C_ItemListBase_GetItemCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }

    public Window GetItemAt(long index)
    {
        Interop.Check(NativeMethods.CEGUI_C_ItemListBase_GetItemAt(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr item));
        return FromHandle(item);
    }

    public void AddItem(Window item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_ItemListBase_AddItem(Handle, item.Handle));
    }

    public void InsertItem(Window item, Window? beforeItem)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_ItemListBase_InsertItem(Handle, item.Handle, beforeItem?.Handle ?? IntPtr.Zero));
    }

    public void RemoveItem(Window item)
    {
        if (item is null) throw new ArgumentNullException(nameof(item));
        Interop.Check(NativeMethods.CEGUI_C_ItemListBase_RemoveItem(Handle, item.Handle));
    }

    public void ResetList() => ExecuteAction(WidgetAction.ItemListReset);
}

public class ScrolledItemListBase : ItemListBase
{
    internal ScrolledItemListBase(IntPtr handle) : base(handle) { }

    public Scrollbar VerticalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ScrolledItemVerticalScrollbar)!;
    public Scrollbar HorizontalScrollbar => GetComponent<Scrollbar>(WidgetComponent.ScrolledItemHorizontalScrollbar)!;
}

public sealed class ItemListbox : ScrolledItemListBase
{
    internal ItemListbox(IntPtr handle) : base(handle) { }

    public long SelectedCount { get { Interop.Check(NativeMethods.CEGUI_C_ItemListbox_GetSelectedCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); } }

    public Window? GetFirstSelected(long startIndex = 0)
    {
        Interop.Check(NativeMethods.CEGUI_C_ItemListbox_GetFirstSelected(Handle, Interop.ToUIntPtr(startIndex, nameof(startIndex)), out IntPtr item));
        return FromNullableHandle(item);
    }

    public Window? GetNextSelected(Window startItem)
    {
        if (startItem is null) throw new ArgumentNullException(nameof(startItem));
        Interop.Check(NativeMethods.CEGUI_C_ItemListbox_GetNextSelected(Handle, startItem.Handle, out IntPtr item));
        return FromNullableHandle(item);
    }

    public void SelectRange(long first, long last) =>
        Interop.Check(NativeMethods.CEGUI_C_ItemListbox_SelectRange(
            Handle, Interop.ToUIntPtr(first, nameof(first)), Interop.ToUIntPtr(last, nameof(last))));

    public void ClearSelections() => ExecuteAction(WidgetAction.ItemListboxClearSelections);
    public void SelectAll() => ExecuteAction(WidgetAction.ItemListboxSelectAll);
}

public class MenuBase : ItemListBase
{
    internal MenuBase(IntPtr handle) : base(handle) { }
}

public sealed class Menubar : MenuBase
{
    internal Menubar(IntPtr handle) : base(handle) { }
}

public sealed class PopupMenu : MenuBase
{
    internal PopupMenu(IntPtr handle) : base(handle) { }
    public void Open() => ExecuteAction(WidgetAction.PopupOpen);
    public void Close() => ExecuteAction(WidgetAction.PopupClose);
}

public sealed class MenuItem : ItemEntry
{
    internal MenuItem(IntPtr handle) : base(handle) { }

    public PopupMenu? PopupMenu
    {
        get
        {
            Interop.Check(NativeMethods.CEGUI_C_MenuItem_GetPopupMenu(Handle, out IntPtr popup));
            return FromNullableHandle(popup) as PopupMenu;
        }
        set => Interop.Check(NativeMethods.CEGUI_C_MenuItem_SetPopupMenu(Handle, value?.Handle ?? IntPtr.Zero));
    }

    public bool TogglePopupMenu()
    {
        Interop.Check(NativeMethods.CEGUI_C_MenuItem_TogglePopupMenu(Handle, out int opened));
        return opened != 0;
    }

    public void OpenPopupMenu() => ExecuteAction(WidgetAction.MenuItemOpenPopup);
    public void ClosePopupMenu() => ExecuteAction(WidgetAction.MenuItemClosePopup);
}
