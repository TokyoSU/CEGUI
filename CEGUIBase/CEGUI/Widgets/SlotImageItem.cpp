#include "CEGUI/Widgets/SlotImageItem.h"
#include "CEGUI/TplWindowProperty.h"

namespace CEGUI
{

const String SlotImageItem::WidgetTypeName("CEGUI/SlotImageItem");
const String SlotImageItem::EventNamespace("SlotImageItem");

//----------------------------------------------------------------------------//
SlotImageItem::SlotImageItem(const String& type, const String& name) :
    Window(type, name),
    d_itemImage(0),
    d_qualityImage(0),
    d_starImage(0),
    d_itemCount(0),
    d_showStar(false),
    d_showQuality(false)
{
    addSlotImageItemProperties();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setItemImage(const Image* img)
{
    d_itemImage = img;
    invalidate();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setQualityImage(const Image* img)
{
    d_qualityImage = img;
    invalidate();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setStarImage(const Image* img)
{
    d_starImage = img;
    invalidate();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setItemCount(uint count)
{
    d_itemCount = count;
    invalidate();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setShowStar(bool show)
{
    d_showStar = show;
    invalidate();
}

//----------------------------------------------------------------------------//
void SlotImageItem::setShowQuality(bool show)
{
    d_showQuality = show;
    invalidate();
}

//----------------------------------------------------------------------------//
const Image* SlotImageItem::getItemImage()    const { return d_itemImage; }
const Image* SlotImageItem::getQualityImage() const { return d_qualityImage; }
const Image* SlotImageItem::getStarImage()    const { return d_starImage; }
uint         SlotImageItem::getItemCount()    const { return d_itemCount; }
bool         SlotImageItem::getShowStar()     const { return d_showStar; }
bool         SlotImageItem::getShowQuality()  const { return d_showQuality; }

//----------------------------------------------------------------------------//
void SlotImageItem::addSlotImageItemProperties()
{
    const String propertyOrigin(WidgetTypeName);

    CEGUI_DEFINE_PROPERTY(SlotImageItem, Image*,
        "ItemImage",
        "Property to get/set the background item image. "
        "Value is \"ImagesetName/ImageName\".",
        &SlotImageItem::setItemImage, &SlotImageItem::getItemImage, 0)

    CEGUI_DEFINE_PROPERTY(SlotImageItem, Image*,
        "QualityImage",
        "Property to get/set the quality border image. "
        "Value is \"ImagesetName/ImageName\".",
        &SlotImageItem::setQualityImage, &SlotImageItem::getQualityImage, 0)

    CEGUI_DEFINE_PROPERTY(SlotImageItem, Image*,
        "StarImage",
        "Property to get/set the star decoration image (top-right corner). "
        "Value is \"ImagesetName/ImageName\".",
        &SlotImageItem::setStarImage, &SlotImageItem::getStarImage, 0)

    CEGUI_DEFINE_PROPERTY(SlotImageItem, uint,
        "ItemCount",
        "Property to get/set the stack count shown in the bottom-right corner. "
        "Value is an unsigned integer; 0 hides the count.",
        &SlotImageItem::setItemCount, &SlotImageItem::getItemCount, 0)

    CEGUI_DEFINE_PROPERTY(SlotImageItem, bool,
        "ShowStar",
        "Property to get/set whether the star icon is visible. "
        "Value is \"true\" or \"false\".",
        &SlotImageItem::setShowStar, &SlotImageItem::getShowStar, false)

    CEGUI_DEFINE_PROPERTY(SlotImageItem, bool,
        "ShowQuality",
        "Property to get/set whether the quality border image is visible. "
        "Value is \"true\" or \"false\".",
        &SlotImageItem::setShowQuality, &SlotImageItem::getShowQuality, false)
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section
