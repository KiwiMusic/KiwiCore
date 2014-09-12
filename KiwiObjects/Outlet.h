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

#ifndef __DEF_KIWI_OUTLET__
#define __DEF_KIWI_OUTLET__

#include "Object.h"
#include "Box.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! The outlet owns to a box and is used to send the messages to the inlets.
    /**
     The outlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Box::Outlet
    {
        friend class Box;
    private:
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
        
    public:
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
}


#endif


