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

#ifndef __DEF_KIWI_INLET__
#define __DEF_KIWI_INLET__

#include "Object.h"
#include "Box.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! The inlet owns to a box and is used to route the messages from the outlets to its box.
    /**
     The inlet is opaque, you just have to worry about the type and the description when you create them in a box.
     */
    class Box::Inlet
    {
    private:
        friend class Outlet;
        friend class Box;
        
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
        
    public:
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
}


#endif


