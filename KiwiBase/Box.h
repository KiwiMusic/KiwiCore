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

#include "Iolets.h"
#include "AttributeBox.h"
#include "Event.h"
#include "Doodle.h"
#include "Text.h"
#include "Beacon.h"
#include "Clock.h"
//#include "Page.h"

// TODO
// - See how to format the expression
// - Box should deletes it owns links at deletion
// - Links and Iolets behavior (resize ect..)
namespace Kiwi
{    
    // ================================================================================ //
    //                                      BOX                                         //
    // ================================================================================ //
    
    //! The box is a graphical box.
    /**
     The box is a graphical class that aims to be instantiate in a page.
     */
	class Box : public AttrBox
    {
    public:
        class Controller;
        typedef shared_ptr<Controller>   sController;
        typedef weak_ptr<Controller>     wController;
        
        enum Behavior
        {
            Signal      = 1<<1,
            Mouse       = 1<<2,
            Keyboard    = 1<<3,
            Graphic     = 1<<4
        };
        
    private:
        friend void Outlet::send(ElemVector const& elements) const noexcept;
        
        const wInstance     m_instance;
        const wPage         m_page;
        const sTag          m_name;
        const ulong         m_id;
        const ulong         m_flags;
        sTag                m_text;
        
        vector<sOutlet>     m_outlets;
        vector<sInlet>      m_inlets;
        atomic_ullong       m_stack_count;
        mutable mutex       m_io_mutex;
        
        wController         m_controller;
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Box(sPage page, string const& name, ulong type = 1<<0);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Box();
        
        //! The box creation method.
        /** The function allocates a box and initialize the defaults members.
         */
        static sBox create(sPage page, sDico dico);
    
        //! Retrieve the sBox.
        /** The function sBox.
         @return The sBox.
         */
		inline sBox getShared() noexcept
		{
			return static_pointer_cast<Box>(shared_from_this());
		}
        
        //! Retrieve the scBox.
        /** The function scBox.
         @return The scBox.
         */
		inline scBox getShared() const noexcept
		{
			return static_pointer_cast<const Box>(shared_from_this());
		}
        
        //! Retrieve the instance that manages the page of the box.
        /** The function retrieves the instance that manages the page of the box.
         @return The instance that manages the page of the box.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
        
        //! Retrieve the page that manages the box.
        /** The function retrieves the page that manages the box.
         @return The page that manages the box.
         */
        inline sPage getPage() const noexcept
        {
            return m_page.lock();
        }
        
        //! Retrieve the controller that manages the box.
        /** The function retrieves the controller that manages the box.
         @return The controller that manages the box.
         */
        inline sController getController() const noexcept
        {
            return m_controller.lock();
        }
        
        //! Retrieve the name of the box.
        /** The function retrieves the name of the box as a tag.
         @return The name of the box as a tag.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the id of the box.
        /** The function retrieves the id of the box as a tag.
         @return The text of the id as a tag.
         */
        inline ulong getId() const noexcept
        {
            return m_id;
        }
        
        //! Retrieve the flags of the box.
        /** The function retrieves the flags of the box.
         @return The flags of the box.
         */
        inline ulong getFlags() const noexcept
        {
            return m_flags;
        }
		
		//! Retrieve if the box is a graphical object.
		/** The function retrieves if the box is a graphical object.
		 @return true if the box is a graphical object otherwise false.
		 */
		inline bool isGUI() const noexcept
		{
			return m_flags & Graphic;
		}
		
		//! Retrieve if the box is a DSP object.
		/** The function retrieves if the box is a DSP object.
		 @return true if the box is a DSP object otherwise false.
		 */
		inline bool isDSP() const noexcept
		{
			return m_flags & Signal;
		}
		
		//! Retrieve if the box wants the mouse focus.
		/** The function retrieves if the box wants the mouse focus.
		 @return true if the box wants the mouse focus otherwise false.
		 */
		inline bool isMouseListener() const noexcept
		{
			return m_flags & Mouse;
		}
		
