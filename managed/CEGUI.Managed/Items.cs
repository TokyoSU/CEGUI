using System;
using System.Collections.Generic;

namespace CEGUI;

public sealed class ListboxItem : IDisposable
{
    private IntPtr _handle;
    private bool _ownsHandle;

    public ListboxItem(string text, uint id = 0, bool disabled = false)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_ListboxTextItem_Create(text, id, disabled ? 1 : 0, 0, out _handle));
        _ownsHandle = true;
    }

    private ListboxItem(IntPtr handle, bool ownsHandle)
    {
        if (handle == IntPtr.Zero) throw new ArgumentException("Listbox item handle must not be null.", nameof(handle));
        _handle = handle;
        _ownsHandle = ownsHandle;
    }

    ~ListboxItem() => Release(false);

    internal IntPtr Handle => _handle != IntPtr.Zero
        ? _handle
        : throw new ObjectDisposedException(nameof(ListboxItem));

    internal static ListboxItem? FromBorrowed(IntPtr handle) =>
        handle == IntPtr.Zero ? null : new ListboxItem(handle, false);

    public bool OwnsNativeItem => _ownsHandle;

    public string Text
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_GetText(Handle, out IntPtr value)); return Interop.PtrToUtf8(value); }
        set { if (value is null) throw new ArgumentNullException(nameof(value)); Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetText(Handle, value)); }
    }

    public uint Id
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_GetID(Handle, out uint value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetID(Handle, value));
    }

    public UIntPtr UserData
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_GetUserData(Handle, out UIntPtr value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetUserData(Handle, value));
    }

    public bool Selected
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_IsSelected(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetSelected(Handle, value ? 1 : 0));
    }

    public bool Disabled
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_IsDisabled(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetDisabled(Handle, value ? 1 : 0));
    }

    public bool NativeAutoDelete
    {
        get { Interop.Check(NativeMethods.CEGUI_C_ListboxItem_IsAutoDeleted(Handle, out int value)); return value != 0; }
    }

    public void SetFont(string fontName)
    {
        if (fontName is null) throw new ArgumentNullException(nameof(fontName));
        Interop.Check(NativeMethods.CEGUI_C_ListboxTextItem_SetFont(Handle, fontName));
    }

    public void SetSelectionBrushImage(string imageName)
    {
        if (imageName is null) throw new ArgumentNullException(nameof(imageName));
        Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetSelectionBrushImage(Handle, imageName));
    }

    public void Dispose()
    {
        Release(true);
        GC.SuppressFinalize(this);
    }

    internal void PrepareForCollection(bool transferOwnership)
    {
        Interop.Check(NativeMethods.CEGUI_C_ListboxItem_SetAutoDeleted(Handle, transferOwnership ? 1 : 0));
    }

    internal void CompleteOwnershipTransfer(bool transferOwnership)
    {
        if (transferOwnership)
            _ownsHandle = false;
    }

    internal void InvalidateBorrowedHandle()
    {
        _ownsHandle = false;
        _handle = IntPtr.Zero;
    }

    private void Release(bool throwOnError)
    {
        IntPtr handle = _handle;
        if (handle == IntPtr.Zero) return;

        if (!_ownsHandle)
        {
            _handle = IntPtr.Zero;
            return;
        }

        try
        {
            NativeResult result = NativeMethods.CEGUI_C_ListboxItem_Destroy(handle);
            if (throwOnError) Interop.Check(result);
            if (result == NativeResult.Success) _handle = IntPtr.Zero;
        }
        catch when (!throwOnError)
        {
            // Native libraries may already be unloading during finalization.
        }
    }
}

public sealed class TreeItem : IDisposable
{
    private IntPtr _handle;
    private bool _ownsHandle;

    public TreeItem(string text, uint id = 0, bool disabled = false)
    {
        if (text is null) throw new ArgumentNullException(nameof(text));
        Interop.Check(NativeMethods.CEGUI_C_TreeItem_Create(text, id, disabled ? 1 : 0, 0, out _handle));
        _ownsHandle = true;
    }

    private TreeItem(IntPtr handle, bool ownsHandle)
    {
        if (handle == IntPtr.Zero) throw new ArgumentException("Tree item handle must not be null.", nameof(handle));
        _handle = handle;
        _ownsHandle = ownsHandle;
    }

