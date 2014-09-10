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

#ifndef __DEF_KIWI_IOLET__
#define __DEF_KIWI_IOLET__

#include "Object.h"
#include "Box.h"
#include "Page.h"

namespace Kiwi
{
    class Page;
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns to a box and is used to route the messages from the outlets to its box.
    /**
     The inlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Box::Inlet
    {
    public:
        friend class Outlet;
        
        const weak_ptr<Object>  m_owner;
        int                     m_index;
        set<shared_ptr<Tag>>    m_names;
        string                  m_description;
        bool                    m_polarity;
        
        //! Call the bang method of the owner.
        /** The function calls the bang method of the owner.
         */
        void receiveBang() const noexcept;
        
        //! Call the long method of the owner with a value.
        /** The function calls the long method of the owner.
         @param long The long value to pass.
         */
        void receiveLong(long value) const noexcept;
        
        //! Call the double method of the owner with a value.
        /** The function calls the double method of the owner.
         @param double The double value to pass.
         */
        void receiveDouble(double value) const noexcept;
        
        //! Call the tag method of the owner with a value.
        /** The function calls the tag method of the owner.
         @param tag The tag to pass.
         */
        void receiveTag(shared_ptr<Tag> tag) const noexcept;
        
        //! Call the elements method of the owner with a value.
        /** The function calls the elements method of the owner.
         @param elements The vector of elements to pass.
         */
        void receiveElements(vector<Element>& elements) const noexcept;
        
        //! Call a named method of the owner with a long value.
        /** The function calls a named method of the owner.
         @param name The name of the method.
         @param long The long value to pass.
         */
        void receiveOther(const shared_ptr<Tag> name, long value) const noexcept;
        
        //! Call a named method of the owner with a double value.
        /** The function calls a named method of the owner.
         @param name The name of the method.
         @param double The double value to pass.
         */
        void receiveOther(const shared_ptr<Tag> name, double value) const noexcept;
        
