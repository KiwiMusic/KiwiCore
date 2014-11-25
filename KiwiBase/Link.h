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

#include "Tools.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    struct Socket
    {
        wBox          box;
        unsigned long index;
        
        Socket() noexcept :
        box(weak_ptr<Box>()), index(0)
        {
            
        }
        
        Socket(shared_ptr<Box> _box, unsigned long _index) noexcept :
        box(_box), index(_index)
        {
            
        }
        
        Socket(Socket const& other) noexcept :
        box(other.box), index(other.index)
        {
            
        }
    };
    
    //! The link is used to create a connection between boxes.
    /**
     The link is a combination of two sockets used to create the connection between boxes in a page.
     */
    class Link
    {
    private:
        const Socket m_from;
        const Socket m_to;
        
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(Socket const& from, Socket const& to) noexcept :
        m_from(from), m_to(to)
        {
            ;
        }
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Link()
        {
            ;
        }
        
        //! The link creation method with sockets.
        /** The function allocates a link, checks if the link is valid and returns it. If the link isn't valid it returns an invalid pointer.
         @param from    The from socket.
         @param to      The to socket.
         @return The link.
         */
        static sLink create(Socket const& from, Socket const& to);
        
        //! The link creation method with a dico.
        /** The function allocates a link with a page and a dico.
         @param page    The page that owns the boxes.
         @param dico    The dico that defines the link.
         @return The link.
         */
        static sLink create(scPage page, scDico dico);
        
        //! The link creation method with another link but change one of the boxes with another one.
        /** The function allocates a link with another link.
         @param link The other link.
         @param oldbox  The old box to replace.
         @param newbox  The newbox box that replace.
         @return The link.
         */
        static sLink create(scLink link, const sBox oldbox, const sBox newbox);
        
        //! Retrieve the from socket.
        /** The function retrieves from socket.
         @return The socket.
         */
        inline Socket getFrom() const noexcept
        {
            return m_from;
        }
        
        //! Retrieve the to socket.
        /** The function retrieves to socket.
         @return The socket.
         */
        inline Socket getTo() const noexcept
        {
            return m_to;
        }
        
        //! Retrieve the output box.
        /** The function retrieves the output box of the link.
         @return The output box.
         */
        inline sBox getBoxFrom() const noexcept
        {
            return m_from.box.lock();
        }
        
        //! Retrieve the input box.
        /** The function retrieves the input box of the link.
         @return The input box.
         */
        inline sBox getBoxTo() const noexcept
        {
            return m_to.box.lock();
        }
        
        //! Retrieve the index of the outlet of the link.
        /** The function retrieves the index of the outlet of the link.
         @return The index of the outlet of the link.
         */
        inline unsigned long getOutletIndex() const noexcept
        {
            return m_from.index;
        }
        
        //! Retrieve the index of the inlet of the link.
        /** The function retrieves the index of the inlet of the link.
         @return The index of the inlet of the link.
         */
        inline unsigned long getInletIndex() const noexcept
        {
            return m_to.index;
        }
        
        //! Retrieve if the link is valid.
        /** The function retrieves if the link is potentially connectable. It checks if the boxes, the inlet and the outlet are compatibles.
         @return True if the link is valid, otherwise false.
         */
        bool isValid() const noexcept;
        
        //! Retrieve if the link is connectable.
        /** The function retrieves if the link is connectable. It checks if link is valid and if the inlet and outlet are not already connected.
         @return True if the link is valid, otherwise false.
         */
        bool isConnectable() const noexcept;
        
        //! Connect the link.
        /** The function connects link.
         @return True if the link has been connected, otherwise false.
         */
        bool connect() const noexcept;
        
        //! Disconnect the link.
        /** The function disconnects link.
         @return True if the link has been disconnected, otherwise false.
         */
        bool disconnect() const noexcept;
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept;
            
    };
}


#endif


