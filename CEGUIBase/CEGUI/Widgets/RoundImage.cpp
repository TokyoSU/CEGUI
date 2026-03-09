#include "CEGUI/Widgets/RoundImage.h"
#include "CEGUI/TplWindowProperty.h"

namespace CEGUI
{

const String RoundImage::WidgetTypeName("CEGUI/RoundImage");
const String RoundImage::EventNamespace("RoundImage");

//----------------------------------------------------------------------------//
RoundImage::RoundImage(const String& type, const String& name) :
    Window(type, name),
    d_image(0)
{
    addRoundImageProperties();
}

//----------------------------------------------------------------------------//
void RoundImage::setImage(const Image* img)
{
    d_image = img;
    invalidate();
}

//----------------------------------------------------------------------------//
const Image* RoundImage::getImage() const { return d_image; }

//----------------------------------------------------------------------------//
void RoundImage::addRoundImageProperties()
{
    const String propertyOrigin(WidgetTypeName);

    CEGUI_DEFINE_PROPERTY(RoundImage, Image*,
        "Image",
        "Property to get/set the image displayed inside the circular area. "
        "Value is \"ImagesetName/ImageName\".",
        &RoundImage::setImage, &RoundImage::getImage, 0)
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section
