/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef __DEF_KIWI_ATTRIBUTE_BOX__
#define __DEF_KIWI_ATTRIBUTE_BOX__

#include "Attribute.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ATTRIBUTE BOX                                   //
    // ================================================================================ //
    
    //! The font attribute is the default attributes for the font.
	/** Holds a vector of two double values suitable to represent a position or a size,
	 */
    
    class AttrBox : public Attr::Manager
    {
    public:
        // Appearance //
        static const sTag Tag_Appearance;
        static const sTag Tag_position;
        static const sTag Tag_Position;
        static const sTag Tag_size;
        static const sTag Tag_Size;
        static const sTag Tag_presentation_position;
        static const sTag Tag_Presentation_Position;
        static const sTag Tag_presentation_size;
        static const sTag Tag_Presentation_Size;
        static const sTag Tag_hidden;
        static const sTag Tag_Hide_on_Lock;
        static const sTag Tag_presentation;
        static const sTag Tag_Include_in_Presentation;
        
        // Font //
        static const sTag Tag_Font;
        static const sTag Tag_fontname;
        static const sTag Tag_Font_Name;
        static const sTag Tag_Arial;
        static const sTag Tag_fontsize;
        static const sTag Tag_Font_Size;
        static const sTag Tag_fontface;
        static const sTag Tag_Font_Face;
        static const sTag Tag_normal;
        static const sTag Tag_bold;
        static const sTag Tag_italic;
        static const sTag Tag_bold_italic;
        static const sTag Tag_fontjustification;
        static const sTag Tag_Font_Justification;
        static const sTag Tag_left;
        static const sTag Tag_center;
        static const sTag Tag_right;
        
        // Appearance //
        const shared_ptr<AttrPoint>     appearance_position;
        const shared_ptr<AttrPoint>     appearance_size;
        const shared_ptr<AttrPoint>     appearance_presentation_position;
        const shared_ptr<AttrPoint>     appearance_presentation_size;
        const shared_ptr<AttrBool>      appearance_hidden;
        const shared_ptr<AttrBool>      appearance_presentation;
        
        // Font //
        const shared_ptr<AttrTag>       font_name;
        const shared_ptr<AttrDouble>    font_size;
        const shared_ptr<AttrEnum>      font_face;
        const shared_ptr<AttrEnum>      font_justification;
        
        AttrBox() :
        // Appearance //
        appearance_position(Attr::create<AttrPoint>(Tag_position, Tag_Position, Tag_Appearance)),
        appearance_size(Attr::create<AttrPoint>(Tag_size, Tag_Size, Tag_Appearance, (ElemVector){100., 20.})),
        appearance_presentation_position(Attr::create<AttrPoint>(Tag_presentation_position, Tag_Presentation_Position, Tag_Appearance)),
        appearance_presentation_size(Attr::create<AttrPoint>(Tag_presentation_size, Tag_Presentation_Size, Tag_Appearance)),
        appearance_hidden(Attr::create<AttrBool>(Tag_hidden, Tag_Hide_on_Lock, Tag_Appearance, false)),
        appearance_presentation(Attr::create<AttrBool>(Tag_presentation,  Tag_Include_in_Presentation, Tag_Appearance, false)),
        
        // Font //
        font_name(Attr::create<AttrTag>(Tag_fontname, Tag_Font_Name, Tag_Font, Tag_Arial)),
        font_size(Attr::create<AttrDouble>(Tag_fontsize, Tag_Font_Size, Tag_Font, 12)),
        font_face(Attr::create<AttrEnum>(Tag_fontface, Tag_Font_Face, Tag_Font, (ElemVector){Tag_normal, Tag_bold, Tag_italic, Tag_bold_italic}, 0)),
        font_justification(Attr::create<AttrEnum>(Tag_fontjustification, Tag_Font_Justification, Tag_Font, (ElemVector){Tag_left, Tag_center, Tag_right}, 0))
        
        {
            // Appearance //
            addAttribute(appearance_position);
            addAttribute(appearance_size);
            addAttribute(appearance_presentation_position);
            addAttribute(appearance_presentation_size);
            addAttribute(appearance_hidden);
            addAttribute(appearance_presentation);
            
            // Font //
            addAttribute(font_name);
            addAttribute(font_size);
            addAttribute(font_face);
            addAttribute(font_justification);
        }
        
        ~AttrBox()
        {
            ;
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPosition() const noexcept
        {
            return appearance_position->get();
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getSize() const noexcept
        {
            return appearance_size->get();
        }
    
        //! Retrieve the bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Rectangle getBounds() const noexcept
        {
            return Rectangle(appearance_position->get(), appearance_size->get());
        }
        
        //! Retrieve the presentation size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPresentationPosition() const noexcept
        {
            return appearance_presentation_position->get();
        }
        
        //! Retrieve the presentation size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPresentationSize() const noexcept
        {
            return appearance_presentation_size->get();
        }
        
        //! Retrieve the presentation bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Rectangle getPresentationBounds() const noexcept
        {
            return Rectangle(appearance_presentation_position->get(), appearance_presentation_size->get());
        }
        
        //! Retrieve if the box should be hidden.
        /** The function retrieves if the box should be hidden.
         @return True if the box should be hidden, otherwise false.
         */
        inline bool isHidden() const noexcept
        {
            return appearance_hidden->get();
        }
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isInPresentation() const noexcept
        {
            return appearance_presentation->get();
        }
    };
}


#endif


