/***********************************************************************
    purpose:    Interface to SlotImageItem widget
                Renders three layers:
                  - background: item image (full area)
                  - middle:     quality border image (full area)
                  - top:        star icon (top-right), item count (bottom-right)
*************************************************************************/
#ifndef _CEGUISlotImageItem_h_
#define _CEGUISlotImageItem_h_

#include "../Base.h"
#include "../Window.h"
#include "../WindowFactory.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

class CEGUIEXPORT SlotImageItem : public Window
{
public:
    static const String WidgetTypeName; //!< Window factory name
    static const String EventNamespace; //!< Namespace for global events

    SlotImageItem(const String& type, const String& name);
    virtual ~SlotImageItem() {}

    //! Set/get the background item image.
    void         setItemImage(const Image* img);
    const Image* getItemImage() const;

    //! Set/get the quality border image rendered on top of the item image.
    void         setQualityImage(const Image* img);
    const Image* getQualityImage() const;

    //! Set/get the small star decoration drawn in the top-right corner.
    void         setStarImage(const Image* img);
    const Image* getStarImage() const;

    //! Set/get the item count shown in the bottom-right corner (0 = hidden).
    void setItemCount(uint count);
    uint getItemCount() const;

    //! Set/get whether the star image is visible.
    void setShowStar(bool show);
    bool getShowStar() const;

    //! Set/get whether the quality border image is visible.
    void setShowQuality(bool show);
    bool getShowQuality() const;

protected:
    const Image* d_itemImage;    //!< Background item image.
    const Image* d_qualityImage; //!< Quality border overlay image.
    const Image* d_starImage;    //!< Star icon drawn in the top-right corner.
    uint         d_itemCount;    //!< Stack count drawn in the bottom-right corner.
    bool         d_showStar;     //!< Whether to draw the star icon.
    bool         d_showQuality;  //!< Whether to draw the quality border image.

private:
    void addSlotImageItemProperties();
};

CEGUI_DECLARE_WINDOW_FACTORY(SlotImageItem)

} // End of CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUISlotImageItem_h_
