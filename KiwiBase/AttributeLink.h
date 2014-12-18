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

#ifndef __DEF_KIWI_ATTRIBUTE_LINK__
#define __DEF_KIWI_ATTRIBUTE_LINK__

#include "Attribute.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  ATTRIBUTE LINK                                  //
    // ================================================================================ //
    
    //!
	/**
	 */
    class AttrLink : public Attr::Manager
    {
    public:
        static const sTag Tag_Color;
        static const sTag Tag_mescolor;
        static const sTag Tag_sigcolor;
        static const sTag Tag_Message_Color;
        static const sTag Tag_Signal_Color;
        
        const shared_ptr<AttrColor>     attr_color_message;
        const shared_ptr<AttrColor>     attr_color_signal;
        
        AttrLink() :
        attr_color_message(
        Attr::create<AttrColor>(Tag_mescolor, Tag_Message_Color, Tag_Color, (ElemVector){0.42, 0.42, 0.42, 1.})),
        attr_color_signal(
        Attr::create<AttrColor>(Tag_sigcolor, Tag_Signal_Color, Tag_Color, (ElemVector){0.4, 0.4, 0.4, 1.}))
        {
            addAttribute(attr_color_message);
            addAttribute(attr_color_signal);
        }
        
        ~AttrLink()
        {
            ;
        }
        
        //! Retrieve if the message color of the link.
        /** The function retrieves the message color of the link.
         @return The message color of the link.
         */
        inline Color getMessageColor() const noexcept
        {
            return attr_color_message->get();
        }
        
        //! Retrieve if the signal color of the link.
        /** The function retrieves the signal color of the link.
         @return The signal color of the link.
         */
        inline Color getSignalColor() const noexcept
        {
            return attr_color_signal->get();
        }
    };
}


#endif


