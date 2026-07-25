using System;
using System.Runtime.InteropServices;

namespace CEGUI;

internal enum NativeResult
{
    Success = 0,
    InvalidArgument = 1,
    NotInitialised = 2,
    CeguiException = 3,
    StandardException = 4,
    UnknownException = 5,
    NotSupported = 6
}

public enum MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2,
    X1 = 3,
    X2 = 4,
    Count = 5,
    None = 6
}

public enum EventKind
{
    Generic = 0,
    Window = 1,
    Mouse = 2,
    Key = 3,
    Activation = 4,
    Update = 5
}

public enum KeyScan : uint
{
    Unknown = 0x00,
    Escape = 0x01,
    One = 0x02,
    Two = 0x03,
    Three = 0x04,
    Four = 0x05,
    Five = 0x06,
    Six = 0x07,
    Seven = 0x08,
    Eight = 0x09,
    Nine = 0x0A,
    Zero = 0x0B,
    Minus = 0x0C,
    Equals = 0x0D,
    Backspace = 0x0E,
    Tab = 0x0F,
    Q = 0x10,
    W = 0x11,
    E = 0x12,
    R = 0x13,
    T = 0x14,
    Y = 0x15,
    U = 0x16,
    I = 0x17,
    O = 0x18,
    P = 0x19,
    LeftBracket = 0x1A,
    RightBracket = 0x1B,
    Return = 0x1C,
    LeftControl = 0x1D,
    A = 0x1E,
    S = 0x1F,
    D = 0x20,
    F = 0x21,
    G = 0x22,
    H = 0x23,
    J = 0x24,
    K = 0x25,
    L = 0x26,
    Semicolon = 0x27,
    Apostrophe = 0x28,
    Grave = 0x29,
    LeftShift = 0x2A,
    Backslash = 0x2B,
    Z = 0x2C,
    X = 0x2D,
    C = 0x2E,
    V = 0x2F,
    B = 0x30,
    N = 0x31,
    M = 0x32,
    Comma = 0x33,
    Period = 0x34,
    Slash = 0x35,
    RightShift = 0x36,
    Multiply = 0x37,
    LeftAlt = 0x38,
    Space = 0x39,
    Capital = 0x3A,
    F1 = 0x3B,
    F2 = 0x3C,
    F3 = 0x3D,
    F4 = 0x3E,
    F5 = 0x3F,
    F6 = 0x40,
    F7 = 0x41,
    F8 = 0x42,
    F9 = 0x43,
    F10 = 0x44,
    NumLock = 0x45,
    ScrollLock = 0x46,
    Numpad7 = 0x47,
    Numpad8 = 0x48,
    Numpad9 = 0x49,
    Subtract = 0x4A,
    Numpad4 = 0x4B,
    Numpad5 = 0x4C,
    Numpad6 = 0x4D,
    Add = 0x4E,
    Numpad1 = 0x4F,
    Numpad2 = 0x50,
    Numpad3 = 0x51,
    Numpad0 = 0x52,
    Decimal = 0x53,
    Oem102 = 0x56,
    F11 = 0x57,
    F12 = 0x58,
    F13 = 0x64,
    F14 = 0x65,
    F15 = 0x66,
    Kana = 0x70,
    AbntC1 = 0x73,
    Convert = 0x79,
    NoConvert = 0x7B,
    Yen = 0x7D,
    AbntC2 = 0x7E,
    NumpadEquals = 0x8D,
    PrevTrack = 0x90,
    At = 0x91,
    Colon = 0x92,
    Underline = 0x93,
    Kanji = 0x94,
    Stop = 0x95,
    Ax = 0x96,
    Unlabeled = 0x97,
    NextTrack = 0x99,
    NumpadEnter = 0x9C,
    RightControl = 0x9D,
    Mute = 0xA0,
    Calculator = 0xA1,
    PlayPause = 0xA2,
    MediaStop = 0xA4,
    VolumeDown = 0xAE,
    VolumeUp = 0xB0,
    WebHome = 0xB2,
    NumpadComma = 0xB3,
    Divide = 0xB5,
    SysRq = 0xB7,
    RightAlt = 0xB8,
    Pause = 0xC5,
    Home = 0xC7,
    ArrowUp = 0xC8,
    PageUp = 0xC9,
    ArrowLeft = 0xCB,
    ArrowRight = 0xCD,
    End = 0xCF,
    ArrowDown = 0xD0,
    PageDown = 0xD1,
    Insert = 0xD2,
    Delete = 0xD3,
    LeftWindows = 0xDB,
    RightWindows = 0xDC,
    AppMenu = 0xDD,
    Power = 0xDE,
    Sleep = 0xDF,
    Wake = 0xE3,
    WebSearch = 0xE5,
    WebFavorites = 0xE6,
    WebRefresh = 0xE7,
    WebStop = 0xE8,
    WebForward = 0xE9,
    WebBack = 0xEA,
    MyComputer = 0xEB,
    Mail = 0xEC,
    MediaSelect = 0xED
}

