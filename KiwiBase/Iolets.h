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

#include "Attribute.h"

namespace Kiwi
{
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Iolet : public Attr::Listener, public enable_shared_from_this<Iolet>
    {
    public:
        enum Type
        {
            Message = 0,
            Signal  = 1,
            Both    = 2
        };
        
        enum Polarity
        {
            Cold   = 0,
            Hot    = 1
        };
        
        struct Connection
        {
            wBox box;
            unsigned long index;
            bool operator<(Connection const& other) const noexcept;
        };
        
    protected:

        vector<Connection>    m_connections;
        const Iolet::Type     m_type;
        const Iolet::Polarity m_polarity;
        const string          m_description;
        mutable mutex         m_mutex;
        
        //! Check if a connection is in the iolet.
        /** The functions checks if a connection is in the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection is in the iolet, otherwise false.
         */
        bool has(sBox box, unsigned long index) const noexcept;
        
        //! Append a new connection to the iolet.
        /** The functions appends a new connection to the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection has been added, otherwise false.
         */
        bool append(sBox box, unsigned long index) noexcept;
        
        //! Remove a connection from the iolet.
        /** The functions removes a connection from the iolet.
         @param box The box.
         @param index the iolet's index.
         @return true if the connection has been removed, otherwise false.
         */
        bool erase(sBox box, unsigned long index) noexcept;
        
    public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Iolet(Iolet::Type type, Iolet::Polarity polarity, string const& description) noexcept :
        m_type(type),
        m_polarity(polarity),
        m_description(description)
        {
            ;
        }
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Iolet()
        {
            m_connections.clear();
        }
        
        //! Retrieve the type of the iolet.
        /** The functions retrieves the type of the iolet.
         @return The type of the iolet.
         */
        inline Iolet::Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the polarity of the iolet.
        /** The functions retrieves the polarity of the iolet.
         @return The polarity of the iolet.
         */
        inline Iolet::Polarity getPolarity() const noexcept
        {
            return m_polarity;
        }
        
        //! Retrieve the description of the iolet.
        /** The functions retrieves the description of the iolet.
         @return The description of the iolet.
         */
        inline string getDescription() const noexcept
        {
            return m_description;
        }
        
        //! Retrieve the number of connections.
        /** The functions retrieves the number of connections of the iolet.
         @return The number of connections.
         */
        inline unsigned long getNumberOfConnection() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (unsigned long)m_connections.size();
        }
        
        //! Retrieve the a connection.
        /** The functions retrieves a connection.
         @param index The index of the connection.
         @return The connection.
         */
        inline Connection getConnection(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (unsigned long)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index];
            }
            else
            {
                return {sBox(), 0};
            }
        }
        
        //! Retrieve the box of a connection.
        /** The functions retrieves the box of a connection.
         @param index The index of the connection.
         @return The box of a connection.
         */
        inline sBox getBox(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (unsigned long)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index].box.lock();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the iolet's index of a connection.
        /** The functions retrieves the iolet's index of a connection.
         @param index The index of the connection.
         @return The iolet's index of a connection.
         */
        inline unsigned long getIndex(unsigned long index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < (unsigned long)m_connections.size())
            {
                return m_connections[(vector<Connection>::size_type)index].index;
            }
            else
            {
                return 0;
            }
        }
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager		The manager that manages the attribute.
         @param attr		The attribute that has been modified.
         @param type		The type of notification.
         */
        void notify(Attr::sManager manager, sAttr attr, Attr::Notification type);
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of links.
    /**
     The inlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Inlet : public Iolet
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Inlet(Iolet::Type type, Iolet::Polarity polarity, string const& description) noexcept :
        Iolet(type, polarity, description)
        {
            ;
        }
    
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Inlet()
        {
            ;
        }
        
        //! The creation method.
        /** The function creates a shared pointer of an inlet.
         @param type        The type of the inlet.
         @param polarity    The polarity of the inlet.
         @param description The description of the inlet.
         @return The inlet.
         */
        static sInlet create(Iolet::Type type, Iolet::Polarity polarity, string const& description)
        {
            return make_shared<Inlet>(type, polarity, description);
        }
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of links.
    /**
     The outlet owns a set of links that are used to manage links in a box. It also have a type and a description.
     */
    class Outlet : public Iolet
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Outlet(Iolet::Type type, string const& description) noexcept :
        Iolet(type, Polarity::Hot, description)
        {
            ;
        }
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Outlet()
        {
            ;
        }
        
        //! The creation method.
        /** The function creates a shared pointer of an outlet.
         @param type        The type of the outlet.
         @param polarity    The polarity of the outlet.
         @param description The description of the outlet.
         @return The outlet.
         */
        static sOutlet create(Iolet::Type type, string const& description)
        {
            return make_shared<Outlet>(type, description);
        }
        
        //! Send a vector of elements to the connected inlets.
        /** The function sends of elements to the connected inlets.
         @param elements The vector of elements.
         */
        void send(ElemVector const& elements) const noexcept;
    };
}


#endif


