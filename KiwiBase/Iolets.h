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

#include "Tools.h"

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      SOCKET                                      //
    // ================================================================================ //
    
    class Socket
    {
    public:
        wBox          box;
        unsigned long index;
        
        Socket() noexcept
        {
            ;
        }
        
        Socket(sBox _box, const unsigned long _index) noexcept :
        box(_box), index(_index)
        {
            ;
        }
        
        static sSocket create(sBox _box, const unsigned long _index)
        {
            return make_shared<Socket>(_box, _index);
        }
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of sockets.
    /**
     The inlet owns a set of sockets that are used to manage connections in a box. It also have a type and a description.
     */
    class Inlet
    {
    public:
        enum Type
        {
            DataCold    = 0,
            DataHot     = 1,
            Signal      = 2,
            Error       = 3
        };
    private:
        
        vector<shared_ptr<Socket>> m_sockets;
        const Type      m_type;
        const string    m_description;
    
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Inlet(Type type, string description) noexcept :
        m_type(type),
        m_description(description)
        {
            ;
        }
    
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Inlet()
        {
            m_sockets.clear();
        }
        
        //! Retrieve the type of the inlet.
        /** The functions retrieves the type of the inlet.
         @return The type of the inlet.
         */
        inline Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the description of the inlet.
        /** The functions retrieves the description of the inlet.
         @return The description of the inlet.
         */
        inline string getDescription() const noexcept
        {
            return m_description;
        }
        
        //! Retrieve the number of sockets.
        /** The functions retrieves the number of sockets of the inlet.
         @return The number of sockets.
         */
        inline unsigned long size() const noexcept
        {
            return (unsigned long)m_sockets.size();
        }
        
        //! Retrieve the box of a socket.
        /** The functions retrieves the box of a socket.
         @param index The index of the socket.
         @return The box of a socket.
         */
        inline sBox getBox(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index]->box.lock();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the outlet's index of a socket.
        /** The functions retrieves the outlet's index of a socket.
         @param index The index of the socket.
         @return The outlet's index of a socket.
         */
        inline unsigned long getOutletIndex(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index]->index;
            }
            else
            {
                return 0;
            }
        }
        
        //! Append a new socket to the inlet.
        /** The functions appends a new socket to the inlet.
         @param index The box of the socket.
         @param index The outlet's index of the socket.
         @return true if the socket has been added, otherwise false.
         */
        bool append(sBox box, unsigned long index) noexcept;
        
        //! Remove a socket from the inlet.
        /** The functions removes a socket from the inlet.
         @param index The box of the socket.
         @param index The outlet's index of the socket.
         @return true if the socket has been removed, otherwise false.
         */
        bool erase(sBox box, unsigned long index) noexcept;
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of sockets.
    /**
     The outlet owns a set of sockets that are used to manage connections in a box. It also have a type and a description.
     */
    class Outlet
    {
    public:
        enum Type
        {
            Data    = 0,
            Signal  = 2,
            Error   = 3
        };
        
    private:
        vector<sSocket> m_sockets;
        const Type      m_type;
        const string    m_description;
        
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Outlet(Type type, string description) noexcept :
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
            m_sockets.clear();
        }
        
        //! Retrieve the type of the outlet.
        /** The functions retrieves the type of the outlet.
         @return The type of the outlet.
         */
        inline Type getType() const noexcept
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
        
        //! Retrieve the number of sockets.
        /** The functions retrieves the number of sockets of the outlet.
         @return The number of sockets.
         */
        inline unsigned long size() const noexcept
        {
            return (unsigned long)m_sockets.size();
        }
        
        //! Retrieve the box of a socket.
        /** The functions retrieves the box of a socket.
         @param index The index of the socket.
         @return The box of a socket.
         */
        inline sBox getBox(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index]->box.lock();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the inlet's index of a socket.
        /** The functions retrieves the inlet's index of a socket.
         @param index The index of the socket.
         @return The inlet's index of a socket.
         */
        inline unsigned long getInletIndex(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index]->index;
            }
            else
            {
                return 0;
            }
        }
        
        //! Append a new socket to the outlet.
        /** The functions appends a new socket to the outlet.
         @param index The box of the socket.
         @param index The inlet's index of the socket.
         @return true if the socket has been added, otherwise false.
         */
        bool append(sBox box, unsigned long index) noexcept;
        
        //! Remove a socket from the outlet.
        /** The functions removes a socket from the outlet.
         @param index The box of the socket.
         @param index The inlet's index of the socket.
         @return true if the socket has been removed, otherwise false.
         */
        bool erase(sBox box, unsigned long index) noexcept;
    };
}


#endif


