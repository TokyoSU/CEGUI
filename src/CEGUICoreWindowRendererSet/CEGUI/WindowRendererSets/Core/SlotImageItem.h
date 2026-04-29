/***********************************************************************
    purpose:    Falagard window renderer for SlotImageItem.

    Rendering layers (back to front):
      1. Item image       - fills the entire widget area.
      2. Quality image    - quality border overlay, fills the entire widget area.
      3. Star image       - occupies the top-right 35% × 35% of the slot (optional).
      4. Item count text  - right/bottom aligned inside the slot (hidden when 0).
*************************************************************************/
#ifndef _FalSlotImageItem_h_
#define _FalSlotImageItem_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

class COREWRSET_API FalagardSlotImageItem : public WindowRenderer
{
public:
    static const String TypeName; //!< type name for this renderer.

    FalagardSlotImageItem(const String& type);

    void render();
};

} // End of CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _FalSlotImageItem_h_