        //! Call a named method of the owner with a tag.
        /** The function calls a named method of the owner.
         @param name The name of the method.
         @param tag The tag to pass.
         */
        void receiveOther(const shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept;
        
        //! Call a named method of the owner with elements.
        /** The function calls a named method of the owner.
         @param name The name of the method.
         @param elements The vector of elements to pass.
         */
        void receiveOther(const shared_ptr<Tag> name, vector<Element>& elements) const noexcept;
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Inlet(const shared_ptr<Object> owner, int index);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Inlet();
        
        //! Retrieve the owner of the inlet.
        /** The function retrieves the owner of the inlet.
         @return The owner of the inlet.
         */
        weak_ptr<Object> owner() const noexcept;
        
        //! Retrieve the index of the inlet.
        /** The function retrieves the index of the inlet.
         @return The index of the inlet.
         */
        int index() const noexcept;
        
        //! Retrieve the description of the inlet.
        /** The function retrieves the description of the inlet.
         @return The description of the inlet.
         */
        std::string description() const noexcept;
        
        //! Retrieve the polarity of the inlet.
        /** The function retrieves the polarity of the inlet.
         @return The polarity of the inlet.
         */
        bool polarity() const noexcept;
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns to a box and is used to send the messages to the inlets.
    /**
     The outlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Box::Outlet
    {
    public:
        const weak_ptr<Object>  m_owner;
        int                     m_index;
        set<shared_ptr<Tag>>    m_names;
        string                  m_description;
        set<shared_ptr<Inlet>>  m_inlets;
        
        //! Check if an inlet is compatible with the outlet
        /** The function checks if the inlet is compatible with the outlet
         @param inlet       The inlet.
         @return True if the inlet is compatible otherwise false.
         */
        bool compatible(const shared_ptr<Inlet> inlet);
        
        //! Connect an inlet to the outlet
        /** The function connects an inlet to the outlet
         @param inlet       The inlet.
         @return True if the inlet has been connected otherwise false.
         */
        bool connect(const shared_ptr<Inlet> inlet);
        
        //! Disconnect an inlet to the outlet
        /** The function disconnects an inlet to the outlet
         @param inlet       The inlet.
         */
        void disconnect(const shared_ptr<Inlet> inlet);
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Outlet(const weak_ptr<Object> owner, int index);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Outlet();
        
        //! Retrieve the owner of the outlet.
        /** The function retrieves the owner of the outlet.
         @return The owner of the outlet.
         */
        weak_ptr<Object> owner() const noexcept;
        
        //! Retrieve the index of the outlet.
        /** The function retrieves the index of the outlet.
         @return The index of the outlet.
         */
        int index() const noexcept;
        
        //! Retrieve the description of the outlet.
        /** The function retrieves the description of the outlet.
         @return The description of the outlet.
         */
        std::string description() const noexcept;
        
        //! Send a bang.
        /** The function sends a bang to all the connected inlets.
         */
        void sendBang() const noexcept;
        
        //! Send a long value with the method name long.
        /** The function sends a long value to all the connected inlets.
         @param value The long value.
         */
        void sendLong(long value) const noexcept;
        
        //! Send a double value with the method name double.
        /** The function sends a double value to all the connected inlets.
         @param value The double value.
         */
        void sendDouble(double value) const noexcept;
        
        //! Send a tag with the method name tag.
        /** The function sends a tag to all the connected inlets.
         @param tag The tag.
         */
        void sendTag(shared_ptr<Tag> tag) const noexcept;
        
        //! Send elements with the method name elements.
        /** The function sends a vector of elements to all the connected inlets.
         @param elements The vector of elements.
         */
        void sendElements(vector<Element>& elements) const noexcept;
        
        //! Send long value with another method name.
        /** The function sends a long value to all the connected inlets.
         @param name The name that matchs to a method.
         @param long The long value.
         */
        void sendOther(const shared_ptr<Tag> name, long value) const noexcept;
        
        //! Send double value with another method name.
        /** The function sends a double value to all the connected inlets.
         @param name    The name that matchs to a method.
         @param double The double value.
         */
        void sendOther(const shared_ptr<Tag> name, double value) const noexcept;
        
        //! Send tag with another method name.
        /** The function sends a tag to all the connected inlets.
         @param name    The name that matchs to a method.
         @param tag     The tag.
         */
        void sendOther(const shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept;
        
        //! Send elements with another method name.
        /** The function sends a vector of elements to all the connected inlets.
         @param name        The name that matchs to a method.
         @param elements    The vector of elements.
         */
        void sendOther(const shared_ptr<Tag> name, vector<Element>& elements) const noexcept;
    };

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
        const weak_ptr<Instance> m_kiwi;
        const weak_ptr<Box> m_from;
        const int           m_outlet;
        const weak_ptr<Box> m_to;
        const int           m_inlet;
        bool                m_selected;

    public:
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        Connection(const shared_ptr<Instance> kiwi, const shared_ptr<Box> from, int outlet, const shared_ptr<Box> to, int inlet);
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~Connection();
        
        //! Check if the connection carries signal.
        /** The function checks if the connection carries signal.
         @return True is the connection carries signal otherwise it returns false.
         */
        bool isDsp();
        
        shared_ptr<Box> getFrom();
        int getOutletIndex();
        shared_ptr<Box> getTo();
        
        //! Retrieve the index of inlet.
        /** The function retrieves the index of inlet.
         @return The index of inlet.
         */
        int getInletIndex();
        
        //! Write the connection in a dico.
        /** The function writes the connection in a dico.
         @param dico The dico.
         */
        void write(shared_ptr<Dictionary> dico);
        
        inline bool operator==(const Connection& other) noexcept
        {
            if(!other.m_to.lock())
            {
                if(other.m_inlet < 0)
                    return this->m_from.lock() == other.m_from.lock();
                else
                    return this->m_from.lock() == other.m_from.lock() && this->m_outlet == other.m_outlet;
            }
            else if(!other.m_from.lock())
            {
                if(other.m_outlet < 0)
                    return this->m_to.lock() == other.m_to.lock();
                else
                    return this->m_to.lock() == other.m_to.lock() && this->m_inlet == other.m_inlet;
            }
            else
                return this->m_from.lock() == other.m_from.lock() && this->m_outlet == other.m_outlet && this->m_to.lock() == other.m_to.lock() && this->m_inlet == other.m_inlet;
        }
    };
}


#endif


