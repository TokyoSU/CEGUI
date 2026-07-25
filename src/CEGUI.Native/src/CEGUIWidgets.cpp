#include "CEGUI/CEGUIWidgets.h"

#include "CEGUI/Exceptions.h"
#include "CEGUI/MemoryAllocation.h"
#include "CEGUI/Widgets/All.h"
#include "CEGUI/Widgets/ButtonBase.h"
#include "CEGUI/Widgets/ClippedContainer.h"
#include "CEGUI/Widgets/ItemListBase.h"
#include "CEGUI/Widgets/LayoutContainer.h"
#include "CEGUI/Widgets/ListHeaderSegment.h"
#include "CEGUI/Widgets/MenuBase.h"
#include "CEGUI/Widgets/ScrolledItemListBase.h"
#include "CEGUI/Widgets/SequentialLayoutContainer.h"

#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>

extern "C" void CEGUI_C_InternalSetError(const char* message);

namespace
{
std::string toUtf8Copy(const CEGUI::String& value)
{
#if defined(CEGUI_STRING_CLASS_UNICODE)
    return reinterpret_cast<const char*>(value.c_str());
#else
    return value.c_str();
#endif
}

CEGUI::String fromUtf8(const char* value)
{
    return CEGUI::String(value ? value : "");
}

thread_local std::string g_widgetReturnString;

const char* storeReturnString(const CEGUI::String& value)
{
    g_widgetReturnString = toUtf8Copy(value);
    return g_widgetReturnString.c_str();
}

CEGUI::Window* asWindow(CEGUI_C_Window value)
{
    return reinterpret_cast<CEGUI::Window*>(value);
}

CEGUI_C_Window toWindow(CEGUI::Window* value)
{
    return reinterpret_cast<CEGUI_C_Window>(value);
}

CEGUI::ListboxItem* asListboxItem(CEGUI_C_ListboxItem value)
{
    return reinterpret_cast<CEGUI::ListboxItem*>(value);
}

CEGUI_C_ListboxItem toListboxItem(CEGUI::ListboxItem* value)
{
    return reinterpret_cast<CEGUI_C_ListboxItem>(value);
}

CEGUI::TreeItem* asTreeItem(CEGUI_C_TreeItem value)
{
    return reinterpret_cast<CEGUI::TreeItem*>(value);
}

CEGUI_C_TreeItem toTreeItem(CEGUI::TreeItem* value)
{
    return reinterpret_cast<CEGUI_C_TreeItem>(value);
}

CEGUI::UDim toUDim(CEGUI_C_UDim value)
{
    return CEGUI::UDim(value.scale, value.offset);
}

CEGUI_C_UDim fromUDim(const CEGUI::UDim& value)
{
    CEGUI_C_UDim result = { value.d_scale, value.d_offset };
    return result;
}

CEGUI::MCLGridRef toGridRef(CEGUI_C_MCLGridRef value)
{
    return CEGUI::MCLGridRef(value.row, value.column);
}

template<typename T>
T* checkedWidget(CEGUI_C_Window handle, const char* expected)
{
    if (!handle)
        throw std::invalid_argument("Window handle must not be null.");

    T* widget = dynamic_cast<T*>(asWindow(handle));
    if (!widget)
        throw std::invalid_argument(std::string("Window is not a ") + expected + ".");
    return widget;
}

CEGUI::ListboxItem* checkedListboxItem(CEGUI_C_ListboxItem handle)
{
    if (!handle)
        throw std::invalid_argument("ListboxItem handle must not be null.");
    return asListboxItem(handle);
}

CEGUI::TreeItem* checkedTreeItem(CEGUI_C_TreeItem handle)
{
    if (!handle)
        throw std::invalid_argument("TreeItem handle must not be null.");
    return asTreeItem(handle);
}

#define CEGUI_WIDGET_BEGIN() try
#define CEGUI_WIDGET_END() \
    catch (const CEGUI::Exception& exception) \
    { \
        const std::string message = toUtf8Copy(exception.getMessage()); \
        CEGUI_C_InternalSetError(message.c_str()); \
        return CEGUI_C_RESULT_CEGUI_EXCEPTION; \
    } \
    catch (const std::exception& exception) \
    { \
        CEGUI_C_InternalSetError(exception.what()); \
        return CEGUI_C_RESULT_STD_EXCEPTION; \
    } \
    catch (...) \
    { \
        CEGUI_C_InternalSetError("Unknown native widget binding exception."); \
        return CEGUI_C_RESULT_UNKNOWN_EXCEPTION; \
    }

#define REQUIRE_OUT(value, name) \
    do { if (!(value)) throw std::invalid_argument(std::string(name) + " must not be null."); } while (false)

} // namespace

