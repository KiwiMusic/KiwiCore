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
        // Internal //
        static const sTag Tag_ninlets;
        static const sTag Tag_noutlets;
        
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
		
		// Behavior //
		static const sTag Tag_Behavior;
		static const sTag Tag_ignoreclick;
		static const sTag Tag_Ignore_Click;
		
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
        static const sTag Tag_bgcolor;
        static const sTag Tag_Background_Color;
        static const sTag Tag_bdcolor;
        static const sTag Tag_Border_Color;
        static const sTag Tag_textcolor;
        static const sTag Tag_Text_Color;
        
        // Internal //
        const shared_ptr<AttrLong>      attr_ninlets;
        const shared_ptr<AttrLong>      attr_noutlets;
        
        // Appearance //
        const shared_ptr<AttrPoint>     attr_position;
        const shared_ptr<AttrSize>		attr_size;
        const shared_ptr<AttrPoint>     attr_presentation_position;
        const shared_ptr<AttrSize>		attr_presentation_size;
        const shared_ptr<AttrBool>      attr_hidden;
        const shared_ptr<AttrBool>      attr_presentation;
		
		// Behavior //
		const shared_ptr<AttrBool>		attr_ignoreclick;
		
        // Font //
        const shared_ptr<AttrTag>       attr_font_name;
        const shared_ptr<AttrDouble>    attr_font_size;
        const shared_ptr<AttrEnum>      attr_font_face;
        const shared_ptr<AttrEnum>      attr_font_justification;
        
        // Color //
        const shared_ptr<AttrColor>     attr_color_background;
        const shared_ptr<AttrColor>     attr_color_border;
        const shared_ptr<AttrColor>     attr_color_text;
        
        AttrBox() :
        // Internal //
        attr_ninlets(Attr::create<AttrLong>(Tag_ninlets, nullptr, nullptr, 0, Attr::Invisible)),
        attr_noutlets(Attr::create<AttrLong>(Tag_noutlets, nullptr, nullptr, 0, Attr::Invisible)),
        
        // Appearance //
        attr_position(Attr::create<AttrPoint>(Tag_position, Tag_Position, Tag_Appearance)),
        attr_size(Attr::create<AttrSize>(Tag_size, Tag_Size, Tag_Appearance, (ElemVector){100., 20.})),
        attr_presentation_position(Attr::create<AttrPoint>(Tag_presentation_position, Tag_Presentation_Position, Tag_Appearance)),
        attr_presentation_size(Attr::create<AttrSize>(Tag_presentation_size, Tag_Presentation_Size, Tag_Appearance)),
        attr_hidden(Attr::create<AttrBool>(Tag_hidden, Tag_Hide_on_Lock, Tag_Appearance, false)),
        attr_presentation(Attr::create<AttrBool>(Tag_presentation,  Tag_Include_in_Presentation, Tag_Appearance, false)),
		
		// Appearance //
		attr_ignoreclick(Attr::create<AttrBool>(Tag_ignoreclick, Tag_Ignore_Click, Tag_Behavior, false)),
		
        // Font //
        attr_font_name(Attr::create<AttrTag>(Tag_fontname, Tag_Font_Name, Tag_Font, Tag_Menelo)),
        attr_font_size(Attr::create<AttrDouble>(Tag_fontsize, Tag_Font_Size, Tag_Font, 13)),
        attr_font_face(Attr::create<AttrEnum>(Tag_fontface, Tag_Font_Face, Tag_Font, (ElemVector){Tag_normal, Tag_bold, Tag_italic, Tag_bold_italic}, 0)),
        attr_font_justification(Attr::create<AttrEnum>(Tag_fontjustification, Tag_Font_Justification, Tag_Font, (ElemVector){Tag_left, Tag_center, Tag_right}, 0)),
        
        // Color //
        attr_color_background(Attr::create<AttrColor>(Tag_bgcolor, Tag_Background_Color, Tag_Color, (ElemVector){1., 1., 1, 1.})),
        attr_color_border(Attr::create<AttrColor>(Tag_bdcolor, Tag_Border_Color, Tag_Color, (ElemVector){0.4, 0.4, 0.4, 1.})),
        attr_color_text(Attr::create<AttrColor>(Tag_textcolor, Tag_Text_Color, Tag_Color, (ElemVector){0.3, 0.3, 0.3, 1.}))
        {
            // Appearance //
            addAttribute(attr_position);
            addAttribute(attr_size);
            addAttribute(attr_presentation_position);
            addAttribute(attr_presentation_size);
            addAttribute(attr_hidden);
            addAttribute(attr_presentation);
			
			// Appearance //
			addAttribute(attr_ignoreclick);
			
            // Font //
            addAttribute(attr_font_name);
            addAttribute(attr_font_size);
            addAttribute(attr_font_face);
            addAttribute(attr_font_justification);
            
            // Color //
            addAttribute(attr_color_background);
            addAttribute(attr_color_border);
            addAttribute(attr_color_text);
        }
        
        ~AttrBox()
        {
            ;
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
         @return The size of the box as a point.
         */
        inline Point getPosition(const bool edition = true) const noexcept
        {
			if(edition)
				return attr_position->get();
			else
				return attr_presentation_position->get();
        }
        
        //! Retrieve the size of the box.
        /** The function retrieves the size of the box as a point.
		 @param edition
         @return The size of the box as a point.
         */
        inline Point getSize(const bool edition = true) const noexcept
        {
			if(edition)
				return attr_size->get();
			else
				return attr_presentation_size->get();
        }
    
        //! Retrieve the bounds of the box.
        /** The function retrieves the bounds of the box as a rectangle.
         @return The bounds of the box as a rectangle.
         */
        inline Rectangle getBounds(const bool edition = true) const noexcept
        {
			if(edition)
				return Rectangle(attr_position->get(), attr_size->get());
			else
				return Rectangle(attr_presentation_position->get(), attr_presentation_size->get());
        }
		
		//! Sets a minimum and a maximum width and height limit.
		/** Pass a 0 point if you don't want to limit width or height.
		 @param min The minimum width and height limit.
		 @param max The maximum width and height limit.
		 */
		inline void setSizeLimits(Point const& min, Point const& max = Point()) noexcept
		{
			attr_size->setMinLimits(min);
			attr_presentation_size->setMinLimits(min);
			attr_size->setMaxLimits(max);
			attr_presentation_size->setMaxLimits(max);
		}
		
		//! Retrieves the minimum width and height limit.
		/** The function retrieves the minimum width and height limit.
		 @return The minimum width and height limit.
		 */
		inline Point getSizeMinLimits() const noexcept
		{
			return attr_size->getMinLimits();
		}
		
		//! Retrieves the maximum width and height limit.
		/** The function retrieves the maximum width and height limit.
		 @return The maximum width and height limit.
		 */
		inline Point getSizeMaxLimits() const noexcept
		{
			return attr_size->getMaxLimits();
		}
		
		//! Specifies a width-to-height ratio that the box should always maintain when it is resized.
		/** If the value is 0, no aspect ratio is enforced. If it's non-zero, the width
		 will always be maintained as this multiple of the height.
		 @see setSizeLimits
		 */
		inline void setSizeRatio(const double ratio) noexcept
		{
			attr_size->setSizeRatio(ratio);
			attr_presentation_size->setSizeRatio(ratio);
		}
		
		//! Retrieves the aspect ratio that was set with setSizeRatio().
		/** If no aspect ratio is being enforced, this will return 0.
		 */
		inline bool getSizeRatio() const noexcept
		{
			return attr_size->getSizeRatio();
		}
		
		//! Retrieves if the box should be hidden when the page is locked.
		/** The function retrieves if the box should be hidden when the page is locked.
		 @return True if the box should be hidden when the page is locked, false otherwise.
		 */
		inline bool isHiddenOnLock() const noexcept
		{
			return attr_hidden->get();
		}
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isInPresentation() const noexcept
        {
            return attr_presentation->get();
        }
		
		//! Retrieve if the box should ignore mouse click.
		/** The function retrieves if the box should ignore mouse click.
		 @return True if the box should ignore mouse click, false otherwise.
		 */
		inline bool getIgnoreClick() const noexcept
		{
			return attr_ignoreclick->get();
		}
        
        //! Retrieve if the font of the box.
        /** The function retrieves the font of the box.
         @return The font of the box.
         */
        inline Font getFont() const noexcept
        {
            return Font(toString(attr_font_name->get()), attr_font_size->get(), (Font::Face)attr_font_face->get());
        }
        
        //! Retrieve if the font justification of the box.
        /** The function retrieves the font justification of the box.
         @return The font justification of the box.
         */
        inline Font::Justification getFontJustification() const noexcept
        {
            return (Font::Justification)attr_font_justification->get();
        }
        
        //! Retrieve if the background color of the box.
        /** The function retrieves the background color of the box.
         @return The background color of the box.
         */
        inline Color getBackgroundColor() const noexcept
        {
            return attr_color_background->get();
        }
        
        //! Retrieve if the border color of the box.
        /** The function retrieves the   color of the box.
         @return The border color of the box.
         */
        inline Color getBorderColor() const noexcept
        {
            return attr_color_border->get();
        }
        
        //! Retrieve if the text color of the box.
        /** The function retrieves the text of the box.
         @return The text color of the box.
         */
        inline Color getTextColor() const noexcept
        {
            return attr_color_text->get();
        }
    };
}


#endif


