using System;
using System.Runtime.InteropServices;

namespace CEGUI;

public sealed class CeguiEvent
{
    internal CeguiEvent(NativeEventData data)
    {
        Kind = data.Kind;
        Window = Window.FromNullableHandle(data.Window);
        OtherWindow = Window.FromNullableHandle(data.OtherWindow);
        HandledCount = data.HandledCount;
        PositionX = data.PositionX; PositionY = data.PositionY;
        MoveDeltaX = data.MoveDeltaX; MoveDeltaY = data.MoveDeltaY;
        WheelChange = data.WheelChange;
        MouseButton = (MouseButton)data.MouseButton;
        ClickCount = data.ClickCount; SystemKeys = data.SystemKeys;
        Codepoint = data.Codepoint; ScanCode = (KeyScan)data.Scancode;
        ElapsedSeconds = data.Elapsed;
    }
    public EventKind Kind { get; }
    public Window? Window { get; }
    public Window? OtherWindow { get; }
    public uint HandledCount { get; }
    public float PositionX { get; }
    public float PositionY { get; }
    public float MoveDeltaX { get; }
    public float MoveDeltaY { get; }
    public float WheelChange { get; }
    public MouseButton MouseButton { get; }
    public uint ClickCount { get; }
    public uint SystemKeys { get; }
    public uint Codepoint { get; }
    public KeyScan ScanCode { get; }
    public float ElapsedSeconds { get; }
}

public sealed class EventConnection : IDisposable
{
    private sealed class State { internal State(Func<CeguiEvent, bool> handler) => Handler = handler; internal Func<CeguiEvent, bool> Handler { get; } }
    private static readonly NativeMethods.EventCallback Callback = Dispatch;
    private IntPtr _native;
    private GCHandle _state;

    private EventConnection(IntPtr native, GCHandle state) { _native = native; _state = state; }

    internal static EventConnection Create(Window window, string eventName, Func<CeguiEvent, bool> handler)
    {
        if (eventName is null) throw new ArgumentNullException(nameof(eventName));
        if (handler is null) throw new ArgumentNullException(nameof(handler));
        GCHandle state = GCHandle.Alloc(new State(handler));
        try
        {
            Interop.Check(NativeMethods.CEGUI_C_Window_SubscribeEvent(window.Handle, eventName, Callback, GCHandle.ToIntPtr(state), out IntPtr native));
            return new EventConnection(native, state);
        }
        catch { state.Free(); throw; }
    }

    public bool IsConnected { get { if (_native == IntPtr.Zero) return false; Interop.Check(NativeMethods.CEGUI_C_EventConnection_IsConnected(_native, out int v)); return v != 0; } }
    public void Disconnect() { if (_native != IntPtr.Zero) Interop.Check(NativeMethods.CEGUI_C_EventConnection_Disconnect(_native)); }
    public void Dispose() { if (_native != IntPtr.Zero) { NativeMethods.CEGUI_C_EventConnection_Release(_native); _native = IntPtr.Zero; } if (_state.IsAllocated) _state.Free(); GC.SuppressFinalize(this); }
    ~EventConnection() { Dispose(); }

    private static int Dispatch(ref NativeEventData data, IntPtr userData)
    {
        try { return ((State)GCHandle.FromIntPtr(userData).Target!).Handler(new CeguiEvent(data)) ? 1 : 0; }
        catch { return 0; }
    }
}