		//! Retrieve if the box wants the keyboard focus.
		/** The function retrieves if the box wants the keyboard focus.
		 @return true if the box wants the keyboard focus otherwise false.
		 */
		inline bool isKeyboardListener() const noexcept
		{
			return m_flags & Keyboard;
		}
		
        //! Retrieve the text of the box.
        /** The function retrieves the text of the box as a tag.
         @return The text of the box as a tag.
         */
        inline sTag getText() const noexcept
        {
            return m_text;
        }
        
        //! Retrieve the expression of the box.
        /** The function retrieves the expression of the box as a string.
         @return The expression of the box as a string.
         */
        virtual string getExpression() const noexcept
        {
            return "error";
        }
        
        //! Retrieve the number of inlets of the box.
        /** The functions retrieves the number of inlets of the box.
         @return The number of inlets.
         */
        inline ulong getNumberOfInlets() const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            return (ulong)m_inlets.size();
        }
        
        //! Retrieve an inlet.
        /** The functions retrieves an inlet.
         @param index The inlet's index.
         @return The inlet.
         */
        inline sInlet getInlet(ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_inlets.size())
            {
                return m_inlets[(vector<sInlet>::size_type)index];
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Retrieve the number of outlets of the box.
        /** The functions retrieves the number of outlets of the box.
         @return The number of outlets.
         */
        inline ulong getNumberOfOutlets() const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            return (ulong)m_outlets.size();
        }
        
        //! Retrieve an outlet.
        /** The functions retrieves an outlet.
         @param index The outlet's index.
         @return The outlet.
         */
        inline sOutlet getOutlet(ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_io_mutex);
            if(index < m_outlets.size())
            {
                return m_outlets[(vector<sOutlet>::size_type)index];
            }
            else
            {
                return nullptr;
            }
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if the vector of element doesn't match with your method then the box will check if the vector match with attributes methods, othersize return true.
         @param elements    A list of elements to pass.
         */
        virtual bool receive(ulong index, ElemVector const& elements)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if you don't want the mouse event then the box manager will notify other mouse listener if needed, othersize return true.
         @param event    A mouse event.
         */
        virtual bool receive(Gui::Event::Mouse const& event)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if you don't want the keyboard event then the box manager will notifiy other keyboard listener if needed, othersize return true.
         @param event    A keyboard event.
         */
        virtual bool receive(Gui::Event::Keyboard const& event)
        {
            return false;
        }
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff. Return false if you don't want the focus event then the box manager will notifiy other keyboard listener if needed, othersize return true.
         @param event    A focus event.
         */
        virtual bool receive(Gui::Event::Focus::Type event)
        {
            return false;
        }
        
        //! The paint method that should be override.
        /** The function shoulds draw some stuff in the doodle. Return false if you don't want to draw then the box manager will draw the text of the box, othersize return true.
         @param doodle    A doodle to draw.
         */
        virtual bool draw(Gui::Doodle& doodle) const
        {
            return false;
        }
        
        //! Write the box in a dico.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
    protected:
        
        //! Send a notification to the controler that the box should be redraw.
        /** The function sends a notification to the controler that the box should be redraw.
         */
        void    redraw() const noexcept;
        
        //! Send a notification to the controler that the box should be redraw.
        /** The function sends a notification to the controler that the box should be redraw.
         */
        void    grabKeyboardFocus() const noexcept;
        
        //! Send a vector of elements via an outlet.
        /** The function sends a vector of elements via an outlet and dispatches it to all the connected inlets.
         @param index The index of the outlet.
         @param elements A list of elements to pass.
         */
        void    send(ulong index, ElemVector const& elements) const noexcept;
        
        //! Add a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    addInlet(Iolet::Type type, Iolet::Polarity polarity, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @param index The index of the inlet;
         @param type The type of the inlet.
         @param description The description of the inlet.
         */
        void    insertInlet(ulong index, Iolet::Type type, Iolet::Polarity polarity, string const& description = "");
        
