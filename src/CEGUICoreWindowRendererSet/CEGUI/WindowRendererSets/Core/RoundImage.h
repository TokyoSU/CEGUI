/***********************************************************************
    purpose:    Falagard window renderer for RoundImage.

    Rendering:
      1. The user-supplied image is drawn scaled to the full widget area.
      2. The LookNFeel "Enabled" / "Disabled" state imagery is rendered on
         top to provide the circular mask / frame.  The LookNFeel author is
         responsible for supplying the circular overlay that hides the corners
         of the underlying rectangular image.
*************************************************************************/
#ifndef _FalRoundImage_h_
#define _FalRoundImage_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

class COREWRSET_API FalagardRoundImage : public WindowRenderer
{
public:
    static const String TypeName; //!< type name for this renderer.

    FalagardRoundImage(const String& type);

    void render();
};

} // End of CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _FalRoundImage_h_
