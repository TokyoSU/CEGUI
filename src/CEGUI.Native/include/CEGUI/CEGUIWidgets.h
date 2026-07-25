#ifndef CEGUI_C_WIDGET_BINDING_H
#define CEGUI_C_WIDGET_BINDING_H

#include "CEGUIC.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CEGUI_C_ListboxItem_t* CEGUI_C_ListboxItem;
typedef struct CEGUI_C_TreeItem_t* CEGUI_C_TreeItem;

typedef enum CEGUI_C_WidgetKind
{
    CEGUI_C_WIDGET_WINDOW = 0,
    CEGUI_C_WIDGET_DEFAULT_WINDOW,
    CEGUI_C_WIDGET_BUTTON_BASE,
    CEGUI_C_WIDGET_PUSH_BUTTON,
    CEGUI_C_WIDGET_TOGGLE_BUTTON,
    CEGUI_C_WIDGET_RADIO_BUTTON,
    CEGUI_C_WIDGET_PROGRESS_BAR,
    CEGUI_C_WIDGET_EDITBOX,
    CEGUI_C_WIDGET_MULTI_LINE_EDITBOX,
    CEGUI_C_WIDGET_FRAME_WINDOW,
    CEGUI_C_WIDGET_TITLEBAR,
    CEGUI_C_WIDGET_TOOLTIP,
    CEGUI_C_WIDGET_DRAG_CONTAINER,
    CEGUI_C_WIDGET_CLIPPED_CONTAINER,
    CEGUI_C_WIDGET_SCROLLBAR,
    CEGUI_C_WIDGET_SLIDER,
    CEGUI_C_WIDGET_THUMB,
    CEGUI_C_WIDGET_SPINNER,
    CEGUI_C_WIDGET_COMBOBOX,
    CEGUI_C_WIDGET_COMBO_DROP_LIST,
    CEGUI_C_WIDGET_LISTBOX,
    CEGUI_C_WIDGET_MULTI_COLUMN_LIST,
    CEGUI_C_WIDGET_LIST_HEADER,
    CEGUI_C_WIDGET_LIST_HEADER_SEGMENT,
    CEGUI_C_WIDGET_TREE,
    CEGUI_C_WIDGET_TAB_CONTROL,
    CEGUI_C_WIDGET_TAB_BUTTON,
    CEGUI_C_WIDGET_MENU_BASE,
    CEGUI_C_WIDGET_MENUBAR,
    CEGUI_C_WIDGET_POPUP_MENU,
    CEGUI_C_WIDGET_MENU_ITEM,
    CEGUI_C_WIDGET_SCROLLABLE_PANE,
    CEGUI_C_WIDGET_SCROLLED_CONTAINER,
    CEGUI_C_WIDGET_LAYOUT_CONTAINER,
    CEGUI_C_WIDGET_LAYOUT_CELL,
    CEGUI_C_WIDGET_SEQUENTIAL_LAYOUT_CONTAINER,
    CEGUI_C_WIDGET_HORIZONTAL_LAYOUT_CONTAINER,
    CEGUI_C_WIDGET_VERTICAL_LAYOUT_CONTAINER,
    CEGUI_C_WIDGET_GRID_LAYOUT_CONTAINER,
    CEGUI_C_WIDGET_ITEM_ENTRY,
    CEGUI_C_WIDGET_ITEM_LIST_BASE,
    CEGUI_C_WIDGET_SCROLLED_ITEM_LIST_BASE,
    CEGUI_C_WIDGET_ITEM_LISTBOX,
    CEGUI_C_WIDGET_GROUP_BOX,
    CEGUI_C_WIDGET_ROUND_IMAGE,
    CEGUI_C_WIDGET_SLOT_IMAGE_ITEM
} CEGUI_C_WidgetKind;

