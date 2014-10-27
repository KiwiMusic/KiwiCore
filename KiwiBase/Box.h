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

#ifndef __DEF_KIWI_BOX__
#define __DEF_KIWI_BOX__

#include "Object.h"
#include "Attribute.h"
#include "Connection.h"

namespace Kiwi
{
    class Instance;
    class Page;
    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical object.
    /**
     The box is a graphical object that aims to be instantiate in a page. It inherites from the object and from the attribute owner. The main feature is the managing of inlets and outlets.
     @see Object, Inlet, Outlet.
     */
    
    class Box : public Object, public AttributeFactory
    {
        friend class Page;
        
    private:

        weak_ptr<Page>  m_page;
        vector<sInlet>  m_inlets;
        vector<sOutlet> m_outlets;
        size_t          m_last_inlet;
        size_t          m_stack_count;
        size_t          m_id;
        
        //! Set the id of the boxe.
        /** The function sets the id of the boxe.
         @param _id    The id number.
         */
        inline void setId(size_t _id) noexcept
        {
            m_id = _id;
        }
        
        //! Set the page of the boxe.
        /** The function sets the page of the boxe.
         @param page   The page.
         */
        inline void setPage(weak_ptr<Page>  page) noexcept
        {
            m_page = page;
        }
        
        // ================================================================================ //
        //                                      INLETS                                      //
        // ================================================================================ //

    protected:
        
        //! Add a new inlet to the box.
        /** The function adds a new inlet to the box.
        @param type The type of the inlet.
        @param description The description of the inlet.
         */
        void    addInlet(Inlet::Type type, string const& description = "");
        
        //! Remove an inlet from the box.
        /** The function removes an inlet from the box.
         @param index The index of the inlet
         */
        void    removeInlet(size_t index);
        
        //! Retrieve the index of the inlet that received the message.
        /** The functions retrieves the index of the inlet that received the message. This function should be used in the the "receive" function.
         @return The inlet index.
         */
        size_t  getInletIndex();
        
    public:
        
        //! Retrieve the number of inlets of the box.
        /** The functions retrieves the number of inlets of the box.
         @return The number of inlets.
         */
        size_t getNumberOfInlets() const noexcept;
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The inlet index.
         @return The inlet.
         */
        shared_ptr<Inlet> getInlet(size_t index) const noexcept;
        
        //! The receive method that should be override called by a specific inlet. You should never have to use this method.
        /** The function shoulds perform some stuff.
         @param index The index of the inlet.
         @param elements A list of elements to pass.
         */
        void receive(size_t index, ElemVector const& values);
        
        // ================================================================================ //
        //                                      OUTLETS                                     //
        // ================================================================================ //
    
    protected:
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    addOutlet(Outlet::Type type, string const& description = "");
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The index of the outlet.
         */
        void    removeOutlet(size_t index);
        
        //! Send a vector of elements via an outlet.
        /** The function sends a vector of elements via an outlet and dispatches it to all the connected inlets.
         @param index The index of the outlet.
         @param elements A list of elements to pass.
         */
        void    send(size_t index, ElemVector const& elements) const noexcept;
        
    public:
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        size_t  getNumberOfOutlets() const noexcept;
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The index of the outlet.
         @return The outlet.
         */
        sOutlet getOutlet(size_t index) const noexcept;
        
    public:
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        Box(shared_ptr<Instance> kiwi, string const& name);
        
        //! Constructor.
        /** Set up the default pointers and tags.
         @param kiwi    A pointer to the instance.
         @param name    A name for the object.
         */
        virtual ~Box();
        
        //! Retrieve the type of the object.
        /** The function retrieves the type of the object.
         @return The type of the object.
         */
        virtual Type type() const noexcept override;
        
        //! Write the object in a dico.
        /** The function writes the name of the object in a dico.
         @param dico The dico.
         */
        virtual void write(shared_ptr<Dico> dico) const noexcept override;
        
        //! Read a dico.
        /** The function reads a dico but doesn't do anything.
         @param dico The dico.
         */
        virtual void read(shared_ptr<const Dico> dico) noexcept override;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param elements    A list of elements to pass.
         */
        virtual void receive(ElemVector const& elements) override;
        
        //! Retreive the id of the boxe.
        /** The function retreives the id of the boxe.
         @param elements    A list of elements to pass.
         */
        inline size_t getId() const noexcept
        {
            return m_id;
        }
    };
    
    typedef shared_ptr<Box>      sBox;
}


#endif


