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

#ifndef __DEF_KIWI_LINK__
#define __DEF_KIWI_LINK__

#include "Box.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    //! The link is used to create a connection between boxes.
    /**
     The link is a combination of two sockets used to create the connection between boxes in a page.
     */
    class Link : public Attr::Manager
    {
    private:
        const wPage         m_page;
        const wBox          m_box_from;
        const wBox          m_box_to;
        const ulong         m_index_outlet;
        const ulong         m_index_intlet;
        const Box::Io::Type m_type;
        const sAttrColor    m_attr_color_message;
        const sAttrColor    m_attr_color_signal;
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(const sPage page, const sBox from, const ulong outlet, const sBox to, const ulong inlet, const Box::Io::Type type) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Link();
        
        //! The link creation method with a dico.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(sPage page, scDico dico);
        
        //! Retrieve the sLink.
        /** The function sLink.
         @return The sLink.
         */
		inline sLink getShared() noexcept
		{
			return static_pointer_cast<Link>(shared_from_this());
		}
        
        //! Retrieve the scLink.
        /** The function scLink.
         @return The scLink.
         */
		inline scLink getShared() const noexcept
		{
			return static_pointer_cast<const Link>(shared_from_this());
		}
        
        //! Retrieve the page of the link.
        /** The function retrieves the page of the link.
         @return The page of the link.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
        //! Retrieve the output box.
        /** The function retrieves the output box of the link.
         @return The output box.
         */
        inline sBox getBoxFrom() const noexcept
        {
            return m_box_from.lock();
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the link.
         @return The input box.
         */
        inline sBox getBoxTo() const noexcept
        {
            return m_box_to.lock();
        }
        
        //! Retrieve the index of the outlet of the link.
        /** The function retrieves the index of the outlet of the link.
         @return The index of the outlet of the link.
         */
        inline ulong getOutletIndex() const noexcept
        {
            return m_index_outlet;
        }
        
        //! Retrieve the index of the inlet of the link.
        /** The function retrieves the index of the inlet of the link.
         @return The index of the inlet of the link.
         */
        inline ulong getInletIndex() const noexcept
        {
            return m_index_intlet;
        }
        
        //! Retrieve the io type of the link.
        /** The function retrieves the io type of the link.
         @return The io type of the link.
         */
        inline Box::Io::Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve if the message color of the link.
        /** The function retrieves the message color of the link.
         @return The message color of the link.
         */
        inline Gui::Color getMessageColor() const noexcept
        {
            return m_attr_color_message->get();
        }
        
        //! Retrieve if the signal color of the link.
        /** The function retrieves the signal color of the link.
         @return The signal color of the link.
         */
        inline Gui::Color getSignalColor() const noexcept
        {
            return m_attr_color_signal->get();
        }
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept;
    };
    
    static bool operator==(scLink link, scBox box) noexcept
    {
        if(link && box)
        {
            return link->getBoxFrom() == box || link->getBoxTo() == box;
        }
        else
        {
            return false;
        }
    }
    
    static bool operator==(sLink link, sBox box) noexcept
    {
        if(link && box)
        {
            return link->getBoxFrom() == box || link->getBoxTo() == box;
        }
        else
        {
            return false;
        }
    }
    
}


#endif


