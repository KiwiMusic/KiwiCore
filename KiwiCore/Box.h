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

#include "Defs.h"
#include "Tag.h"
#include "Element.h"
#include "Object.h"

// Add hot and cold inlets and clean the notifications and functions

namespace Kiwi
{
    class Instance;
    class Page;
    class Connection;
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical object.
    /**
     The box is a graphical object that aims to be instantiate in a page. It inherites from the object to manage the method and from the attribute manager. The main feature is the managing of inlets and outlets. Note that the box use other methods than the default constructor and destructor to be instantiate in a page, look at the create and delete methods. If you want to create a box's child class you can have a look at the objects implementations like plus or minus.
     @see Object, Inlet, Outlet, BoxUI, BoxDSP, BoxDSPUI.
     */
    class Box : public Object
    {
    private:
        class Inlet;
        class Outlet;
        friend class Inlet;
        friend class Page;
        
        weak_ptr<Page>              m_page;
        vector<shared_ptr<Inlet>>   m_inlets;
        vector<shared_ptr<Outlet>>  m_outlets;
        int                         m_last_inlet;
        
        //! Check if an inlet is compatible with an outlet.
        /** The function checks if the inlet is compatible with the outlet.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         @return True if the inlet is compatible otherwise false.
         */
        static bool compatible(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
        
        //! Connect an outlet to an inlet.
        /** The function connects an inlet to the outlet.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         @return True if the inlet has been connected otherwise false.
         */
        static bool connect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
        
        //! Disconnect an inlet from an outlet.
        /** The function disconnects an inlet to the outlet.
         @param from        The outlet owner.
         @param outlet      The outlet index.
         @param to          The inlet owner.
         @param inlet       The inlet index.
         */
        static void disconnect(shared_ptr<Box> from, int outlet, shared_ptr<Box> to, int inlet);
    protected:
        
        //! Retrieve the index of the inlet that received the message.
        /** The functions retrieves the index of the inlet that received the message.
         @return The inlet index.
         */
        inline int getProxy()
        {
            return m_last_inlet;
        }
        
        //! Add a new inlet the the box.
        /** The function adds a new inlet the the box.
         @param size  The number of name to add.
         @param ...   The call names to add.
         */
        void    addInlet(int size, ...);
        
        //! Add a new inlet the the box.
        /** The function adds a new inlet the the box.
         @param name1 The first call name to add.
         @param ...   The other call names to add.
         */
        void    addInlet(string name1, string name2 = string(""), string name3 = string(""), string name4 = string(""), string name5 = string(""), string name6 = string(""), string name7 = string(""), string name8 = string(""), string name9 = string(""));
        
        //! Set the call names of an inlet.
        /** The function sets the call names to the inlet
         @param index The inlet's index.
         @param size  The number of name to add.
         @param ...   The call names to add.
         */
        void    setInletCallName(int index, int size, ...);
        
        //! Set the inlet's description.
        /** The sets the inlet's description.
         @param index The inlet's index.
         @param description The inlet's description.
         */
        void    setInletDescription(int index, string description);
        
        //! Set the inlet's polarity.
        /** The sets the inlet's polarity.
         @param index The inlet index.
         @param polarity The inlet's polarity.
         */
        void    setInletPolarity(int index, bool polarity);
        
        //! Set the inlet's attributes.
        /** The sets the inlet's description and polarity.
         @param index The inlet's index.
         @param description The inlet's description.
         @param polarity The inlet's polarity.
         */
        void    setInletAttributes(int index, string description, bool polarity);
        
        //! Remove an inlet.
        /** The function removes an inlet.
         @param index The inlet index.
         */
        void    removeInlet(int index);
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param size  The number of name to add.
         @param ...   The call names to add.
         */
        void    addOutlet(int size, ...);
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param name1 The first call name to add.
         @param ...   The other call names to add.
         */
        void    addOutlet(string name1, string name2 = string(""), string name3 = string(""), string name4 = string(""), string name5 = string(""), string name6 = string(""), string name7 = string(""), string name8 = string(""), string name9 = string(""));
        
        //! Set the call names of an outlet.
        /** The function sets the call names to the outlet.
         @param index The inlet outlet.
         @param size  The number of name to add.
         @param ...   The call names to add.
         */
        void    setOutletCallName(int index, int size, ...);
        
        //! Set the outlet's description.
        /** The sets the outlet's description.
         @param index The outlet index.
         @param description The outlet's description.
         */
        void    setOutletDescription(int index, string description);
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The outlet index.
         */
        void    removeOutlet(int index);
        
        //! Send a bang via an outlet.
        /** The function sends a bang to all the connected inlets.
         @param index The outlet index.
         */
        void sendBang(int index) const noexcept;
        
        //! Send a long value with the method name long via an outlet.
        /** The function sends a long value to all the connected inlets.
         @param index The outlet index.
         @param value The long value.
         */
        void sendLong(int index, long value) const noexcept;
        
        //! Send a double value with the method name double via an outlet.
        /** The function sends a double value to all the connected inlets.
         @param index The outlet index.
         @param value The double value.
         */
        void sendDouble(int index, double value) const noexcept;
        
        //! Send a tag with the method name tag via an outlet.
        /** The function sends a tag to all the connected inlets.
         @param index The outlet index.
         @param tag The tag.
         */
        void sendTag(int index, shared_ptr<Tag> tag) const noexcept;
        
        //! Send elements with the method name elements via an outlet.
        /** The function sends a vector of elements to all the connected inlets.
         @param index The outlet index.
         @param elements The vector of elements.
         */
        void sendElements(int index, vector<Element>& elements) const noexcept;
        
        //! Send long value with another method name via an outlet.
        /** The function sends a long value to all the connected inlets.
         @param index The outlet index.
         @param name The name that matchs to a method.
         @param long The long value.
         */
        void sendOther(int index, shared_ptr<Tag> name, long value) const noexcept;
        
        //! Send double value with another method name via an outlet.
        /** The function sends a double value to all the connected inlets.
         @param index The outlet index.
         @param name    The name that matchs to a method.
         @param double The double value.
         */
        void sendOther(int index, shared_ptr<Tag> name, double value) const noexcept;
        
        //! Send tag with another method name via an outlet.
        /** The function sends a tag to all the connected inlets.
         @param index The outlet index.
         @param name    The name that matchs to a method.
         @param tag     The tag.
         */
        void sendOther(int index, shared_ptr<Tag> name, shared_ptr<Tag> value) const noexcept;
        
        //! Send elements with another method name via an outlet.
        /** The function sends a vector of elements to all the connected inlets.
         @param index The outlet index.
         @param name        The name that matchs to a method.
         @param elements    The vector of elements.
         */
        void sendOther(int index, shared_ptr<Tag> name, vector<Element>& elements) const noexcept;
        
    public:
        enum InletPolarity
        {
            Cold = 0,
            Hot  = 1
        };
        
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
        inline bool isBox() const noexcept override
        {
            return true;
        }
        
        //! Check if the box performs digital signal processing.
        /** The functions checks if the box performs digital signal processing.
         @return True is the box performs digital signal processing otherwise it returns false.
         */
        bool isDsp();
        
        //! Retrieve the number of inlets of the box.
        /** The functions retrieves the number of inlets of the box.
         @return The number of inlets.
         */
        int getNumberOfInlets() const noexcept;
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        int getNumberOfOutlets() const noexcept;
        
        //! Retrieve the number of signal inlets of the box.
        /** The functions retrieves the number of signal inlets of the box.
         @return The number of signal inlets.
         */
        int getNumberOfSignalInlets() noexcept;
        
        //! Retrieve the number of signal outlets of the box.
        /** The functions retrieves the number of signal outlets of the box.
         @return The number of signal outlets.
         */
        int getNumberOfSignalOutlets() noexcept;
        
        //! Retrieve the description of an inlet.
        /** The functions retrieves the description of an inlet.
         @param index The inlet index.
         @return The description of the inlet.
         */
        string getInletDescription(int index) const noexcept;
        
        //! Retrieve the description of an outlet.
        /** The functions retrieves the description of an outlet.
         @param index The outlet index.
         @return The description of the outlet.
         */
        string getOutletDescription(int index) const noexcept;
        
        //! Retrieve the signal index of an signal inlet.
        /** The functions the signal index of an signal inlet.
         @param index The inlet index.
         @return The signal index of the inlet.
         */
        int getInletSignalIndex(int index) const noexcept;
        
        //! Retrieve the outlet index of an signal outlet.
        /** The functions the signal outlet of an signal outlet.
         @param index The inlet outlet.
         @return The signal index of the outlet.
         */
        int getOutletSignalIndex(int index) const noexcept;
        
        //! Check if an inlet carries signal.
        /** The functions checks if an inlet carries signal.
         @param index The inlet index.
         @return True is the inlet carries signal otherwise it returns false.
         */
        bool isInletSignal(int index) const noexcept;
        
        //! Check if an outlet carries signal.
        /** The functions checks if an outlet carries signal.
         @param index The outlet index.
         @return True is the outlet carries signal otherwise it returns false.
         */
        bool isOutletSignal(int index) const noexcept;
        
        static void setId(shared_ptr<Box> x, vector<Element>& elements);
        static void getId(shared_ptr<Box> x, vector<Element>& elements);
    };
}


#endif


