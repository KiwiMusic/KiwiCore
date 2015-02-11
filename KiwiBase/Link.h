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

#include "Object.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    //! The link is used to create a connection between objects.
    /**
     The link is a combination of two sockets used to create the connection between objects in a page.
     */
    class Link
    {
    private:
        const wPage         m_page;
        const wObject       m_object_from;
        const wObject       m_object_to;
        const ulong         m_index_outlet;
        const ulong         m_index_intlet;
        const Object::Io::Type m_type;
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(const sPage page, const sObject from, const ulong outlet, const sObject to, const ulong inlet, const Object::Io::Type type) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        virtual ~Link();
        
        //! Retrieve the page of the link.
        /** The function retrieves the page of the link.
         @return The page of the link.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
        //! Retrieve the output object.
        /** The function retrieves the output object of the link.
         @return The output object.
         */
        inline sObject getObjectFrom() const noexcept
        {
            return m_object_from.lock();
        }
        
        //! Retrieve the input object.
        /** The function retrieves the input object of the link.
         @return The input object.
         */
        inline sObject getObjectTo() const noexcept
        {
            return m_object_to.lock();
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
        inline Object::Io::Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Write the page in a dico.
        /** The function writes the link in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const noexcept;
        
        class DspLink;
    };

    class Link::DspLink : public Link, public Dsp::Connection
    {
    public:
        DspLink(const sPage page, const sObject from, const ulong outlet, const sObject to, const ulong inlet, const Object::Io::Type type, Dsp::sProcess pfrom, const ulong poutlet, Dsp::sProcess pto, const ulong pinlet) :
        Link(page, from, outlet, to, inlet, type),
        Dsp::Connection(pfrom, poutlet, pto, pinlet)
        {
            ;
        }
    };
}


#endif


