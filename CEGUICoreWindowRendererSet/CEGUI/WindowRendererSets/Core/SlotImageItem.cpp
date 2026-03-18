#include "CEGUI/WindowRendererSets/Core/SlotImageItem.h"
#include "CEGUI/Widgets/SlotImageItem.h"
#include "CEGUI/Font.h"
#include "CEGUI/Image.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/GeometryBuffer.h"

namespace CEGUI
{

const String FalagardSlotImageItem::TypeName("Core/SlotImageItem");

//----------------------------------------------------------------------------//
FalagardSlotImageItem::FalagardSlotImageItem(const String& type) :
    WindowRenderer(type, "SlotImageItem")
{
}

//----------------------------------------------------------------------------//
void FalagardSlotImageItem::render()
{
    SlotImageItem* w = static_cast<SlotImageItem*>(d_window);

    const Rectf  destRect  = w->getUnclippedOuterRect().get();
    const Rectf  clipRect  = w->getOuterRectClipper();
    GeometryBuffer& geobuf = w->getGeometryBuffer();

    const float alpha = w->getEffectiveAlpha();
    const ColourRect baseColours(Colour(1.0f, 1.0f, 1.0f, alpha));

    // --- Layer 1: background item image ---
    if (w->getItemImage())
        w->getItemImage()->render(geobuf, destRect, &clipRect, baseColours);

    // --- Layer 2: quality border image ---
    if (w->getShowQuality() && w->getQualityImage())
        w->getQualityImage()->render(geobuf, destRect, &clipRect, baseColours);

    // --- Layer 3: star icon in top-right corner ---
    if (w->getShowStar() && w->getStarImage())
    {
        const float starW = destRect.getWidth()  * 0.35f;
        const float starH = destRect.getHeight() * 0.35f;
        const Rectf starRect(destRect.right() - starW,
                             destRect.top(),
                             destRect.right(),
                             destRect.top() + starH);
        w->getStarImage()->render(geobuf, starRect, &clipRect, baseColours);
    }

    // --- Layer 4: item count text in bottom-right corner ---
    if (w->getItemCount() > 0)
    {
        const Font* font = w->getFont();
        if (font)
        {
            const String countStr(PropertyHelper<uint>::toString(w->getItemCount()));

            const float textW = font->getTextExtent(countStr);
            const float textH = font->getFontHeight();

            const Vector2f textPos(destRect.right() - textW - 2.0f,
                                   destRect.bottom() - textH - 2.0f);

            const ColourRect textCols(Colour(1.0f, 1.0f, 1.0f, alpha));
            font->drawText(geobuf, countStr, textPos, &clipRect, textCols);
        }
    }
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section
