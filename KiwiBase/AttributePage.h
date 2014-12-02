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

#ifndef __DEF_KIWI_ATTRIBUTE_PAGE__
#define __DEF_KIWI_ATTRIBUTE_PAGE__

#include "Attribute.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                 ATTRIBUTE PAGE                                   //
    // ================================================================================ //
    
    //! The AttrPage class manages attributes of a page.
	/** The AttrPage class manages attributes of a page.
	 */
    
    class AttrPage : public Attr::Manager
    {
    public:
        // Font //
        static const sTag Tag_Font;
        static const sTag Tag_fontname;
        static const sTag Tag_Font_Name;
        static const sTag Tag_Arial;
        static const sTag Tag_Menelo;
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
        
        // Color //
        static const sTag Tag_Color;
        static const sTag Tag_editing_bgcolor;
        static const sTag Tag_Unlocked_Background_Color;
		static const sTag Tag_locked_bgcolor;
		static const sTag Tag_Locked_Background_Color;
		
		// Editing //
		static const sTag Tag_Editing;
		static const sTag Tag_gridsize;
		static const sTag Tag_Grid_Size;
		
		//-----------------------------------------------------
		
        // Font //
        const shared_ptr<AttrTag>       font_name;
        const shared_ptr<AttrDouble>    font_size;
        const shared_ptr<AttrEnum>      font_face;
        const shared_ptr<AttrEnum>      font_justification;
        
        // Color //
        const shared_ptr<AttrColor>     color_editing_background;
        const shared_ptr<AttrColor>     color_locked_background;
		
		// Editing //
		const shared_ptr<AttrLong>		editing_grid_size;
		
		//-----------------------------------------------------
		
        AttrPage() :
        // Font //
        font_name(Attr::create<AttrTag>(Tag_fontname, Tag_Font_Name, Tag_Font, Tag_Menelo)),
        font_size(Attr::create<AttrDouble>(Tag_fontsize, Tag_Font_Size, Tag_Font, 13)),
        font_face(Attr::create<AttrEnum>(Tag_fontface, Tag_Font_Face, Tag_Font, (ElemVector){Tag_normal, Tag_bold, Tag_italic, Tag_bold_italic}, 0)),
        font_justification(Attr::create<AttrEnum>(Tag_fontjustification, Tag_Font_Justification, Tag_Font, (ElemVector){Tag_left, Tag_center, Tag_right}, 0)),
        
        // Color //
        color_editing_background(Attr::create<AttrColor>(Tag_editing_bgcolor, Tag_Unlocked_Background_Color, Tag_Color, (ElemVector){1., 1., 1, 1.})),
        color_locked_background(Attr::create<AttrColor>(Tag_locked_bgcolor, Tag_Locked_Background_Color, Tag_Color, (ElemVector){0.4, 0.4, 0.4, 1.})),
		
		// Editing //
		editing_grid_size(Attr::create<AttrLong>(Tag_editing_bgcolor, Tag_Unlocked_Background_Color, Tag_Editing, 15))
        {
            // Font //
            addAttribute(font_name);
            addAttribute(font_size);
            addAttribute(font_face);
            addAttribute(font_justification);
            
            // Color //
            addAttribute(color_editing_background);
            addAttribute(color_locked_background);
        }
        
        ~AttrPage()
        {
            ;
        }
        
        //! Retrieve if the font of the box.
        /** The function retrieves the font of the box.
         @return The font of the box.
         */
        inline Font getFont() const noexcept
        {
            return Font(toString(font_name->get()), font_size->get(), (Font::Face)font_face->get());
        }
        
        //! Retrieve if the font justification of the box.
        /** The function retrieves the font justification of the box.
         @return The font justification of the box.
         */
        inline Font::Justification getFontJustification() const noexcept
        {
            return (Font::Justification)font_justification->get();
        }
        
        //! Retrieve if the background color of the box.
        /** The function retrieves the background color of the box.
         @return The background color of the box.
         */
        inline Color getEditingBgColor() const noexcept
        {
            return color_editing_background->get();
        }
		
		//! Retrieve if the locked background color.
		/** The function retrieves the background color of the box.
		 @return The background color of the box.
		 */
		inline Color getLockedBgColor() const noexcept
		{
			return color_locked_background->get();
		}
    };
}


#endif