    ~TreeItem() => Release(false);

    internal IntPtr Handle => _handle != IntPtr.Zero
        ? _handle
        : throw new ObjectDisposedException(nameof(TreeItem));

    internal static TreeItem? FromBorrowed(IntPtr handle) =>
        handle == IntPtr.Zero ? null : new TreeItem(handle, false);

    public bool OwnsNativeItem => _ownsHandle;

    public string Text
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_GetText(Handle, out IntPtr value)); return Interop.PtrToUtf8(value); }
        set { if (value is null) throw new ArgumentNullException(nameof(value)); Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetText(Handle, value)); }
    }

    public uint Id
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_GetID(Handle, out uint value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetID(Handle, value));
    }

    public UIntPtr UserData
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_GetUserData(Handle, out UIntPtr value)); return value; }
        set => Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetUserData(Handle, value));
    }

    public bool Selected
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_IsSelected(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetSelected(Handle, value ? 1 : 0));
    }

    public bool Disabled
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_IsDisabled(Handle, out int value)); return value != 0; }
        set => Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetDisabled(Handle, value ? 1 : 0));
    }

    public bool Open
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_IsOpen(Handle, out int value)); return value != 0; }
    }

    public bool NativeAutoDelete
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_IsAutoDeleted(Handle, out int value)); return value != 0; }
    }

    public long ChildCount
    {
        get { Interop.Check(NativeMethods.CEGUI_C_TreeItem_GetChildCount(Handle, out UIntPtr value)); return Interop.FromUIntPtr(value); }
    }

    public IEnumerable<TreeItem> Children
    {
        get
        {
            long count = ChildCount;
            for (long index = 0; index < count; ++index)
                yield return GetChildAt(index);
        }
    }

    public void ToggleOpen() => Interop.Check(NativeMethods.CEGUI_C_TreeItem_ToggleOpen(Handle));

    public TreeItem GetChildAt(long index)
    {
        Interop.Check(NativeMethods.CEGUI_C_TreeItem_GetChildAt(Handle, Interop.ToUIntPtr(index, nameof(index)), out IntPtr child));
        return FromBorrowed(child) ?? throw new CeguiException(NativeResult.UnknownException, "CEGUI returned a null TreeItem child.");
    }

    public void AddChild(TreeItem child, bool transferOwnership = true)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        child.PrepareForCollection(transferOwnership);
        Interop.Check(NativeMethods.CEGUI_C_TreeItem_AddChild(Handle, child.Handle));
        child.CompleteOwnershipTransfer(transferOwnership);
    }

    public void RemoveChild(TreeItem child)
    {
        if (child is null) throw new ArgumentNullException(nameof(child));
        bool deleted = child.NativeAutoDelete;
        Interop.Check(NativeMethods.CEGUI_C_TreeItem_RemoveChild(Handle, child.Handle));
        if (deleted) child.InvalidateBorrowedHandle();
    }

    public void Dispose()
    {
        Release(true);
        GC.SuppressFinalize(this);
    }

    internal void PrepareForCollection(bool transferOwnership)
    {
        Interop.Check(NativeMethods.CEGUI_C_TreeItem_SetAutoDeleted(Handle, transferOwnership ? 1 : 0));
    }

    internal void CompleteOwnershipTransfer(bool transferOwnership)
    {
        if (transferOwnership)
            _ownsHandle = false;
    }

    internal void InvalidateBorrowedHandle()
    {
        _ownsHandle = false;
        _handle = IntPtr.Zero;
    }

    private void Release(bool throwOnError)
    {
        IntPtr handle = _handle;
        if (handle == IntPtr.Zero) return;

        if (!_ownsHandle)
        {
            _handle = IntPtr.Zero;
            return;
        }

        try
        {
            NativeResult result = NativeMethods.CEGUI_C_TreeItem_Destroy(handle);
            if (throwOnError) Interop.Check(result);
            if (result == NativeResult.Success) _handle = IntPtr.Zero;
        }
        catch when (!throwOnError)
        {
            // Native libraries may already be unloading during finalization.
        }
    }
}
