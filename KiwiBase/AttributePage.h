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
        const shared_ptr<AttrTag>       attr_font_name;
        const shared_ptr<AttrDouble>    attr_font_size;
        const shared_ptr<AttrEnum>      attr_font_face;
        const shared_ptr<AttrEnum>      attr_font_justification;
        
        // Color //
        const shared_ptr<AttrColor>     color_editing_background;
        const shared_ptr<AttrColor>     color_locked_background;
		
		// Editing //
		const shared_ptr<AttrLong>		editing_grid_size;
		
		//-----------------------------------------------------
		
        AttrPage() :
        // Font //
        attr_font_name(Attr::create<AttrTag>(Tag_fontname, Tag_Font_Name, Tag_Font, Tag_Menelo)),
        attr_font_size(Attr::create<AttrDouble>(Tag_fontsize, Tag_Font_Size, Tag_Font, 13)),
        attr_font_face(Attr::create<AttrEnum>(Tag_fontface, Tag_Font_Face, Tag_Font, (ElemVector){Tag_normal, Tag_bold, Tag_italic, Tag_bold_italic}, 0)),
        attr_font_justification(Attr::create<AttrEnum>(Tag_fontjustification, Tag_Font_Justification, Tag_Font, (ElemVector){Tag_left, Tag_center, Tag_right}, 0)),
        
        // Color //
        color_editing_background(Attr::create<AttrColor>(Tag_editing_bgcolor, Tag_Unlocked_Background_Color, Tag_Color, (ElemVector){0.88, 0.89, 0.88, 1.})),
        color_locked_background(Attr::create<AttrColor>(Tag_locked_bgcolor, Tag_Locked_Background_Color, Tag_Color, (ElemVector){0.88, 0.89, 0.88, 1.})),
		
		// Editing //
		editing_grid_size(Attr::create<AttrLong>(Tag_gridsize, Tag_Grid_Size, Tag_Editing, 15))
        {
            // Font //
            addAttribute(attr_font_name);
            addAttribute(attr_font_size);
            addAttribute(attr_font_face);
            addAttribute(attr_font_justification);
            
            // Color //
            addAttribute(color_editing_background);
            addAttribute(color_locked_background);
			
			// Editing //
			addAttribute(editing_grid_size);
        }
        
        ~AttrPage()
        {
            ;
        }
        
        //! Retrieve the default font for boxes of the page.
        /** The function retrieves the default font for boxes of the page.
         @return The default font for boxes of the page.
         */
        inline Font getFont() const noexcept
        {
            return Font(toString(attr_font_name->get()), attr_font_size->get(), (Font::Face)attr_font_face->get());
        }
        
        //! Retrieve the default font justification for boxes of the page.
		/** The function retrieves the default font justification for boxes of the page.
		 @return The default font justification for boxes of the page.
         */
        inline Font::Justification getFontJustification() const noexcept
        {
            return (Font::Justification)attr_font_justification->get();
        }
        
        //! Retrieve if the background color of the page when unlocked.
        /** The function retrieves the background color of the page when unlocked
         @return The Unlocked background color of the page.
         */
        inline Color getEditingBgColor() const noexcept
        {
            return color_editing_background->get();
        }
		
		//! Retrieve if the locked background color.
		/** The function retrieves the locked background color of the page.
		 @return The locked background color of the page.
		 */
		inline Color getLockedBgColor() const noexcept
		{
			return color_locked_background->get();
		}
		
		//! Retrieve the grid size attribute value.
		/** The function retrieve the grid size attribute value.
		 @return The grid size attribute value.
		 */
		inline long getGridSize() const noexcept
		{
			return editing_grid_size->get();
		}
    };
}


#endif