        //! Remove an inlet from the box.
        /** The function removes an inlet from the box.
         @param index The index of the inlet
         */
        void    removeInlet(ulong index);
        
        //! Add a new outlet the the box.
        /** The function adds a new outlet the the box.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    addOutlet(Iolet::Type type, string const& description = "");
        
        //! Insert a new inlet to the box.
        /** The function adds a new inlet to the box.
         @pram index The index of the outlet.
         @param type The type of the outlet.
         @param description The description of the outlet.
         */
        void    insertOutlet(ulong index, Iolet::Type type, string const& description = "");
        
        //! Remove an outlet.
        /** The function removes an outlet.
         @param index The index of the outlet.
         */
        void    removeOutlet(ulong index);

    private:
        
        //! The virtual constructor called by the page to create an instance of box.
        /** The function retrieves an instance of the box.
         @param dico        The dico that defines the box.
         */
        virtual sBox allocate(sPage page, sDico dico) const = 0;
        
        //! The write method that should be override.
        /** The function writes the box in a dico.
         @param dico The dico.
         */
		virtual void save(sDico dico) const {};
		
        //! The read method that should be override.
        /** The function reads a dico to initalize the boxe.
         @param dico The dico.
         */
		virtual void load(scDico dico) {};
		
        //! Notify the manager that the values of an attribute has changed.
		/** The function notifies the manager that the values of an attribute has changed.
		 @param attr The attribute that has changed.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
        virtual bool attributeChanged(sAttr attr)
        {
            return true;
        }
		
		//! Notify the manager that the values of an attribute has changed.
		/** The function notifies the manager that the values of an attribute has changed.
		 @param attr The attribute that has changed.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		bool attributeValueChanged(sAttr attr);

        //! Set the controller of the box.
        /** The function sets the controller of the box.
         @param ctrl    The controller.
         */
        void setController(sController ctrl);
        
    public:
        
        // ================================================================================ //
        //                                  BOX CONTROLER                                   //
        // ================================================================================ //
        
        //! The box controller .
        /**
         The box controller...
         */
		class Controller
        {
        private:
            
            const sBox		m_box;
			const bool		m_boxgui;
			const bool		m_boxdsp;
            const bool  	m_want_mouse_focus;
            const bool		m_want_keyboard_focus;
			const double	m_framesize;
			
            bool			m_selected;
			bool			m_page_edition_status;
			bool			m_page_presentation_status;
        public:
            
            //! Constructor.
            /** You should never call this method except if you really know what you're doing.
             */
            Controller(sBox box) :
            m_box(box),
            m_boxgui(box->getFlags() & Box::Graphic),
            m_boxdsp(box->getFlags() & Box::Signal),
			m_want_mouse_focus(box->getFlags() & Box::Mouse),
			m_want_keyboard_focus(box->getFlags() & Box::Keyboard),
			m_framesize(4.),
            m_selected(false),
			m_page_edition_status(true),
			m_page_presentation_status(false)
            {
                ;
            }
            
            //! The destructor.
            /** You should never call this method except if you really know what you're doing.
             */
            virtual ~Controller()
            {
                ;
            }
            
            //! The controller maker.
            /** The function creates a controller with arguments.
             */
            template<class CtrlClass, class ...Args> static shared_ptr<CtrlClass> create(Args&& ...arguments)
            {
                shared_ptr<CtrlClass> ctrl = make_shared<CtrlClass>(forward<Args>(arguments)...);
                if(ctrl && ctrl->m_box)
                {
                    ctrl->m_box->setController(ctrl);
                }
                return ctrl;
            }
            
            //! Retrieve the box.
            /** The funtion retrieves the box.
             @return The box.
             */
            inline sBox getBox() const noexcept
            {
                return m_box;
            }
            
            //! Retrieve the text of the box.
            /** The funtion retrieves the text of the box.
             @return The text of the box.
             */
            inline string getText() const noexcept
            {
                return toString(m_box->getText());
            }
            
