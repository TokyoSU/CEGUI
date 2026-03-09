#include "CEGUI/WindowRendererSets/Core/RoundImage.h"
#include "CEGUI/Widgets/RoundImage.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/Image.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/GeometryBuffer.h"

namespace CEGUI
{

const String FalagardRoundImage::TypeName("Falagard/RoundImage");

//----------------------------------------------------------------------------//
FalagardRoundImage::FalagardRoundImage(const String& type) :
    WindowRenderer(type, "RoundImage")
{
}

//----------------------------------------------------------------------------//
void FalagardRoundImage::render()
{
    RoundImage* w = static_cast<RoundImage*>(d_window);

    // --- Layer 1: user image scaled to the full widget area ---
    if (w->getImage())
    {
        const Rectf  destRect  = w->getUnclippedOuterRect().get();
        const Rectf  clipRect  = w->getOuterRectClipper();
        GeometryBuffer& geobuf = w->getGeometryBuffer();

        const ColourRect colours(Colour(1.0f, 1.0f, 1.0f, w->getEffectiveAlpha()));
        w->getImage()->render(geobuf, destRect, &clipRect, colours);
    }

    // --- Layer 2: LookNFeel circular mask / frame overlay ---
    const WidgetLookFeel& wlf = getLookNFeel();
    wlf.getStateImagery(w->isEffectiveDisabled() ? "Disabled" : "Enabled").render(*d_window);
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section
