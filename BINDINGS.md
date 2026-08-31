# Optional C and C# bindings

The language bindings are disabled by default and do not affect an ordinary
CEGUI build.

- `CEGUI.Native` is the stable C ABI DLL.
- `CEGUI.Managed` is the C# wrapper assembly and depends on `CEGUI.Native`.

## CMake options

| Option | Default | Result |
|---|---:|---|
| `BUILD_C_BINDING` | `OFF` | Builds the optional `CEGUI.Native` DLL. |
| `BUILD_CSHARP_BINDING` | `OFF` | Builds `CEGUI.Managed` and automatically enables `BUILD_C_BINDING`. |

For complete Unicode text and `GUIContext.InjectChar` support, configure with:

```powershell
-DCEGUI_STRING_CLASS=UNICODE
```

### Build only the native C binding

```powershell
cmake -S . -B Build `
  -DBUILD_C_BINDING=ON `
  -DBUILD_CSHARP_BINDING=OFF `
  -DBUILD_RENDERER_D3D11=ON `
  -DCEGUI_STRING_CLASS=UNICODE

cmake --build Build --config Debug --target CEGUI.Native
```

### Build the native and managed bindings

The .NET SDK must be available through `dotnet`.

```powershell
cmake -S . -B Build `
  -DBUILD_CSHARP_BINDING=ON `
  -DBUILD_RENDERER_D3D11=ON `
  -DCEGUI_STRING_CLASS=UNICODE

cmake --build Build --config Debug --target CEGUI.Managed
```

`CEGUI.Managed` depends on `CEGUI.Native`, so the native DLL is always built
first. CMake passes the resulting native path to the managed project, which
copies the native DLL beside each managed build output.

When `BUILD_SHARED_LIBS=ON`, the other CEGUI DLLs and their runtime dependencies
must also be beside the application or on its DLL search path.

When `BUILD_SHARED_LIBS=OFF`, CEGUI, one selected XML parser, and one selected
image codec are linked into `CEGUI.Native.dll`. The binding itself remains a DLL
because P/Invoke needs a native dynamic-library boundary.

A C++ application that already owns CEGUI and calls
`CeguiSystem.FromExisting()` from C# should use `BUILD_SHARED_LIBS=ON`, with the
application and `CEGUI.Native.dll` linked against the same `CEGUIBase.dll`.
Separately statically linked copies contain separate CEGUI singletons.

## Native C API

Include the main C-only header:

```c
#include <CEGUI/CEGUIC.h>

CEGUI_C_System system = NULL;
CEGUI_C_Result result = CEGUI_C_System_GetExisting(&system);
if (result != CEGUI_C_RESULT_SUCCESS)
    fprintf(stderr, "CEGUI error: %s\n", CEGUI_C_GetLastError());
```

`CEGUIC.h` includes the full widget API from `CEGUIWidgets.h`.

The ABI exposes only C-compatible values:

- opaque handles instead of C++ classes
- UTF-8 `char*` strings
- fixed-width integers and C structures
- result codes instead of exceptions
- callbacks using C function pointers

All C++ exceptions are caught inside `CEGUI.Native` and converted into a
`CEGUI_C_Result`. `CEGUI_C_GetLastError()` returns the associated UTF-8 message.
Returned string pointers are temporary and must be copied before another
binding call on the same thread.

## Managed C# usage

Reference:

```text
managed/CEGUI.Managed/CEGUI.Managed.csproj
```

The assembly targets `netstandard2.1` and `net8.0`.

When the C++ engine already owns the CEGUI system:

```csharp
using CEGUI;

using CeguiSystem cegui = CeguiSystem.FromExisting();
GuiContext context = cegui.DefaultContext;

Window root = WindowManager.LoadLayoutFromFile("Login.layout");
context.RootWindow = root;

PushButton loginButton = root.GetChild<PushButton>("login_button");
using EventConnection click = loginButton.SubscribeEvent(
    "Clicked",
    _ =>
    {
        Console.WriteLine("Login clicked");
        return true;
    });
```

When C# owns initialisation:

```csharp
using CeguiSystem cegui = CeguiSystem.BootstrapD3D11(devicePtr, contextPtr);
```

The `ID3D11Device*` and immediate `ID3D11DeviceContext*` must remain alive until
the owning `CeguiSystem` is disposed because the renderer stores borrowed
native pointers.

When the host already owns an initialised bgfx instance, C# can bootstrap the
optional bgfx renderer instead:

```csharp
using CeguiSystem cegui = CeguiSystem.BootstrapBgfx(
    width: 1920,
    height: 1080,
    viewIdBase: 240,
    viewIdCount: 16);