typedef enum CEGUI_C_WidgetComponent
{
    CEGUI_C_COMPONENT_FRAME_TITLEBAR = 0,
    CEGUI_C_COMPONENT_FRAME_CLOSE_BUTTON,
    CEGUI_C_COMPONENT_COMBOBOX_EDITBOX,
    CEGUI_C_COMPONENT_COMBOBOX_BUTTON,
    CEGUI_C_COMPONENT_COMBOBOX_DROP_LIST,
    CEGUI_C_COMPONENT_SCROLLBAR_INCREASE_BUTTON,
    CEGUI_C_COMPONENT_SCROLLBAR_DECREASE_BUTTON,
    CEGUI_C_COMPONENT_SCROLLBAR_THUMB,
    CEGUI_C_COMPONENT_SLIDER_THUMB,
    CEGUI_C_COMPONENT_LISTBOX_VERTICAL_SCROLLBAR,
    CEGUI_C_COMPONENT_LISTBOX_HORIZONTAL_SCROLLBAR,
    CEGUI_C_COMPONENT_MULTICOLUMN_LIST_HEADER,
    CEGUI_C_COMPONENT_MULTICOLUMN_VERTICAL_SCROLLBAR,
    CEGUI_C_COMPONENT_MULTICOLUMN_HORIZONTAL_SCROLLBAR,
    CEGUI_C_COMPONENT_MULTILINE_VERTICAL_SCROLLBAR,
    CEGUI_C_COMPONENT_MULTILINE_HORIZONTAL_SCROLLBAR,
    CEGUI_C_COMPONENT_SCROLLABLE_CONTENT_PANE,
    CEGUI_C_COMPONENT_SCROLLABLE_VERTICAL_SCROLLBAR,
    CEGUI_C_COMPONENT_SCROLLABLE_HORIZONTAL_SCROLLBAR,
    CEGUI_C_COMPONENT_GROUPBOX_CONTENT_PANE,
    CEGUI_C_COMPONENT_TOOLTIP_TARGET,
    CEGUI_C_COMPONENT_DRAG_DROP_TARGET,
    CEGUI_C_COMPONENT_SPINNER_EDITBOX,
    CEGUI_C_COMPONENT_SPINNER_INCREASE_BUTTON,
    CEGUI_C_COMPONENT_SPINNER_DECREASE_BUTTON,
    CEGUI_C_COMPONENT_SCROLLED_ITEM_VERTICAL_SCROLLBAR,
    CEGUI_C_COMPONENT_SCROLLED_ITEM_HORIZONTAL_SCROLLBAR,
    CEGUI_C_COMPONENT_TAB_BUTTON_PANE
} CEGUI_C_WidgetComponent;

typedef enum CEGUI_C_WidgetAction
{
    CEGUI_C_ACTION_PROGRESS_STEP = 0,
    CEGUI_C_ACTION_FRAME_TOGGLE_ROLLUP,
    CEGUI_C_ACTION_TOOLTIP_RESET_TIMER,
    CEGUI_C_ACTION_TOOLTIP_POSITION_SELF,
    CEGUI_C_ACTION_TOOLTIP_SIZE_SELF,
    CEGUI_C_ACTION_COMBOBOX_SHOW_DROP_LIST,
    CEGUI_C_ACTION_COMBOBOX_HIDE_DROP_LIST,
    CEGUI_C_ACTION_LISTBOX_RESET,
    CEGUI_C_ACTION_LISTBOX_CLEAR_SELECTIONS,
    CEGUI_C_ACTION_COMBOBOX_RESET_LIST,
    CEGUI_C_ACTION_COMBOBOX_CLEAR_SELECTIONS,
    CEGUI_C_ACTION_MULTICOLUMN_RESET,
    CEGUI_C_ACTION_MULTICOLUMN_CLEAR_SELECTIONS,
    CEGUI_C_ACTION_TREE_RESET,
    CEGUI_C_ACTION_TREE_CLEAR_SELECTIONS,
    CEGUI_C_ACTION_ITEMLIST_RESET,
    CEGUI_C_ACTION_ITEMLISTBOX_CLEAR_SELECTIONS,
    CEGUI_C_ACTION_ITEMLISTBOX_SELECT_ALL,
    CEGUI_C_ACTION_LAYOUT_MARK_DIRTY,
    CEGUI_C_ACTION_LAYOUT_NOW,
    CEGUI_C_ACTION_LAYOUT_IF_NEEDED,
    CEGUI_C_ACTION_POPUP_OPEN,
    CEGUI_C_ACTION_POPUP_CLOSE,
    CEGUI_C_ACTION_MENUITEM_OPEN_POPUP,
    CEGUI_C_ACTION_MENUITEM_CLOSE_POPUP
} CEGUI_C_WidgetAction;

