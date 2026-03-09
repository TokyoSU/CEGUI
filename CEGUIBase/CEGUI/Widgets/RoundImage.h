/***********************************************************************
    purpose:    Interface to RoundImage widget.
                Displays a user-supplied image scaled to the widget area.
                The circular appearance is driven entirely by the LookNFeel
                (which provides a circular mask / frame overlay rendered on
                top of the image via the "Enabled" / "Disabled" states).
*************************************************************************/
#ifndef _CEGUIRoundImage_h_
#define _CEGUIRoundImage_h_

#include "../Base.h"
#include "../Window.h"
#include "../WindowFactory.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

class CEGUIEXPORT RoundImage : public Window
{
public:
    static const String WidgetTypeName; //!< Window factory name
    static const String EventNamespace; //!< Namespace for global events

    RoundImage(const String& type, const String& name);
    virtual ~RoundImage() {}

    //! Set/get the image to display inside the circular frame.
    void         setImage(const Image* img);
    const Image* getImage() const;

protected:
    const Image* d_image; //!< The image rendered inside the circular area.

private:
    void addRoundImageProperties();
};

CEGUI_DECLARE_WINDOW_FACTORY(RoundImage)

} // End of CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIRoundImage_h_
