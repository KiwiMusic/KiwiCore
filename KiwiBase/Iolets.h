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
    
    //! The socket owns a box and an index of inlet or outlet.
    /**
     The socket is a half part of a link and is used by inlets and outlets to send messages between boxes.
     */
    class Socket
    {
    private:
        const wBox          m_box;
        const unsigned long m_index;
        
     public:
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Socket(const sBox box, const unsigned long index) noexcept :
        m_box(box), m_index(index)
        {
            ;
        }
        
        ~Socket()
        {
            ;
        }
        
        //! The socket creation method.
        /** The function allocates a socket.
         @param box    The box.
         @param index  The index.
         @return The socket.
         */
        static sSocket create(const sBox box, const unsigned long index)
        {
            if(box)
            {
                return make_shared<Socket>(box, index);
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the box of the socket.
        /** The functions retrieves the box of the socket.
         @return The box of the socket.
         */
        inline sBox getBox() const noexcept
        {
            return m_box.lock();
        }
        
        //! Retrieve the index of the socket.
        /** The functions retrieves the index of the socket.
         @return The index of the socket.
         */
        inline unsigned long getIndex() const noexcept
        {
            return m_index;
        }
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns a set of sockets.
    /**
     The inlet owns a set of sockets that are used to manage links in a box. It also have a type and a description.
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
        
        vector<sSocket> m_sockets;
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
        inline unsigned long getNumberOfSockets() const noexcept
        {
            return (unsigned long)m_sockets.size();
        }
        
        //! Retrieve a socket.
        /** The functions retrieves a socket.
         @param index The index of the socket.
         @return The socket.
         */
        inline sSocket getSocket(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index];
            }
            else
            {
                return 0;
            }
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
                return m_sockets[(vector<sSocket>::size_type)index]->getBox();
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
                return m_sockets[(vector<sSocket>::size_type)index]->getIndex();
            }
            else
            {
                return 0;
            }
        }
        
        //! Check if a socket is in the inlet.
        /** The functions checks if a socket is in the inlet.
         @param socket The socket.
         @return true if the socket is in the inlet, otherwise false.
         */
        bool has(sSocket socket) noexcept;
        
        //! Append a new socket to the inlet.
        /** The functions appends a new socket to the inlet.
         @param sSocket The socket.
         @return true if the socket has been added, otherwise false.
         */
        bool append(sSocket socket) noexcept;
        
        //! Remove a socket from the inlet.
        /** The functions removes a socket from the inlet.
         @param index The box of the socket.
         @param index The outlet's index of the socket.
         @return true if the socket has been removed, otherwise false.
         */
        bool erase(const sSocket socket) noexcept;
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns a set of sockets.
    /**
     The outlet owns a set of sockets that are used to manage links in a box. It also have a type and a description.
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
        inline unsigned long getNumberOfSockets() const noexcept
        {
            return (unsigned long)m_sockets.size();
        }
        
        //! Retrieve a socket.
        /** The functions retrieves a socket.
         @param index The index of the socket.
         @return The socket.
         */
        inline sSocket getSocket(unsigned long index) const noexcept
        {
            if(index < (unsigned long)m_sockets.size())
            {
                return m_sockets[(vector<sSocket>::size_type)index];
            }
            else
            {
                return 0;
            }
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
                return m_sockets[(vector<sSocket>::size_type)index]->getBox();
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
                return m_sockets[(vector<sSocket>::size_type)index]->getIndex();
            }
            else
            {
                return 0;
            }
        }
        
        //! Check if a socket is in the outlet.
        /** The functions checks if a socket is in the outlet.
         @param socket The socket.
         @return true if the socket is in the inlet, otherwise false.
         */
        bool has(sSocket socket) noexcept;
        
        //! Append a new socket to the outlet.
        /** The functions appends a new socket to the outlet.
         @param index The box of the socket.
         @param index The inlet's index of the socket.
         @return true if the socket has been added, otherwise false.
         */
        bool append(sSocket socket) noexcept;
        
        //! Remove a socket from the outlet.
        /** The functions removes a socket from the outlet.
         @param index The box of the socket.
         @param index The inlet's index of the socket.
         @return true if the socket has been removed, otherwise false.
         */
        bool erase(const sSocket socket) noexcept;
    };
}


#endif


