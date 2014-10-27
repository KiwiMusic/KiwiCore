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

#ifndef __DEF_KIWI_CONNECTION__
#define __DEF_KIWI_CONNECTION__

#include "Defs.h"

namespace Kiwi
{
    class Box;
    class Dico;
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet is used to route the messages from the outlets to its box.
    /**
     The inlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Inlet
    {
    public:
        friend class Box;
        
        enum Type
        {
            DataCold    = 0,
            DataHot     = 1,
            Signal      = 2
        };
        
    private:
        
        const Type              m_flag;
        size_t                  m_index;
        
        void setIndex(size_t index);
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Inlet(Type type);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Inlet();
        
        //! Retrieve the index of the inlet.
        /** The function retrieves the index of the inlet.
         @return The index of the inlet.
         */
        size_t index() const noexcept;
        
        //! Retrieve the polarity of the inlet.
        /** The function retrieves the polarity of the inlet.
         @return The polarity of the inlet.
         */
        Type type() const noexcept;
        
    };
    
    typedef shared_ptr<Inlet> sInlet;
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns to a box and is used to send the messages to the inlets.
    /**
     The outlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Outlet
    {
    public:
        friend class Page;
        friend class Box;
        
        enum Type
        {
            Data    = 0,
            Signal  = 1
        };
        
    private:
        const Type                              m_type;
        size_t                                  m_index;
        map<shared_ptr<Inlet>, shared_ptr<Box>> m_inlets;
        
        //! Connect an inlet to the outlet
        /** The function connects an inlet to the outlet
         @param inlet       The inlet.
         @return True if the inlet has been connected otherwise false.
         */
        void connect(const shared_ptr<Inlet> inlet, shared_ptr<Box> box);
        
        //! Disconnect an inlet to the outlet
        /** The function disconnects an inlet to the outlet
         @param inlet       The inlet.
         */
        void disconnect(const shared_ptr<Inlet> inlet);
        
        void setIndex(size_t index);
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Outlet(Type type);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Outlet();
        
        //! Retrieve the index of the outlet.
        /** The function retrieves the index of the outlet.
         @return The index of the outlet.
         */
        size_t index() const noexcept;
        
        //! Retrieve the polarity of the inlet.
        /** The function retrieves the polarity of the inlet.
         @return The polarity of the inlet.
         */
        Type type() const noexcept;
        
        //! Retrieve the polarity of the inlet.
        /** The function retrieves the polarity of the inlet.
         @return The polarity of the inlet.
         */
        bool has(sInlet inlet) const noexcept;
    };
    
    typedef shared_ptr<Outlet> sOutlet;
    
    // ================================================================================ //
    //                                      CONNECTION                                  //
    // ================================================================================ //
    
    //! The connection owns to a page and is used to create a patch lines.
    /**
     The connection is opaque, you shouldn't have to use it at all.
     */
    class Connection
    {
    private:

        const weak_ptr<Box>     m_from;
        const weak_ptr<Box>     m_to;
        const weak_ptr<Outlet>  m_outlet;
        const weak_ptr<Inlet>   m_inlet;
        
    public:
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Connection(const shared_ptr<Box> from, const shared_ptr<Outlet> outlet, const shared_ptr<Box> to, const shared_ptr<Inlet> inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Connection();
        
        //! Retrieve the output box.
        /** The function retrieves the output box.
         @return The output box.
         */
        shared_ptr<Outlet> outlet();
        
        //! Retrieve the output box.
        /** The function retrieves the output box.
         @return The output box.
         */
        shared_ptr<Inlet> inlet();
        
        //! Write the connection in a dico.
        /** The function writes the connection in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dico> dico);
    };
}


#endif


