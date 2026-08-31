using System;

namespace CEGUI;

public sealed class CeguiSystem : IDisposable
{
    private enum BootstrapOwner
    {
        None,
        D3D11,
        Bgfx
    }

    private BootstrapOwner _bootstrapOwner;
    private bool _disposed;

    private CeguiSystem(IntPtr handle, BootstrapOwner bootstrapOwner)
    {
        Handle = handle;
        _bootstrapOwner = bootstrapOwner;
    }

    internal IntPtr Handle { get; private set; }

    public static Version NativeVersion => new(
        checked((int)NativeMethods.CEGUI_C_GetVersionMajor()),
        checked((int)NativeMethods.CEGUI_C_GetVersionMinor()),
        checked((int)NativeMethods.CEGUI_C_GetVersionPatch()));

    public static string NativeVersionString => Interop.PtrToUtf8(NativeMethods.CEGUI_C_GetVersionString());

    public static bool UsesUnicodeString => NativeMethods.CEGUI_C_IsUnicodeStringBuild() != 0;

    public static CeguiSystem BootstrapD3D11(IntPtr device, IntPtr deviceContext)
    {
        if (device == IntPtr.Zero)
            throw new ArgumentException("The ID3D11Device pointer must not be null.", nameof(device));
        if (deviceContext == IntPtr.Zero)
            throw new ArgumentException("The ID3D11DeviceContext pointer must not be null.", nameof(deviceContext));

        Interop.Check(NativeMethods.CEGUI_C_D3D11_BootstrapSystem(device, deviceContext, out IntPtr system));
        return new CeguiSystem(system, BootstrapOwner.D3D11);
    }

    /// <summary>
    /// Bootstraps CEGUI using an already-initialised bgfx instance. The host retains
    /// ownership of bgfx and must reserve the supplied view-ID range for CEGUI.
    /// </summary>
    public static CeguiSystem BootstrapBgfx(
        float width,
        float height,
        ushort viewIdBase = 240,
        ushort viewIdCount = 16,
        string? shaderRoot = null)
    {
        if (width <= 0.0f)
            throw new ArgumentOutOfRangeException(nameof(width), "Display width must be positive.");
        if (height <= 0.0f)
            throw new ArgumentOutOfRangeException(nameof(height), "Display height must be positive.");
        if (viewIdCount == 0)
            throw new ArgumentOutOfRangeException(nameof(viewIdCount), "At least one bgfx view ID must be reserved.");

        Interop.Check(NativeMethods.CEGUI_C_Bgfx_BootstrapSystem(
            width, height, viewIdBase, viewIdCount, shaderRoot, out IntPtr system));
        return new CeguiSystem(system, BootstrapOwner.Bgfx);
    }

    public static CeguiSystem FromExisting()
    {
        Interop.Check(NativeMethods.CEGUI_C_System_GetExisting(out IntPtr system));
        return new CeguiSystem(system, BootstrapOwner.None);
    }

    public GuiContext DefaultContext
    {
        get
        {
            ThrowIfDisposed();
            Interop.Check(NativeMethods.CEGUI_C_System_GetDefaultGUIContext(Handle, out IntPtr context));
            return new GuiContext(context);
        }
    }

    public void RenderAllGuiContexts()
    {
        ThrowIfDisposed();
        Interop.Check(NativeMethods.CEGUI_C_System_RenderAllGUIContexts(Handle));
    }

    public bool InjectTimePulse(float elapsedSeconds)
    {
        ThrowIfDisposed();
        Interop.Check(NativeMethods.CEGUI_C_System_InjectTimePulse(Handle, elapsedSeconds, out int handled));
        return handled != 0;
    }

    public void NotifyDisplaySizeChanged(float width, float height)
    {
        ThrowIfDisposed();
        Interop.Check(NativeMethods.CEGUI_C_System_NotifyDisplaySizeChanged(Handle, width, height));
    }

    public void SetResourceGroupDirectory(string resourceGroup, string directory)
    {
        ThrowIfDisposed();
        if (resourceGroup is null) throw new ArgumentNullException(nameof(resourceGroup));
        if (directory is null) throw new ArgumentNullException(nameof(directory));
        Interop.Check(NativeMethods.CEGUI_C_ResourceProvider_SetGroupDirectory(Handle, resourceGroup, directory));
    }

    public string GetResourceGroupDirectory(string resourceGroup)
    {
        ThrowIfDisposed();
        if (resourceGroup is null) throw new ArgumentNullException(nameof(resourceGroup));
        Interop.Check(NativeMethods.CEGUI_C_ResourceProvider_GetGroupDirectory(Handle, resourceGroup, out IntPtr directory));
        return Interop.PtrToUtf8(directory);
    }

    public void ClearResourceGroupDirectory(string resourceGroup)
    {
        ThrowIfDisposed();
        if (resourceGroup is null) throw new ArgumentNullException(nameof(resourceGroup));
        Interop.Check(NativeMethods.CEGUI_C_ResourceProvider_ClearGroupDirectory(Handle, resourceGroup));
    }

    public void Dispose()
    {
        if (_disposed)
            return;

        if (Handle != IntPtr.Zero)
        {
            switch (_bootstrapOwner)
            {
                case BootstrapOwner.D3D11:
                    Interop.Check(NativeMethods.CEGUI_C_D3D11_DestroySystem());
                    break;
                case BootstrapOwner.Bgfx:
                    Interop.Check(NativeMethods.CEGUI_C_Bgfx_DestroySystem());
                    break;
            }
        }

        _bootstrapOwner = BootstrapOwner.None;
        Handle = IntPtr.Zero;
        _disposed = true;
        GC.SuppressFinalize(this);
    }

    private void ThrowIfDisposed()
    {
        if (_disposed || Handle == IntPtr.Zero)
            throw new ObjectDisposedException(nameof(CeguiSystem));
    }
}
