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

#ifndef __DEF_KIWI_PAGE__
#define __DEF_KIWI_PAGE__

#include "Object.h"
#include "Link.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The patcher manages objects and links.
    /**
     The patcher is...
     */
	class Patcher : virtual public Attr::Manager
	{
    public:
		class Listener;
		typedef shared_ptr<Listener>			sListener;
		typedef weak_ptr<Listener>              wListener;
		typedef shared_ptr<const Listener>      scListener;
        typedef weak_ptr<const Listener>        swListener;
		
        /** Flags describing the type of the notification
         @see Controler
         */
		enum Notification : bool
        {
            Added        = false,
            Removed      = true
        };
			
    private:
        const wInstance             m_instance;
        Dsp::sContext               m_dsp_context;
        vector<sObject>             m_objects;
        vector<sLink>               m_links;
        vector<ulong>               m_free_ids;
        mutable mutex               m_mutex;
        set<wListener,
        owner_less<wListener>>      m_lists;
        mutable mutex               m_lists_mutex;
			
		// Patcher attributes :
		const sAttrColor			m_color_unlocked_background;
		const sAttrColor			m_color_locked_background;
		const sAttrLong				m_gridsize;
        
    private:
        
        //! @internal Trigger notification to controlers.
        void send(sObject object, Notification type);
        void send(sLink link, Notification type);
        
        //! @internal Object and link creation.
        void createObject(scDico dico);
        void createLink(scDico dico);
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Patcher(sInstance instance);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Patcher();
        
        //! The patcher creation method.
        /** The function allocates a patcher and initialize the defaults objects.
         @param instance The instance that will manage the patcher.
         @param dico The dico that will initialize the patcher.
         @return The patcher.
         */
        static sPatcher create(sInstance instance, sDico dico = nullptr);
		
        //! Retrieve the instance that manages the patcher.
        /** The function retrieves the instance that manages the patcher.
         @return The instance that manages the patcher.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
            
        //! Retrieve the shared pointer of the patcher.
        /** The function retrieves the shared pointer of the patcher.
         @return The shared pointer of the patcher.
         */
        inline scPatcher getShared() const noexcept
        {
            return dynamic_pointer_cast<const Patcher>(shared_from_this());
        }
            
        //! Retrieve the shared pointer of the patcher.
        /** The function retrieves the shared pointer of the patcher.
         @return The shared pointer of the patcher.
         */
        inline sPatcher getShared() noexcept
        {
            return dynamic_pointer_cast<Patcher>(shared_from_this());
        }

        //! Get the objects.
        /** The function retrieves the objects from the patcher.
         @param objects   A vector of atoms.
         */
        void getObjects(vector<sObject>& objects) const
        {
            lock_guard<mutex> guard(m_mutex);
            objects = m_objects;
        }
        
        //! Get the number of objects.
        /** The function retrieves the number of objects.
         @return The number of objects.
         */
        ulong getNumberOfObjects() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return m_objects.size();
        }
        
        //! Get the links.
        /** The function retrieves the links from the patcher.
         @param links   A vector of links.
         */
        void getLinks(vector<sLink>& links) const
        {
            lock_guard<mutex> guard(m_mutex);
            links = m_links;
        }

        //! Check if the dsp is running.
        /** The function checks if the dsp is running
         */
        inline bool isDspRunning() const noexcept
        {
            return (bool)m_dsp_context.use_count();
        }
        
        //! Append a dico.
        /** The function reads a dico and add the objects and links to the patcher.
         @param dico The dico.
         */
        void add(scDico dico);
        
        //! Free a object.
        /** The function removes a object from the patcher.
         @param object        The pointer to the object.
         */
        void remove(sObject object);
        
        //! Free a link.
        /** The function removes a link from the patcher.
         @param link        The pointer to the link.
         */
        void remove(sLink link);
        
        //! Bring a object to the front of the patcher.
        /** The function brings a object to the front of the patcher. The object will be setted as if it was the last object created and will be the last object of the vector of objects.
         @param object        The pointer to the object.
         */
        void toFront(sObject object);
        
        //! Bring a object to the back of the patcher.
        /** The function brings a object to the back of the patcher. The object will be setted as if it was the first object created and will be the first object of the vector of objects.
         @param object        The pointer to the object.
         */
        void toBack(sObject object);
        
        //! Write the patcher in a dico.
        /** The function writes the patcherin a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
        //! Start the dsp.
        /** The function start the dsp chain.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @return true if the patcher can process signal.
         */
        void dspStart(const ulong samplerate, const ulong vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain. You should never call this method if the dsp hasn't been started before.
         */
        inline void dspTick() const noexcept
        {
            m_dsp_context->tick();
        }
        
        //! Stop the dsp.
        /** The function stop the dsp chain.
         */
        void dspStop();
		
		//! Add a listener to the object.
		/** The function adds a listener to the object.
		 @param list    The listener.
		 */
		void addListener(sListener list);
        
        //! Remove a listener from the object.
        /** The function removes a listener from the object.
         @param list    The listener.
         */
        void removeListener(sListener list);
		
		//! Notify the manager that the values of an attribute has changed.
		/** The function notifies the manager that the values of an attribute has changed.
		 @param attr An attribute.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		virtual bool notify(sAttr attr) {return true;};
			
		//! Retrieve the "gridsize" attribute value of the patcher.
		/** The function retrieves the "gridsize" attribute value of the patcher.
		@return The "gridsize" attribute value of the patcher.
		*/
		inline long getGridSize() const noexcept
		{
			return m_gridsize->getValue();
		}
			
		//! Retrieve the "locked_bgcolor" attribute value of the patcher.
		/** The function retrieves the "locked_bgcolor" attribute value of the patcher.
		 @return The "locked_bgcolor" attribute value of the patcher.
		 */
		inline Color getLockedBackgroundColor() const noexcept
		{
			return m_color_locked_background->getValue();
		}
			
		//! Retrieve the "locked_bgcolor" attribute value of the patcher.
		/** The function retrieves the "locked_bgcolor" attribute value of the patcher.
		 @return The "locked_bgcolor" attribute value of the patcher.
		 */
		inline Color getUnlockedBackgroundColor() const noexcept
		{
			return m_color_unlocked_background->getValue();
		}
    };
		
		
    // ================================================================================ //
    //                                  PAGE LISTENER                                   //
    // ================================================================================ //
    
    //! The patcher listener is an abstract class that facilitates the control of a patcher in an application.
    /**
     The patcher listener should be a shared pointer to be able to bind itself to a patcher. Thus, like in all the kiwi classes, you should use another creation method and call the bind function in it. The patcher listener owns a vector of object listeners and facilitates managements of objects like the creation, the deletion, the selection, etc.
     @see Patcher, Patcher::Listener, Object::Listener
     */
    class Patcher::Listener
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         Please use the create method instead.
         @param patcher The patcher to control.
         */
        Listener() noexcept
        {
            ;
        }
        
        //! The destructor.
        /** The destructor.
         */
        virtual ~Listener()
        {
            ;
        }
        
        //! Receive the notification that a object has been created.
        /** The function is called by the patcher when a object has been created.
         @param object     The object.
         */
        virtual void objectCreated(sPatcher patcher, sObject object) = 0;
        
        //! Receive the notification that a object has been removed.
        /** The function is called by the patcher when a object has been removed.
         @param object     The object.
         */
        virtual void objectRemoved(sPatcher patcher, sObject object) = 0;
        
        //! Receive the notification that a link has been created.
        /** The function is called by the patcher when a link has been created.
         @param link     The link.
         */
        virtual void linkCreated(sPatcher patcher, sLink link) = 0;
        
        //! Receive the notification that a link has been removed.
        /** The function is called by the patcher when a link has been removed.
         @param link    The link.
         */
        virtual void linkRemoved(sPatcher patcher, sLink link) = 0;
    };
}


#endif