```

Build it with `-DBUILD_RENDERER_BGFX=ON` and install bgfx with the `tools`
feature so CMake can compile the renderer shaders. The host owns the bgfx
lifecycle: CEGUI does not call `bgfx::init()`, `bgfx::shutdown()`, or
`bgfx::frame()`. The supplied view-ID range must be reserved exclusively for
CEGUI; the first view is used for the default target and the remaining views
are allocated to CEGUI texture targets. `shaderRoot` may be supplied to
`BootstrapBgfx` when the generated `Shaders/bgfx/<backend>` directory is not at
the default build-time location.


## Widget coverage

`WindowManager.CreateWindow`, `LoadLayoutFromFile`, `GetWindow`, child lookup,
and component lookup automatically inspect the native runtime type and return
the matching managed class.

Every widget shipped by this source tree has a managed type:

- `DefaultWindow`
- `ButtonBase`, `PushButton`, `ToggleButton`, `RadioButton`
- `ProgressBar`
- `Editbox`, `MultiLineEditbox`
- `FrameWindow`, `Titlebar`, `Tooltip`
- `DragContainer`, `ClippedContainer`
- `Scrollbar`, `Slider`, `Thumb`, `Spinner`
- `Combobox`, `ComboDropList`, `Listbox`
- `MultiColumnList`, `ListHeader`, `ListHeaderSegment`
- `Tree`
- `TabControl`, `TabButton`
- `MenuBase`, `Menubar`, `PopupMenu`, `MenuItem`
- `ScrollablePane`, `ScrolledContainer`
- `LayoutContainer`, `LayoutCell`, `SequentialLayoutContainer`,
  `HorizontalLayoutContainer`, `VerticalLayoutContainer`, and
  `GridLayoutContainer`
- `ItemEntry`, `ItemListBase`, `ScrolledItemListBase`, and `ItemListbox`
- `GroupBox`
- the source-specific `RoundImage` and `SlotImageItem`

Supporting non-window objects are also wrapped:

- `ListboxItem` backed by `CEGUI::ListboxTextItem`
- `TreeItem`
- `MultiColumnGridReference`

All widget classes inherit the generic `Window` text, hierarchy, area, state,
property, and event APIs. This keeps every registered CEGUI property and event
available even when a property does not need a dedicated C function.
Collection-heavy and component-heavy widgets additionally expose typed methods
for list items, tree hierarchies, multi-column cells, columns, tabs, menus,
scrollbars, content panes, and layouts.

### Listbox example

```csharp
Listbox list = WindowManager.GetWindow<Listbox>("Inventory/List");
var sword = new ListboxItem("Sword", id: 1001);

// The default transfers ownership to CEGUI. The wrapper remains usable while
// the item exists, but CEGUI destroys it when the owning list destroys it.
list.AddItem(sword);
list.SetItemSelected(sword, true);
```

Pass `transferOwnership: false` when managed code must retain ownership. In
that mode, remove the item from its widget before calling `Dispose()`.
`CEGUI.Native` rejects destruction while an item still belongs to a widget to
avoid leaving a dangling pointer inside CEGUI.

### Tree example

```csharp
Tree tree = WindowManager.GetWindow<Tree>("QuestTree");
var chapter = new TreeItem("Chapter 1");
chapter.AddChild(new TreeItem("First quest"));
tree.AddItem(chapter);
```

### Multi-column example

```csharp
MultiColumnList table = WindowManager.GetWindow<MultiColumnList>("Players");
table.AddColumn("Name", 1, UDim.Relative(0.6f));
table.AddColumn("Level", 2, UDim.Relative(0.4f));
uint row = table.AddRow();
table.SetItem(new ListboxItem("Alice"), 1, row);
table.SetItem(new ListboxItem("42"), 2, row);
```

## Ownership rules

- Windows are owned by CEGUI's `WindowManager` and managed `Window` wrappers are
  borrowed views.
- Event connections are owned handles and must be disposed.
- Newly created `ListboxItem` and `TreeItem` objects are managed-owned until
  added with `transferOwnership: true`.
- Items returned by a widget are borrowed wrappers.
- A borrowed or transferred item must not be used after its owner removes,
  resets, or destroys it.
