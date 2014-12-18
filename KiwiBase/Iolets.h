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

#ifndef __DEF_KIWI_IOLETS__
#define __DEF_KIWI_IOLETS__

#include "Link.h"

namespace Kiwi
{
    enum IoType
    {
        Data    = 0,
        Signal  = 1,
        Both    = 2
    };
    
    enum IoPolarity
    {
        Cold   = 0,
        Hot    = 1
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of links.
    /**
     The inlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Inlet
    {
    private:
        
        vector<sLink>       m_links;
        const IoType        m_type;
        const IoPolarity    m_polarity;
        const string        m_description;
        
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Inlet(IoType type, IoPolarity polarity, string description) noexcept :
        m_type(type),
        m_polarity(polarity),
        m_description(description)
        {
            ;
        }
    
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Inlet()
        {
            m_links.clear();
        }
        
        //! Retrieve the type of the inlet.
        /** The functions retrieves the type of the inlet.
         @return The type of the inlet.
         */
        inline IoType getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the polarity of the inlet.
        /** The functions retrieves the polarity of the inlet.
         @return The polarity of the inlet.
         */
        inline IoPolarity getPolarity() const noexcept
        {
            return m_polarity;
        }
        
        //! Retrieve the description of the inlet.
        /** The functions retrieves the description of the inlet.
         @return The description of the inlet.
         */
        inline string getDescription() const noexcept
        {
            return m_description;
        }
        
        //! Retrieve the number of links.
        /** The functions retrieves the number of links of the inlet.
         @return The number of links.
         */
        inline unsigned long getNumberOfLinks() const noexcept
        {
            return (unsigned long)m_links.size();
        }
        
        //! Retrieve a link.
        /** The functions retrieves a link.
         @param index The index of the link.
         @return The link.
         */
        inline sLink getLink(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index];
            }
            else
            {
                return 0;
            }
        }
        
        //! Retrieve the box of a link.
        /** The functions retrieves the box of a link.
         @param index The index of the link.
         @return The box of a link.
         */
        inline sBox getBox(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index]->getBoxFrom();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the outlet's index of a link.
        /** The functions retrieves the outlet's index of a link.
         @param index The index of the link.
         @return The outlet's index of a link.
         */
        inline unsigned long getOutletIndex(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index]->getOutletIndex();
            }
            else
            {
                return 0;
            }
        }
        
        //! Check if a link is in the inlet.
        /** The functions checks if a link is in the inlet.
         @param link The link.
         @return true if the link is in the inlet, otherwise false.
         */
        bool has(sLink link) noexcept;
        
        //! Append a new link to the inlet.
        /** The functions appends a new link to the inlet.
         @param sLink The link.
         @return true if the link has been added, otherwise false.
         */
        bool append(sLink link) noexcept;
        
        //! Remove a link from the inlet.
        /** The functions removes a link from the inlet.
         @param index The box of the link.
         @param index The outlet's index of the link.
         @return true if the link has been removed, otherwise false.
         */
        bool erase(sLink link) noexcept;
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Outlet
    {
    private:
        vector<sLink> m_links;
        const IoType  m_type;
        const string  m_description;
        
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Outlet(IoType type, string description) noexcept :
        m_type(type),
        m_description(description)
        {
            ;
        }
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Outlet()
        {
            m_links.clear();
        }
        
        //! Retrieve the type of the outlet.
        /** The functions retrieves the type of the outlet.
         @return The type of the outlet.
         */
        inline IoType getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the description of the outlet.
        /** The functions retrieves the description of the outlet.
         @return The description of the outlet.
         */
        inline string getDescription() const noexcept
        {
            return m_description;
        }
        
        //! Retrieve the number of links.
        /** The functions retrieves the number of links of the outlet.
         @return The number of links.
         */
        inline unsigned long getNumberOfLinks() const noexcept
        {
            return (unsigned long)m_links.size();
        }
        
        //! Retrieve a link.
        /** The functions retrieves a link.
         @param index The index of the link.
         @return The link.
         */
        inline sLink getLink(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index];
            }
            else
            {
                return 0;
            }
        }
        
        //! Retrieve the box of a link.
        /** The functions retrieves the box of a link.
         @param index The index of the link.
         @return The box of a link.
         */
        inline sBox getBox(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index]->getBoxTo();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the inlet's index of a link.
        /** The functions retrieves the inlet's index of a link.
         @param index The index of the link.
         @return The inlet's index of a link.
         */
        inline unsigned long getInletIndex(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_links.size())
            {
                return m_links[(vector<sLink>::size_type)index]->getInletIndex();
            }
            else
            {
                return 0;
            }
        }
        
        //! Check if a link is in the outlet.
        /** The functions checks if a link is in the outlet.
         @param link The link.
         @return true if the link is in the inlet, otherwise false.
         */
        bool has(sLink link) noexcept;
        
        //! Append a new link to the outlet.
        /** The functions appends a new link to the outlet.
         @param index The box of the link.
         @param index The inlet's index of the link.
         @return true if the link has been added, otherwise false.
         */
        bool append(sLink link) noexcept;
        
        //! Remove a link from the outlet.
        /** The functions removes a link from the outlet.
         @param index The box of the link.
         @param index The inlet's index of the link.
         @return true if the link has been removed, otherwise false.
         */
        bool erase(sLink link) noexcept;
    };
}


#endif