[StructLayout(LayoutKind.Sequential)]
public readonly struct UDim
{
    public UDim(float scale, float offset)
    {
        Scale = scale;
        Offset = offset;
    }

    public readonly float Scale;
    public readonly float Offset;

    public static UDim Relative(float scale) => new(scale, 0.0f);
    public static UDim Pixels(float pixels) => new(0.0f, pixels);
}

[StructLayout(LayoutKind.Sequential)]
public readonly struct UVector2
{
    public UVector2(UDim x, UDim y)
    {
        X = x;
        Y = y;
    }

    public readonly UDim X;
    public readonly UDim Y;
}

[StructLayout(LayoutKind.Sequential)]
public readonly struct USize
{
    public USize(UDim width, UDim height)
    {
        Width = width;
        Height = height;
    }

    public readonly UDim Width;
    public readonly UDim Height;
}


public enum WidgetKind
{
    Window = 0,
    DefaultWindow,
    ButtonBase,
    PushButton,
    ToggleButton,
    RadioButton,
    ProgressBar,
    Editbox,
    MultiLineEditbox,
    FrameWindow,
    Titlebar,
    Tooltip,
    DragContainer,
    ClippedContainer,
    Scrollbar,
    Slider,
    Thumb,
    Spinner,
    Combobox,
    ComboDropList,
    Listbox,
    MultiColumnList,
    ListHeader,
    ListHeaderSegment,
    Tree,
    TabControl,
    TabButton,
    MenuBase,
    Menubar,
    PopupMenu,
    MenuItem,
    ScrollablePane,
    ScrolledContainer,
    LayoutContainer,
    LayoutCell,
    SequentialLayoutContainer,
    HorizontalLayoutContainer,
    VerticalLayoutContainer,
    GridLayoutContainer,
    ItemEntry,
    ItemListBase,
    ScrolledItemListBase,
    ItemListbox,
    GroupBox,
    RoundImage,
    SlotImageItem
}

internal enum WidgetComponent
{
    FrameTitlebar = 0,
    FrameCloseButton,
    ComboboxEditbox,
    ComboboxButton,
    ComboboxDropList,
    ScrollbarIncreaseButton,
    ScrollbarDecreaseButton,
    ScrollbarThumb,
    SliderThumb,
    ListboxVerticalScrollbar,
    ListboxHorizontalScrollbar,
    MultiColumnListHeader,
    MultiColumnVerticalScrollbar,
    MultiColumnHorizontalScrollbar,
    MultiLineVerticalScrollbar,
    MultiLineHorizontalScrollbar,
    ScrollableContentPane,
    ScrollableVerticalScrollbar,
    ScrollableHorizontalScrollbar,
    GroupBoxContentPane,
    TooltipTarget,
    DragDropTarget,
    SpinnerEditbox,
    SpinnerIncreaseButton,
    SpinnerDecreaseButton,
    ScrolledItemVerticalScrollbar,
    ScrolledItemHorizontalScrollbar,
    TabButtonPane
}

internal enum WidgetAction
{
    ProgressStep = 0,
    FrameToggleRollup,
    TooltipResetTimer,
    TooltipPositionSelf,
    TooltipSizeSelf,
    ComboboxShowDropList,
    ComboboxHideDropList,
    ListboxReset,
    ListboxClearSelections,
    ComboboxResetList,
    ComboboxClearSelections,
    MultiColumnReset,
    MultiColumnClearSelections,
    TreeReset,
    TreeClearSelections,
    ItemListReset,
    ItemListboxClearSelections,
    ItemListboxSelectAll,
    LayoutMarkDirty,
    LayoutNow,
    LayoutIfNeeded,
    PopupOpen,
    PopupClose,
    MenuItemOpenPopup,
    MenuItemClosePopup
}

public enum MultiColumnSelectionMode
{
    RowSingle = 0,
    RowMultiple = 1,
    CellSingle = 2,
    CellMultiple = 3,
    NominatedColumnSingle = 4,
    NominatedColumnMultiple = 5,
    ColumnSingle = 6,
    ColumnMultiple = 7,
    NominatedRowSingle = 8,
    NominatedRowMultiple = 9
}

public enum ListSortDirection
{
    None = 0,
    Ascending = 1,
    Descending = 2
}

public enum TabPanePosition
{
    Top = 0,
    Bottom = 1
}

public enum GridAutoPositioning
{
    Disabled = 0,
    LeftToRight = 1,
    TopToBottom = 2
}

[StructLayout(LayoutKind.Sequential)]
public readonly struct MultiColumnGridReference
{
    public MultiColumnGridReference(uint row, uint column)
    {
        Row = row;
        Column = column;
    }

    public readonly uint Row;
    public readonly uint Column;
}

[StructLayout(LayoutKind.Sequential)]
internal struct NativeEventData
{
    internal EventKind Kind;
    internal IntPtr Window;
    internal IntPtr OtherWindow;
    internal uint HandledCount;
    internal float PositionX;
    internal float PositionY;
    internal float MoveDeltaX;
    internal float MoveDeltaY;
    internal float WheelChange;
    internal uint MouseButton;
    internal uint ClickCount;
    internal uint SystemKeys;
    internal uint Codepoint;
    internal uint Scancode;
    internal float Elapsed;
}