            //! Retrieve if the page is in edition.
            /** The function retrieves if the page is in edition.
             @param true if the page is in edition, otherwise false.
             */
            inline bool getPageEditionStatus() const noexcept
            {
                return m_page_edition_status;
            }
			
			//! Retrieve if the page is in presentation mode.
			/** The function retrieves if the page is in presentation mode.
			 @param true if the page is in presentation mode, otherwise false.
			 */
			inline bool getPagePresentationStatus() const noexcept
			{
				return m_page_presentation_status;
			}
			
            //! Retrieve if the box is selected.
            /** The function retrieves if the box is selected.
             @param true if the box is selected, otherwise false.
             */
            inline bool isSelected() const noexcept
            {
                return m_selected;
            }
			
			//! Retrieve if the box is a graphical object.
			/** The function retrieves if the box is a graphical object.
			 @return true if the box is a graphical object otherwise false.
			 */
			inline bool isGUI() const noexcept
			{
				return m_boxgui;
			}
			
			//! Retrieve if the box is a DSP object.
			/** The function retrieves if the box is a DSP object.
			 @return true if the box is a DSP object otherwise false.
			 */
			inline bool isDSP() const noexcept
			{
				return m_boxdsp;
			}
			
            //! Retrieve if the box wants the mouse focus.
            /** The function retrieves if the box wants the mouse focus.
             @return true if the box wants the mouse focus otherwise false.
             */
            inline bool isMouseListener() const noexcept
            {
                return m_want_mouse_focus;
            }
            
            //! Retrieve if the box wants the keyboard focus.
            /** The function retrieves if the box wants the keyboard focus.
             @return true if the box wants the keyboard focus otherwise false.
             */
            inline bool isKeyboardListener() const noexcept
            {
                return m_want_keyboard_focus;
            }
			
			//! Retrieve the frame size.
			/** The function retrieves the frame size.
			 @return The frame size value.
			 */
			inline double getFrameSize() const noexcept
			{
				return m_framesize;
			}
			
			//! Retrieve the bounds of the box controller.
			/** The function retrieves the bounds of the box controller.
			 The box controller's bounds is equal to the box's bounds expanded by a framesize.
			 */
			Gui::Rectangle getBounds(const bool presentation) const noexcept;
			
			//! Retrieve the position of the box controller.
			/** The function retrieves the position of the box controller.
			 The box controller's position is equal to the box's position expanded by a framesize.
			 */
			Gui::Point getPosition(const bool presentation) const noexcept;
			
			//! Retrieve the size of the box controller.
			/** The function retrieves the size of the box controller.
			 The box controller's size is equal to the box's size expanded by a framesize.
			 */
			Gui::Point getSize(const bool presentation) const noexcept;
			
            //! Retrieve the position of an inlet.
            /** The function retrieves the position of an inlet.
             @param index The index of the inlet.
             @return the position of the inlet as a point.
             */
            Gui::Point getInletPosition(ulong index) const noexcept;
            
            //! Retrieve the position of an outlet.
            /** The function retrieves the position of an outlet.
             @param index The index of the outlet.
             @return the position of the outlet as a point.
             */
            Gui::Point getOutletPosition(ulong index) const noexcept;
            
            //! Retrieve if the box is touch by a point.
			/** The function retrieves if the box is touch by a point and fill the knock with the knock informations.
             @param point The point.
             @param knock The knock.
			 @param presentation Is the page in presentation mode ?
			 @return true if the box is touch by the point, otherwise false.
			 */
			virtual bool contains(Gui::Point const& point, Knock& knock, const bool presentation) const noexcept;
            
            //! Tests if the box overlaps the rectangle.
			/** The function tests if the box overlaps the rectangle.
             @param rect The Rectangle.
			 @param presentation Is the page in presentation mode ?
			 @return True if the box overlaps the rectangle, otherwise false.
			 */
			virtual bool overlaps(Gui::Rectangle const& rect, const bool presentation) const noexcept;
            