typedef enum CEGUI_C_MCLSelectionMode
{
    CEGUI_C_MCL_ROW_SINGLE = 0,
    CEGUI_C_MCL_ROW_MULTIPLE = 1,
    CEGUI_C_MCL_CELL_SINGLE = 2,
    CEGUI_C_MCL_CELL_MULTIPLE = 3,
    CEGUI_C_MCL_NOMINATED_COLUMN_SINGLE = 4,
    CEGUI_C_MCL_NOMINATED_COLUMN_MULTIPLE = 5,
    CEGUI_C_MCL_COLUMN_SINGLE = 6,
    CEGUI_C_MCL_COLUMN_MULTIPLE = 7,
    CEGUI_C_MCL_NOMINATED_ROW_SINGLE = 8,
    CEGUI_C_MCL_NOMINATED_ROW_MULTIPLE = 9
} CEGUI_C_MCLSelectionMode;

typedef enum CEGUI_C_ListSortDirection
{
    CEGUI_C_LIST_SORT_NONE = 0,
    CEGUI_C_LIST_SORT_ASCENDING = 1,
    CEGUI_C_LIST_SORT_DESCENDING = 2
} CEGUI_C_ListSortDirection;

typedef enum CEGUI_C_TabPanePosition
{
    CEGUI_C_TAB_PANE_TOP = 0,
    CEGUI_C_TAB_PANE_BOTTOM = 1
} CEGUI_C_TabPanePosition;

typedef enum CEGUI_C_GridAutoPositioning
{
    CEGUI_C_GRID_AUTO_DISABLED = 0,
    CEGUI_C_GRID_AUTO_LEFT_TO_RIGHT = 1,
    CEGUI_C_GRID_AUTO_TOP_TO_BOTTOM = 2
} CEGUI_C_GridAutoPositioning;

typedef struct CEGUI_C_MCLGridRef
{
    uint32_t row;
    uint32_t column;
} CEGUI_C_MCLGridRef;