extern "C"
{
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetWidgetKind(
    CEGUI_C_Window windowHandle, CEGUI_C_WidgetKind* out_kind)
{
    CEGUI_WIDGET_BEGIN()
    {
        REQUIRE_OUT(out_kind, "out_kind");
        CEGUI::Window* window = asWindow(windowHandle);
        if (!window)
            throw std::invalid_argument("Window handle must not be null.");

#define KIND(type, kind) if (dynamic_cast<CEGUI::type*>(window)) { *out_kind = kind; return CEGUI_C_RESULT_SUCCESS; }
        KIND(SlotImageItem, CEGUI_C_WIDGET_SLOT_IMAGE_ITEM)
        KIND(RoundImage, CEGUI_C_WIDGET_ROUND_IMAGE)
        KIND(ComboDropList, CEGUI_C_WIDGET_COMBO_DROP_LIST)
        KIND(Combobox, CEGUI_C_WIDGET_COMBOBOX)
        KIND(RadioButton, CEGUI_C_WIDGET_RADIO_BUTTON)
        KIND(ToggleButton, CEGUI_C_WIDGET_TOGGLE_BUTTON)
        KIND(TabButton, CEGUI_C_WIDGET_TAB_BUTTON)
        KIND(Thumb, CEGUI_C_WIDGET_THUMB)
        KIND(ListHeaderSegment, CEGUI_C_WIDGET_LIST_HEADER_SEGMENT)
        KIND(MenuItem, CEGUI_C_WIDGET_MENU_ITEM)
        KIND(PushButton, CEGUI_C_WIDGET_PUSH_BUTTON)
        KIND(ButtonBase, CEGUI_C_WIDGET_BUTTON_BASE)
        KIND(MultiLineEditbox, CEGUI_C_WIDGET_MULTI_LINE_EDITBOX)
        KIND(Editbox, CEGUI_C_WIDGET_EDITBOX)
        KIND(FrameWindow, CEGUI_C_WIDGET_FRAME_WINDOW)
        KIND(Titlebar, CEGUI_C_WIDGET_TITLEBAR)
        KIND(Tooltip, CEGUI_C_WIDGET_TOOLTIP)
        KIND(DragContainer, CEGUI_C_WIDGET_DRAG_CONTAINER)
        KIND(ClippedContainer, CEGUI_C_WIDGET_CLIPPED_CONTAINER)
        KIND(ProgressBar, CEGUI_C_WIDGET_PROGRESS_BAR)
        KIND(Scrollbar, CEGUI_C_WIDGET_SCROLLBAR)
        KIND(Slider, CEGUI_C_WIDGET_SLIDER)
        KIND(Spinner, CEGUI_C_WIDGET_SPINNER)
        KIND(MultiColumnList, CEGUI_C_WIDGET_MULTI_COLUMN_LIST)
        KIND(Listbox, CEGUI_C_WIDGET_LISTBOX)
        KIND(ListHeader, CEGUI_C_WIDGET_LIST_HEADER)
        KIND(Tree, CEGUI_C_WIDGET_TREE)
        KIND(TabControl, CEGUI_C_WIDGET_TAB_CONTROL)
        KIND(Menubar, CEGUI_C_WIDGET_MENUBAR)
        KIND(PopupMenu, CEGUI_C_WIDGET_POPUP_MENU)
        KIND(MenuBase, CEGUI_C_WIDGET_MENU_BASE)
        KIND(ScrollablePane, CEGUI_C_WIDGET_SCROLLABLE_PANE)
        KIND(ScrolledContainer, CEGUI_C_WIDGET_SCROLLED_CONTAINER)
        KIND(ItemListbox, CEGUI_C_WIDGET_ITEM_LISTBOX)
        KIND(ScrolledItemListBase, CEGUI_C_WIDGET_SCROLLED_ITEM_LIST_BASE)
        KIND(ItemListBase, CEGUI_C_WIDGET_ITEM_LIST_BASE)
        KIND(ItemEntry, CEGUI_C_WIDGET_ITEM_ENTRY)
        KIND(GridLayoutContainer, CEGUI_C_WIDGET_GRID_LAYOUT_CONTAINER)
        KIND(HorizontalLayoutContainer, CEGUI_C_WIDGET_HORIZONTAL_LAYOUT_CONTAINER)
        KIND(VerticalLayoutContainer, CEGUI_C_WIDGET_VERTICAL_LAYOUT_CONTAINER)
        KIND(SequentialLayoutContainer, CEGUI_C_WIDGET_SEQUENTIAL_LAYOUT_CONTAINER)
        KIND(LayoutCell, CEGUI_C_WIDGET_LAYOUT_CELL)
        KIND(LayoutContainer, CEGUI_C_WIDGET_LAYOUT_CONTAINER)
        KIND(GroupBox, CEGUI_C_WIDGET_GROUP_BOX)
        KIND(DefaultWindow, CEGUI_C_WIDGET_DEFAULT_WINDOW)
#undef KIND
        *out_kind = CEGUI_C_WIDGET_WINDOW;
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Widget_GetComponent(
    CEGUI_C_Window window, CEGUI_C_WidgetComponent component, CEGUI_C_Window* out_window)
{
    CEGUI_WIDGET_BEGIN()
    {
        REQUIRE_OUT(out_window, "out_window");
        CEGUI::Window* result = nullptr;
        switch (component)
        {
        case CEGUI_C_COMPONENT_FRAME_TITLEBAR: result = checkedWidget<CEGUI::FrameWindow>(window, "FrameWindow")->getTitlebar(); break;
        case CEGUI_C_COMPONENT_FRAME_CLOSE_BUTTON: result = checkedWidget<CEGUI::FrameWindow>(window, "FrameWindow")->getCloseButton(); break;
        case CEGUI_C_COMPONENT_COMBOBOX_EDITBOX: result = checkedWidget<CEGUI::Combobox>(window, "Combobox")->getEditbox(); break;
        case CEGUI_C_COMPONENT_COMBOBOX_BUTTON: result = checkedWidget<CEGUI::Combobox>(window, "Combobox")->getPushButton(); break;
        case CEGUI_C_COMPONENT_COMBOBOX_DROP_LIST: result = checkedWidget<CEGUI::Combobox>(window, "Combobox")->getDropList(); break;
        case CEGUI_C_COMPONENT_SCROLLBAR_INCREASE_BUTTON: result = checkedWidget<CEGUI::Scrollbar>(window, "Scrollbar")->getIncreaseButton(); break;
        case CEGUI_C_COMPONENT_SCROLLBAR_DECREASE_BUTTON: result = checkedWidget<CEGUI::Scrollbar>(window, "Scrollbar")->getDecreaseButton(); break;
        case CEGUI_C_COMPONENT_SCROLLBAR_THUMB: result = checkedWidget<CEGUI::Scrollbar>(window, "Scrollbar")->getThumb(); break;
        case CEGUI_C_COMPONENT_SLIDER_THUMB: result = checkedWidget<CEGUI::Slider>(window, "Slider")->getThumb(); break;
        case CEGUI_C_COMPONENT_LISTBOX_VERTICAL_SCROLLBAR: result = checkedWidget<CEGUI::Listbox>(window, "Listbox")->getVertScrollbar(); break;
        case CEGUI_C_COMPONENT_LISTBOX_HORIZONTAL_SCROLLBAR: result = checkedWidget<CEGUI::Listbox>(window, "Listbox")->getHorzScrollbar(); break;
        case CEGUI_C_COMPONENT_MULTICOLUMN_LIST_HEADER: result = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getListHeader(); break;
        case CEGUI_C_COMPONENT_MULTICOLUMN_VERTICAL_SCROLLBAR: result = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getVertScrollbar(); break;
        case CEGUI_C_COMPONENT_MULTICOLUMN_HORIZONTAL_SCROLLBAR: result = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getHorzScrollbar(); break;
        case CEGUI_C_COMPONENT_MULTILINE_VERTICAL_SCROLLBAR: result = checkedWidget<CEGUI::MultiLineEditbox>(window, "MultiLineEditbox")->getVertScrollbar(); break;
        case CEGUI_C_COMPONENT_MULTILINE_HORIZONTAL_SCROLLBAR: result = checkedWidget<CEGUI::MultiLineEditbox>(window, "MultiLineEditbox")->getHorzScrollbar(); break;
        case CEGUI_C_COMPONENT_SCROLLABLE_CONTENT_PANE: result = const_cast<CEGUI::ScrolledContainer*>(checkedWidget<CEGUI::ScrollablePane>(window, "ScrollablePane")->getContentPane()); break;
        case CEGUI_C_COMPONENT_SCROLLABLE_VERTICAL_SCROLLBAR: result = checkedWidget<CEGUI::ScrollablePane>(window, "ScrollablePane")->getVertScrollbar(); break;
        case CEGUI_C_COMPONENT_SCROLLABLE_HORIZONTAL_SCROLLBAR: result = checkedWidget<CEGUI::ScrollablePane>(window, "ScrollablePane")->getHorzScrollbar(); break;
        case CEGUI_C_COMPONENT_GROUPBOX_CONTENT_PANE: result = checkedWidget<CEGUI::GroupBox>(window, "GroupBox")->getContentPane(); break;
        case CEGUI_C_COMPONENT_TOOLTIP_TARGET: result = const_cast<CEGUI::Window*>(checkedWidget<CEGUI::Tooltip>(window, "Tooltip")->getTargetWindow()); break;
        case CEGUI_C_COMPONENT_DRAG_DROP_TARGET: result = checkedWidget<CEGUI::DragContainer>(window, "DragContainer")->getCurrentDropTarget(); break;
        case CEGUI_C_COMPONENT_SPINNER_EDITBOX: result = checkedWidget<CEGUI::Spinner>(window, "Spinner")->getChild(CEGUI::Spinner::EditboxName); break;
        case CEGUI_C_COMPONENT_SPINNER_INCREASE_BUTTON: result = checkedWidget<CEGUI::Spinner>(window, "Spinner")->getChild(CEGUI::Spinner::IncreaseButtonName); break;
        case CEGUI_C_COMPONENT_SPINNER_DECREASE_BUTTON: result = checkedWidget<CEGUI::Spinner>(window, "Spinner")->getChild(CEGUI::Spinner::DecreaseButtonName); break;
        case CEGUI_C_COMPONENT_SCROLLED_ITEM_VERTICAL_SCROLLBAR: result = checkedWidget<CEGUI::ScrolledItemListBase>(window, "ScrolledItemListBase")->getVertScrollbar(); break;
        case CEGUI_C_COMPONENT_SCROLLED_ITEM_HORIZONTAL_SCROLLBAR: result = checkedWidget<CEGUI::ScrolledItemListBase>(window, "ScrolledItemListBase")->getHorzScrollbar(); break;
        case CEGUI_C_COMPONENT_TAB_BUTTON_PANE: result = checkedWidget<CEGUI::TabControl>(window, "TabControl")->getChild(CEGUI::TabControl::TabButtonPaneName); break;
        default: throw std::invalid_argument("Unknown widget component.");
        }
        *out_window = toWindow(result);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Widget_ExecuteAction(
    CEGUI_C_Window window, CEGUI_C_WidgetAction action)
{
    CEGUI_WIDGET_BEGIN()
    {
        switch (action)
        {
        case CEGUI_C_ACTION_PROGRESS_STEP: checkedWidget<CEGUI::ProgressBar>(window, "ProgressBar")->step(); break;
        case CEGUI_C_ACTION_FRAME_TOGGLE_ROLLUP: checkedWidget<CEGUI::FrameWindow>(window, "FrameWindow")->toggleRollup(); break;
        case CEGUI_C_ACTION_TOOLTIP_RESET_TIMER: checkedWidget<CEGUI::Tooltip>(window, "Tooltip")->resetTimer(); break;
        case CEGUI_C_ACTION_TOOLTIP_POSITION_SELF: checkedWidget<CEGUI::Tooltip>(window, "Tooltip")->positionSelf(); break;
        case CEGUI_C_ACTION_TOOLTIP_SIZE_SELF: checkedWidget<CEGUI::Tooltip>(window, "Tooltip")->sizeSelf(); break;
        case CEGUI_C_ACTION_COMBOBOX_SHOW_DROP_LIST: checkedWidget<CEGUI::Combobox>(window, "Combobox")->showDropList(); break;
        case CEGUI_C_ACTION_COMBOBOX_HIDE_DROP_LIST: checkedWidget<CEGUI::Combobox>(window, "Combobox")->hideDropList(); break;
        case CEGUI_C_ACTION_LISTBOX_RESET: checkedWidget<CEGUI::Listbox>(window, "Listbox")->resetList(); break;
        case CEGUI_C_ACTION_LISTBOX_CLEAR_SELECTIONS: checkedWidget<CEGUI::Listbox>(window, "Listbox")->clearAllSelections(); break;
        case CEGUI_C_ACTION_COMBOBOX_RESET_LIST: checkedWidget<CEGUI::Combobox>(window, "Combobox")->resetList(); break;
        case CEGUI_C_ACTION_COMBOBOX_CLEAR_SELECTIONS: checkedWidget<CEGUI::Combobox>(window, "Combobox")->clearAllSelections(); break;
        case CEGUI_C_ACTION_MULTICOLUMN_RESET: checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->resetList(); break;
        case CEGUI_C_ACTION_MULTICOLUMN_CLEAR_SELECTIONS: checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->clearAllSelections(); break;
        case CEGUI_C_ACTION_TREE_RESET: checkedWidget<CEGUI::Tree>(window, "Tree")->resetList(); break;
        case CEGUI_C_ACTION_TREE_CLEAR_SELECTIONS: checkedWidget<CEGUI::Tree>(window, "Tree")->clearAllSelections(); break;
        case CEGUI_C_ACTION_ITEMLIST_RESET: checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->resetList(); break;
        case CEGUI_C_ACTION_ITEMLISTBOX_CLEAR_SELECTIONS: checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->clearAllSelections(); break;
        case CEGUI_C_ACTION_ITEMLISTBOX_SELECT_ALL: checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->selectAllItems(); break;
        case CEGUI_C_ACTION_LAYOUT_MARK_DIRTY: checkedWidget<CEGUI::LayoutContainer>(window, "LayoutContainer")->markNeedsLayouting(); break;
        case CEGUI_C_ACTION_LAYOUT_NOW: checkedWidget<CEGUI::LayoutContainer>(window, "LayoutContainer")->layout(); break;
        case CEGUI_C_ACTION_LAYOUT_IF_NEEDED: checkedWidget<CEGUI::LayoutContainer>(window, "LayoutContainer")->layoutIfNecessary(); break;
        case CEGUI_C_ACTION_POPUP_OPEN: checkedWidget<CEGUI::PopupMenu>(window, "PopupMenu")->openPopupMenu(); break;
        case CEGUI_C_ACTION_POPUP_CLOSE: checkedWidget<CEGUI::PopupMenu>(window, "PopupMenu")->closePopupMenu(); break;
        case CEGUI_C_ACTION_MENUITEM_OPEN_POPUP: checkedWidget<CEGUI::MenuItem>(window, "MenuItem")->openPopupMenu(); break;
        case CEGUI_C_ACTION_MENUITEM_CLOSE_POPUP: checkedWidget<CEGUI::MenuItem>(window, "MenuItem")->closePopupMenu(); break;
        default: throw std::invalid_argument("Unknown widget action.");
        }
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxTextItem_Create(
    const char* text, uint32_t id, CEGUI_C_Bool disabled, CEGUI_C_Bool auto_delete,
    CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN()
    {
        REQUIRE_OUT(out_item, "out_item");
        *out_item = toListboxItem(CEGUI_NEW_AO CEGUI::ListboxTextItem(
            fromUtf8(text), id, nullptr, disabled != 0, auto_delete != 0));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_Destroy(CEGUI_C_ListboxItem itemHandle)
{
    CEGUI_WIDGET_BEGIN()
    {
        CEGUI::ListboxItem* item = checkedListboxItem(itemHandle);
        if (item->getOwnerWindow())
            throw std::invalid_argument("Cannot manually destroy a ListboxItem while it belongs to a widget. Remove it first.");
        CEGUI_DELETE_AO(item);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetText(CEGUI_C_ListboxItem item, const char** out_text)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_text, "out_text"); *out_text = storeReturnString(checkedListboxItem(item)->getText()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetText(CEGUI_C_ListboxItem item, const char* text)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setText(fromUtf8(text)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetID(CEGUI_C_ListboxItem item, uint32_t* out_id)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_id, "out_id"); *out_id = checkedListboxItem(item)->getID(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetID(CEGUI_C_ListboxItem item, uint32_t id)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setID(id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetUserData(CEGUI_C_ListboxItem item, uintptr_t* out_data)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_data, "out_data"); *out_data = reinterpret_cast<uintptr_t>(checkedListboxItem(item)->getUserData()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetUserData(CEGUI_C_ListboxItem item, uintptr_t data)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setUserData(reinterpret_cast<void*>(data)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsSelected(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedListboxItem(item)->isSelected(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetSelected(CEGUI_C_ListboxItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setSelected(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsDisabled(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedListboxItem(item)->isDisabled(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetDisabled(CEGUI_C_ListboxItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setDisabled(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsAutoDeleted(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedListboxItem(item)->isAutoDeleted(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetAutoDeleted(CEGUI_C_ListboxItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setAutoDeleted(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxTextItem_SetFont(CEGUI_C_ListboxItem item, const char* font_name)
{
    CEGUI_WIDGET_BEGIN()
    {
        auto* textItem = dynamic_cast<CEGUI::ListboxTextItem*>(checkedListboxItem(item));
        if (!textItem) throw std::invalid_argument("ListboxItem is not a ListboxTextItem.");
        textItem->setFont(fromUtf8(font_name));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetSelectionBrushImage(CEGUI_C_ListboxItem item, const char* image_name)
{
    CEGUI_WIDGET_BEGIN() { checkedListboxItem(item)->setSelectionBrushImage(fromUtf8(image_name)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetItemCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::Listbox>(window, "Listbox")->getItemCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetSelectedCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::Listbox>(window, "Listbox")->getSelectedCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::Listbox>(window, "Listbox")->getListboxItemFromIndex(index)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::Listbox>(window, "Listbox")->getFirstSelectedItem()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetNextSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem start, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::Listbox>(window, "Listbox")->getNextSelected(asListboxItem(start))); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_AddItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Listbox>(window, "Listbox")->addItem(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_InsertItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_ListboxItem before)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Listbox>(window, "Listbox")->insertItem(checkedListboxItem(item), asListboxItem(before)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_RemoveItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Listbox>(window, "Listbox")->removeItem(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Listbox>(window, "Listbox")->setItemSelectState(checkedListboxItem(item), selected != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Listbox>(window, "Listbox")->ensureItemIsVisible(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetItemCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::Combobox>(window, "Combobox")->getItemCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::Combobox>(window, "Combobox")->getListboxItemFromIndex(index)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetSelectedItem(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::Combobox>(window, "Combobox")->getSelectedItem()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_AddItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Combobox>(window, "Combobox")->addItem(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_InsertItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_ListboxItem before)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Combobox>(window, "Combobox")->insertItem(checkedListboxItem(item), asListboxItem(before)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_RemoveItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Combobox>(window, "Combobox")->removeItem(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Combobox>(window, "Combobox")->setItemSelectState(checkedListboxItem(item), selected != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnCount(CEGUI_C_Window window, uint32_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getColumnCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSegmentAt(CEGUI_C_Window window, uint32_t column, CEGUI_C_Window* out_segment)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_segment, "out_segment"); *out_segment = toWindow(&checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getSegmentFromColumn(column)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSegmentByID(CEGUI_C_Window window, uint32_t id, CEGUI_C_Window* out_segment)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_segment, "out_segment"); *out_segment = toWindow(&checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getSegmentFromID(id)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnFromID(CEGUI_C_Window window, uint32_t id, uint32_t* out_column)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_column, "out_column"); *out_column = checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getColumnFromID(id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnWidth(CEGUI_C_Window window, uint32_t column, CEGUI_C_UDim* out_width)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_width, "out_width"); *out_width = fromUDim(checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getColumnWidth(column)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_AddColumn(CEGUI_C_Window window, const char* text, uint32_t id, CEGUI_C_UDim width)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->addColumn(fromUtf8(text), id, toUDim(width)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_InsertColumn(CEGUI_C_Window window, const char* text, uint32_t id, CEGUI_C_UDim width, uint32_t position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->insertColumn(fromUtf8(text), id, toUDim(width), position); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_RemoveColumn(CEGUI_C_Window window, uint32_t column)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->removeColumn(column); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_MoveColumn(CEGUI_C_Window window, uint32_t column, uint32_t position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->moveColumn(column, position); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSortColumn(CEGUI_C_Window window, uint32_t* out_column)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_column, "out_column"); *out_column = checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getSortColumn(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_SetSortColumn(CEGUI_C_Window window, uint32_t column)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->setSortColumn(column); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection* out_direction)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_direction, "out_direction"); *out_direction = static_cast<CEGUI_C_ListSortDirection>(checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->getSortDirection()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_SetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection direction)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ListHeader>(window, "ListHeader")->setSortDirection(static_cast<CEGUI::ListHeaderSegment::SortDirection>(direction)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetColumnCount(CEGUI_C_Window window, uint32_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getColumnCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetRowCount(CEGUI_C_Window window, uint32_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getRowCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_AddColumn(CEGUI_C_Window window, const char* text, uint32_t id, CEGUI_C_UDim width)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->addColumn(fromUtf8(text), id, toUDim(width)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_InsertColumn(CEGUI_C_Window window, const char* text, uint32_t id, CEGUI_C_UDim width, uint32_t position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->insertColumn(fromUtf8(text), id, toUDim(width), position); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_RemoveColumn(CEGUI_C_Window window, uint32_t column)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->removeColumn(column); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_AddRow(CEGUI_C_Window window, uint32_t row_id, uint32_t* out_index)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_index, "out_index"); *out_index = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->addRow(row_id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_InsertRow(CEGUI_C_Window window, uint32_t row_index, uint32_t row_id, uint32_t* out_actual)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_actual, "out_actual"); *out_actual = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->insertRow(row_index, row_id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_RemoveRow(CEGUI_C_Window window, uint32_t row)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->removeRow(row); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, uint32_t column_id, uint32_t row)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->setItem(checkedListboxItem(item), column_id, row); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetItem(CEGUI_C_Window window, CEGUI_C_MCLGridRef position, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getItemAtGridReference(toGridRef(position))); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSelectedCount(CEGUI_C_Window window, uint32_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getSelectedCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getFirstSelectedItem()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetNextSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem start, CEGUI_C_ListboxItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toListboxItem(checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getNextSelected(asListboxItem(start))); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->setItemSelectState(checkedListboxItem(item), selected != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_ListboxItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->ensureItemIsVisible(checkedListboxItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSelectionMode(CEGUI_C_Window window, CEGUI_C_MCLSelectionMode* out_mode)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_mode, "out_mode"); *out_mode = static_cast<CEGUI_C_MCLSelectionMode>(checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getSelectionMode()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSelectionMode(CEGUI_C_Window window, CEGUI_C_MCLSelectionMode mode)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->setSelectionMode(static_cast<CEGUI::MultiColumnList::SelectionMode>(mode)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSortColumn(CEGUI_C_Window window, uint32_t* out_column)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_column, "out_column"); *out_column = checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getSortColumn(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSortColumn(CEGUI_C_Window window, uint32_t column)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->setSortColumn(column); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection* out_direction)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_direction, "out_direction"); *out_direction = static_cast<CEGUI_C_ListSortDirection>(checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->getSortDirection()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection direction)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MultiColumnList>(window, "MultiColumnList")->setSortDirection(static_cast<CEGUI::ListHeaderSegment::SortDirection>(direction)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_GetItemCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->getItemCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_Window* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toWindow(checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->getItemFromIndex(index)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_AddItem(CEGUI_C_Window window, CEGUI_C_Window item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->addItem(checkedWidget<CEGUI::ItemEntry>(item, "ItemEntry")); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_InsertItem(CEGUI_C_Window window, CEGUI_C_Window item, CEGUI_C_Window before)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->insertItem(checkedWidget<CEGUI::ItemEntry>(item, "ItemEntry"), before ? checkedWidget<CEGUI::ItemEntry>(before, "ItemEntry") : nullptr); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_RemoveItem(CEGUI_C_Window window, CEGUI_C_Window item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ItemListBase>(window, "ItemListBase")->removeItem(checkedWidget<CEGUI::ItemEntry>(item, "ItemEntry")); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetSelectedCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->getSelectedCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetFirstSelected(CEGUI_C_Window window, size_t start, CEGUI_C_Window* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toWindow(checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->getFirstSelectedItem(start)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetNextSelected(CEGUI_C_Window window, CEGUI_C_Window start, CEGUI_C_Window* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toWindow(checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->getNextSelectedItemAfter(checkedWidget<CEGUI::ItemEntry>(start, "ItemEntry"))); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_SelectRange(CEGUI_C_Window window, size_t first, size_t last)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::ItemListbox>(window, "ItemListbox")->selectRange(first, last); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_Create(const char* text, uint32_t id, CEGUI_C_Bool disabled, CEGUI_C_Bool auto_delete, CEGUI_C_TreeItem* out_item)
{
    CEGUI_WIDGET_BEGIN()
    {
        REQUIRE_OUT(out_item, "out_item");
        *out_item = toTreeItem(CEGUI_NEW_AO CEGUI::TreeItem(fromUtf8(text), id, nullptr, disabled != 0, auto_delete != 0));
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_Destroy(CEGUI_C_TreeItem itemHandle)
{
    CEGUI_WIDGET_BEGIN()
    {
        CEGUI::TreeItem* item = checkedTreeItem(itemHandle);
        if (item->getOwnerWindow())
            throw std::invalid_argument("Cannot manually destroy a TreeItem while it belongs to a widget. Remove it first.");
        CEGUI_DELETE_AO(item);
        return CEGUI_C_RESULT_SUCCESS;
    }
    CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetText(CEGUI_C_TreeItem item, const char** out_text)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_text, "out_text"); *out_text = storeReturnString(checkedTreeItem(item)->getText()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetText(CEGUI_C_TreeItem item, const char* text)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setText(fromUtf8(text)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetID(CEGUI_C_TreeItem item, uint32_t* out_id)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_id, "out_id"); *out_id = checkedTreeItem(item)->getID(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetID(CEGUI_C_TreeItem item, uint32_t id)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setID(id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetUserData(CEGUI_C_TreeItem item, uintptr_t* out_data)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_data, "out_data"); *out_data = reinterpret_cast<uintptr_t>(checkedTreeItem(item)->getUserData()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetUserData(CEGUI_C_TreeItem item, uintptr_t data)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setUserData(reinterpret_cast<void*>(data)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsSelected(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedTreeItem(item)->isSelected(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetSelected(CEGUI_C_TreeItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setSelected(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsDisabled(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedTreeItem(item)->isDisabled(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetDisabled(CEGUI_C_TreeItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setDisabled(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsAutoDeleted(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedTreeItem(item)->isAutoDeleted(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetAutoDeleted(CEGUI_C_TreeItem item, CEGUI_C_Bool value)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->setAutoDeleted(value != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsOpen(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_value, "out_value"); *out_value = checkedTreeItem(item)->getIsOpen(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_ToggleOpen(CEGUI_C_TreeItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->toggleIsOpen(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetChildCount(CEGUI_C_TreeItem item, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedTreeItem(item)->getItemCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetChildAt(CEGUI_C_TreeItem item, size_t index, CEGUI_C_TreeItem* out_child)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_child, "out_child"); *out_child = toTreeItem(checkedTreeItem(item)->getTreeItemFromIndex(index)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_AddChild(CEGUI_C_TreeItem item, CEGUI_C_TreeItem child)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->addItem(checkedTreeItem(child)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_RemoveChild(CEGUI_C_TreeItem item, CEGUI_C_TreeItem child)
{
    CEGUI_WIDGET_BEGIN() { checkedTreeItem(item)->removeItem(checkedTreeItem(child)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetSelectedCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::Tree>(window, "Tree")->getSelectedCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_TreeItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toTreeItem(checkedWidget<CEGUI::Tree>(window, "Tree")->getFirstSelectedItem()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetNextSelected(CEGUI_C_Window window, CEGUI_C_TreeItem start, CEGUI_C_TreeItem* out_item)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_item, "out_item"); *out_item = toTreeItem(checkedWidget<CEGUI::Tree>(window, "Tree")->getNextSelected(asTreeItem(start))); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_AddItem(CEGUI_C_Window window, CEGUI_C_TreeItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Tree>(window, "Tree")->addItem(checkedTreeItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_InsertItem(CEGUI_C_Window window, CEGUI_C_TreeItem item, CEGUI_C_TreeItem before)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Tree>(window, "Tree")->insertItem(checkedTreeItem(item), asTreeItem(before)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_RemoveItem(CEGUI_C_Window window, CEGUI_C_TreeItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Tree>(window, "Tree")->removeItem(checkedTreeItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_SetItemSelected(CEGUI_C_Window window, CEGUI_C_TreeItem item, CEGUI_C_Bool selected)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Tree>(window, "Tree")->setItemSelectState(checkedTreeItem(item), selected != 0); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_TreeItem item)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::Tree>(window, "Tree")->ensureItemIsVisible(checkedTreeItem(item)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetTabCount(CEGUI_C_Window window, size_t* out_count)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_count, "out_count"); *out_count = checkedWidget<CEGUI::TabControl>(window, "TabControl")->getTabCount(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetSelectedIndex(CEGUI_C_Window window, size_t* out_index)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_index, "out_index"); *out_index = checkedWidget<CEGUI::TabControl>(window, "TabControl")->getSelectedTabIndex(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_SetSelectedIndex(CEGUI_C_Window window, size_t index)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->setSelectedTabAtIndex(index); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetContentAt(CEGUI_C_Window window, size_t index, CEGUI_C_Window* out_content)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_content, "out_content"); *out_content = toWindow(checkedWidget<CEGUI::TabControl>(window, "TabControl")->getTabContentsAtIndex(index)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_AddTab(CEGUI_C_Window window, CEGUI_C_Window content)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->addTab(asWindow(content)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_RemoveTabByName(CEGUI_C_Window window, const char* name)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->removeTab(fromUtf8(name)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_RemoveTabByID(CEGUI_C_Window window, uint32_t id)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->removeTab(id); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_MakeVisible(CEGUI_C_Window window, size_t index)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->makeTabVisibleAtIndex(index); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetPanePosition(CEGUI_C_Window window, CEGUI_C_TabPanePosition* out_position)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_position, "out_position"); *out_position = static_cast<CEGUI_C_TabPanePosition>(checkedWidget<CEGUI::TabControl>(window, "TabControl")->getTabPanePosition()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_SetPanePosition(CEGUI_C_Window window, CEGUI_C_TabPanePosition position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::TabControl>(window, "TabControl")->setTabPanePosition(static_cast<CEGUI::TabControl::TabPanePosition>(position)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_GetPopupMenu(CEGUI_C_Window window, CEGUI_C_Window* out_popup)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_popup, "out_popup"); *out_popup = toWindow(checkedWidget<CEGUI::MenuItem>(window, "MenuItem")->getPopupMenu()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_SetPopupMenu(CEGUI_C_Window window, CEGUI_C_Window popup)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::MenuItem>(window, "MenuItem")->setPopupMenu(popup ? checkedWidget<CEGUI::PopupMenu>(popup, "PopupMenu") : nullptr); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_TogglePopupMenu(CEGUI_C_Window window, CEGUI_C_Bool* out_opened)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_opened, "out_opened"); *out_opened = checkedWidget<CEGUI::MenuItem>(window, "MenuItem")->togglePopupMenu(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_GetPosition(CEGUI_C_Window window, CEGUI_C_Window child, size_t* out_position)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_position, "out_position"); *out_position = checkedWidget<CEGUI::SequentialLayoutContainer>(window, "SequentialLayoutContainer")->getPositionOfChild(asWindow(child)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_GetChildAt(CEGUI_C_Window window, size_t position, CEGUI_C_Window* out_child)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_child, "out_child"); *out_child = toWindow(checkedWidget<CEGUI::SequentialLayoutContainer>(window, "SequentialLayoutContainer")->getChildAtPosition(position)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_MoveChildTo(CEGUI_C_Window window, CEGUI_C_Window child, size_t position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::SequentialLayoutContainer>(window, "SequentialLayoutContainer")->moveChildToPosition(asWindow(child), position); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_AddChildAt(CEGUI_C_Window window, CEGUI_C_Window child, size_t position)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::SequentialLayoutContainer>(window, "SequentialLayoutContainer")->addChildToPosition(asWindow(child), position); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_SetDimensions(CEGUI_C_Window window, size_t width, size_t height)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer")->setGridDimensions(width, height); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetDimensions(CEGUI_C_Window window, size_t* out_width, size_t* out_height)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_width, "out_width"); REQUIRE_OUT(out_height, "out_height"); auto* grid = checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer"); *out_width = grid->getGridWidth(); *out_height = grid->getGridHeight(); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetAutoPositioning(CEGUI_C_Window window, CEGUI_C_GridAutoPositioning* out_mode)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_mode, "out_mode"); *out_mode = static_cast<CEGUI_C_GridAutoPositioning>(checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer")->getAutoPositioning()); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_SetAutoPositioning(CEGUI_C_Window window, CEGUI_C_GridAutoPositioning mode)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer")->setAutoPositioning(static_cast<CEGUI::GridLayoutContainer::AutoPositioning>(mode)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetChildAt(CEGUI_C_Window window, size_t x, size_t y, CEGUI_C_Window* out_child)
{
    CEGUI_WIDGET_BEGIN() { REQUIRE_OUT(out_child, "out_child"); *out_child = toWindow(checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer")->getChildAtPosition(x, y)); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}
CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_AddChildAt(CEGUI_C_Window window, CEGUI_C_Window child, size_t x, size_t y)
{
    CEGUI_WIDGET_BEGIN() { checkedWidget<CEGUI::GridLayoutContainer>(window, "GridLayoutContainer")->addChildToPosition(asWindow(child), x, y); return CEGUI_C_RESULT_SUCCESS; } CEGUI_WIDGET_END()
}

} // extern "C"