            //! Notify that the page is in edition.
            /** The function notifies that page is in edition to redraw the box.
             @param edition true if page is in edition, otherwise false.
             */
            void setPageEditionStatus(const bool edition);
			
			//! Notify that the page presentation status changed.
			/** The function notifies that the page presentation status changed.
			 @param presentation True if page is in presentation, otherwise false.
			 */
			void setPagePresentationStatus(const bool presentation);
            
            //! Notify that the box is selected.
            /** The function notifies that the box is selected to redraw the box.
             @param status true if the box is selected, otherwise false.
             */
            void setSelectedStatus(bool status);
            
            //! The inlets notification function that should be override.
            /** The function is called by the box when its inlets changed.
             */
            virtual void inletsChanged();
            
            //! The outlets notification function that should be override.
            /** TThe function is called by the box when its outlets changed.
             */
            virtual void outletsChanged();
            
            //! The position notification function that should be override.
            /** The function is called by the box when its position changed.
             */
			virtual void positionChanged() {};
				
            //! The size notification function that should be override.
            /** The function is called by the box when its size changed.
             */
			virtual void sizeChanged() {};
			
			//! The size notification function that should be override.
			/** The function is called by the box when its size changed.
			 */
			virtual void pageEditionStatusChanged() {};
			
			//! The page presentation notification function.
			/** The function is called by the box when page presentation mode changed.
			 */
			virtual void pagePresentationStatusChanged() {};
			
			//! Called by the box when the box selection status changed.
			/** The function is called by the box selection status changed.
			 */
			virtual void selectionStatusChanged() {};
			
			//! Called by the box when the presentation status changed.
			/** The function is called by the box when the presentation status changed.
			 */
			virtual void presentationStatusChanged() {};
			
            //! The redraw function that should be override.
            /** The function is called by the box when it should be repainted.
             */
			virtual void redraw() {};
            
            //! The grab focus function that should be override.
            /** The function is called by the box when it want to grab keyboard focus.
             */
			virtual void grabKeyboardFocus() {};
				
            //! The default paint method.
            /** The default function paint a default box with the background, border, inlets, outlets and text.
             @param doodle			A doodle to draw.
			 @param selected		If the box is selected.
             @param edit			If the page is in edition mode.
			 @param presentation    If the page is in presentation mode.
             */
            static void paintBoxFrame(sBox box, Gui::Doodle& doodle, const bool selected = false,
									  const bool edit = false, const bool presentation = false);
			
			//! The default paint method.
			/** The default function paint a default box with the background, border, inlets, outlets and text.
			 @param doodle			A doodle to draw.
			 */
			static void paintBox(sBox box, Gui::Doodle& doodle);
        };
		
        // ================================================================================ //
        //                                      BOX FACTORY                                 //
        // ================================================================================ //
        
    private:
        
        static map<sTag, unique_ptr<Box>>  m_prototypes;
        static mutex m_prototypes_mutex;
    public:
    
        //! Box factory
        /** This function adds a new prototype of a box. If the prototype already exists, the function doesn't do anything otherwise the box is added to the prototype list.
         @param     box The prototype of the box.
         */
        static void addPrototype(unique_ptr<Box> box, string const& name = "");
		
		//! Retrieves all loaded prototype names.
		/** This function retrieves all loaded prototype names.
		 @param names A vector of Tag to be filled.
		 */
		static void getPrototypeNames(vector<sTag>& names);
		
        // ================================================================================ //
        //                                      BOX COMMON TAG                              //
        // ================================================================================ //
        
        static const sTag Tag_arguments;
        static const sTag Tag_bang;
        static const sTag Tag_id;
        static const sTag Tag_focus;
        static const sTag Tag_name;
        static const sTag Tag_set;
        static const sTag Tag_text;
        
    };
    
    inline string toString(scBox box)
    {
        return toString(box->getName());
    }
}


#endif