/* Widget discovery and common sub-components. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Window_GetWidgetKind(CEGUI_C_Window window, CEGUI_C_WidgetKind* out_kind);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Widget_GetComponent(CEGUI_C_Window window, CEGUI_C_WidgetComponent component, CEGUI_C_Window* out_window);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Widget_ExecuteAction(CEGUI_C_Window window, CEGUI_C_WidgetAction action);

/* ListboxTextItem. Items created with auto_delete=false remain caller-owned. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxTextItem_Create(const char* text_utf8, uint32_t id, CEGUI_C_Bool disabled, CEGUI_C_Bool auto_delete, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_Destroy(CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetText(CEGUI_C_ListboxItem item, const char** out_text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetText(CEGUI_C_ListboxItem item, const char* text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetID(CEGUI_C_ListboxItem item, uint32_t* out_id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetID(CEGUI_C_ListboxItem item, uint32_t id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_GetUserData(CEGUI_C_ListboxItem item, uintptr_t* out_data);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetUserData(CEGUI_C_ListboxItem item, uintptr_t data);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsSelected(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetSelected(CEGUI_C_ListboxItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsDisabled(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetDisabled(CEGUI_C_ListboxItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_IsAutoDeleted(CEGUI_C_ListboxItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetAutoDeleted(CEGUI_C_ListboxItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxTextItem_SetFont(CEGUI_C_ListboxItem item, const char* font_name_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListboxItem_SetSelectionBrushImage(CEGUI_C_ListboxItem item, const char* image_name_utf8);

/* Listbox and Combobox item collections. Returned items are borrowed. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetItemCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetSelectedCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_GetNextSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem start_item, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_AddItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_InsertItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_ListboxItem before_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_RemoveItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Listbox_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetItemCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_GetSelectedItem(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_AddItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_InsertItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_ListboxItem before_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_RemoveItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Combobox_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected);

/* Standalone ListHeader column management. Returned segments are borrowed windows. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnCount(CEGUI_C_Window window, uint32_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSegmentAt(CEGUI_C_Window window, uint32_t column, CEGUI_C_Window* out_segment);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSegmentByID(CEGUI_C_Window window, uint32_t id, CEGUI_C_Window* out_segment);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnFromID(CEGUI_C_Window window, uint32_t id, uint32_t* out_column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetColumnWidth(CEGUI_C_Window window, uint32_t column, CEGUI_C_UDim* out_width);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_AddColumn(CEGUI_C_Window window, const char* text_utf8, uint32_t id, CEGUI_C_UDim width);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_InsertColumn(CEGUI_C_Window window, const char* text_utf8, uint32_t id, CEGUI_C_UDim width, uint32_t position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_RemoveColumn(CEGUI_C_Window window, uint32_t column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_MoveColumn(CEGUI_C_Window window, uint32_t column, uint32_t position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSortColumn(CEGUI_C_Window window, uint32_t* out_column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_SetSortColumn(CEGUI_C_Window window, uint32_t column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_GetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection* out_direction);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ListHeader_SetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection direction);

/* MultiColumnList. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetColumnCount(CEGUI_C_Window window, uint32_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetRowCount(CEGUI_C_Window window, uint32_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_AddColumn(CEGUI_C_Window window, const char* text_utf8, uint32_t column_id, CEGUI_C_UDim width);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_InsertColumn(CEGUI_C_Window window, const char* text_utf8, uint32_t column_id, CEGUI_C_UDim width, uint32_t position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_RemoveColumn(CEGUI_C_Window window, uint32_t column_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_AddRow(CEGUI_C_Window window, uint32_t row_id, uint32_t* out_row_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_InsertRow(CEGUI_C_Window window, uint32_t row_index, uint32_t row_id, uint32_t* out_actual_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_RemoveRow(CEGUI_C_Window window, uint32_t row_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetItem(CEGUI_C_Window window, CEGUI_C_ListboxItem item, uint32_t column_id, uint32_t row_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetItem(CEGUI_C_Window window, CEGUI_C_MCLGridRef position, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSelectedCount(CEGUI_C_Window window, uint32_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetNextSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem start_item, CEGUI_C_ListboxItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetItemSelected(CEGUI_C_Window window, CEGUI_C_ListboxItem item, CEGUI_C_Bool selected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_ListboxItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSelectionMode(CEGUI_C_Window window, CEGUI_C_MCLSelectionMode* out_mode);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSelectionMode(CEGUI_C_Window window, CEGUI_C_MCLSelectionMode mode);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSortColumn(CEGUI_C_Window window, uint32_t* out_column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSortColumn(CEGUI_C_Window window, uint32_t column);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_GetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection* out_direction);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MultiColumnList_SetSortDirection(CEGUI_C_Window window, CEGUI_C_ListSortDirection direction);

/* ItemEntry / ItemListBase / ItemListbox use Window handles for their items. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_GetItemCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_GetItemAt(CEGUI_C_Window window, size_t index, CEGUI_C_Window* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_AddItem(CEGUI_C_Window window, CEGUI_C_Window item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_InsertItem(CEGUI_C_Window window, CEGUI_C_Window item, CEGUI_C_Window before_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListBase_RemoveItem(CEGUI_C_Window window, CEGUI_C_Window item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetSelectedCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetFirstSelected(CEGUI_C_Window window, size_t start_index, CEGUI_C_Window* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_GetNextSelected(CEGUI_C_Window window, CEGUI_C_Window start_item, CEGUI_C_Window* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_ItemListbox_SelectRange(CEGUI_C_Window window, size_t first, size_t last);

/* TreeItem and Tree. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_Create(const char* text_utf8, uint32_t id, CEGUI_C_Bool disabled, CEGUI_C_Bool auto_delete, CEGUI_C_TreeItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_Destroy(CEGUI_C_TreeItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetText(CEGUI_C_TreeItem item, const char** out_text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetText(CEGUI_C_TreeItem item, const char* text_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetID(CEGUI_C_TreeItem item, uint32_t* out_id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetID(CEGUI_C_TreeItem item, uint32_t id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetUserData(CEGUI_C_TreeItem item, uintptr_t* out_data);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetUserData(CEGUI_C_TreeItem item, uintptr_t data);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsSelected(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetSelected(CEGUI_C_TreeItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsDisabled(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetDisabled(CEGUI_C_TreeItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsAutoDeleted(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_SetAutoDeleted(CEGUI_C_TreeItem item, CEGUI_C_Bool value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_IsOpen(CEGUI_C_TreeItem item, CEGUI_C_Bool* out_value);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_ToggleOpen(CEGUI_C_TreeItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetChildCount(CEGUI_C_TreeItem item, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_GetChildAt(CEGUI_C_TreeItem item, size_t index, CEGUI_C_TreeItem* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_AddChild(CEGUI_C_TreeItem item, CEGUI_C_TreeItem child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TreeItem_RemoveChild(CEGUI_C_TreeItem item, CEGUI_C_TreeItem child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetSelectedCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetFirstSelected(CEGUI_C_Window window, CEGUI_C_TreeItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_GetNextSelected(CEGUI_C_Window window, CEGUI_C_TreeItem start_item, CEGUI_C_TreeItem* out_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_AddItem(CEGUI_C_Window window, CEGUI_C_TreeItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_InsertItem(CEGUI_C_Window window, CEGUI_C_TreeItem item, CEGUI_C_TreeItem before_item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_RemoveItem(CEGUI_C_Window window, CEGUI_C_TreeItem item);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_SetItemSelected(CEGUI_C_Window window, CEGUI_C_TreeItem item, CEGUI_C_Bool selected);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_Tree_EnsureItemVisible(CEGUI_C_Window window, CEGUI_C_TreeItem item);

/* Tabs and menus. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetTabCount(CEGUI_C_Window window, size_t* out_count);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetSelectedIndex(CEGUI_C_Window window, size_t* out_index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_SetSelectedIndex(CEGUI_C_Window window, size_t index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetContentAt(CEGUI_C_Window window, size_t index, CEGUI_C_Window* out_content);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_AddTab(CEGUI_C_Window window, CEGUI_C_Window content);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_RemoveTabByName(CEGUI_C_Window window, const char* name_utf8);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_RemoveTabByID(CEGUI_C_Window window, uint32_t id);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_MakeVisible(CEGUI_C_Window window, size_t index);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_GetPanePosition(CEGUI_C_Window window, CEGUI_C_TabPanePosition* out_position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_TabControl_SetPanePosition(CEGUI_C_Window window, CEGUI_C_TabPanePosition position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_GetPopupMenu(CEGUI_C_Window window, CEGUI_C_Window* out_popup);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_SetPopupMenu(CEGUI_C_Window window, CEGUI_C_Window popup);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_MenuItem_TogglePopupMenu(CEGUI_C_Window window, CEGUI_C_Bool* out_opened);

/* Layout containers. */
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_GetPosition(CEGUI_C_Window window, CEGUI_C_Window child, size_t* out_position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_GetChildAt(CEGUI_C_Window window, size_t position, CEGUI_C_Window* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_MoveChildTo(CEGUI_C_Window window, CEGUI_C_Window child, size_t position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_SequentialLayout_AddChildAt(CEGUI_C_Window window, CEGUI_C_Window child, size_t position);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_SetDimensions(CEGUI_C_Window window, size_t width, size_t height);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetDimensions(CEGUI_C_Window window, size_t* out_width, size_t* out_height);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetAutoPositioning(CEGUI_C_Window window, CEGUI_C_GridAutoPositioning* out_mode);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_SetAutoPositioning(CEGUI_C_Window window, CEGUI_C_GridAutoPositioning mode);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_GetChildAt(CEGUI_C_Window window, size_t x, size_t y, CEGUI_C_Window* out_child);
CEGUI_C_API CEGUI_C_Result CEGUI_C_CALL CEGUI_C_GridLayout_AddChildAt(CEGUI_C_Window window, CEGUI_C_Window child, size_t x, size_t y);

#ifdef __cplusplus
}
#endif

#endif
