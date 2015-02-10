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
    
    //! The page manages objects and links.
    /**
     The page is...
     */
    class Page : public enable_shared_from_this<Page>
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
        
    private:
        
        //! @internal Trigger notification to controlers.
        void send(sObject object, Notification type);
        void send(sLink link, Notification type);
        
        sObject createObject(scDico dico);
        sLink   createLink(scDico dico);
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Page(sInstance instance);
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ~Page();
        
        //! The page creation method.
        /** The function allocates a page and initialize the defaults objects.
         @param instance The instance that will manage the page.
         @param dico The dico that will initialize the page.
         @return The page.
         */
        static sPage create(sInstance instance, sDico dico = nullptr);
		
        //! Retrieve the instance that manages the page.
        /** The function retrieves the instance that manages the page.
         @return The instance that manages the page.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }
            
        //! Retrieve the shared pointer of the page.
        /** The function retrieves the shared pointer of the page.
         @return The shared pointer of the page.
         */
        inline scPage getShared() const noexcept
        {
            return static_pointer_cast<const Page>(shared_from_this());
        }
            
        //! Retrieve the shared pointer of the page.
        /** The function retrieves the shared pointer of the page.
         @return The shared pointer of the page.
         */
        inline sPage getShared() noexcept
        {
            return static_pointer_cast<Page>(shared_from_this());
        }

        //! Get the objects.
        /** The function retrieves the objects from the page.
         @param objects   A vector of elements.
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
        /** The function retrieves the links from the page.
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
        /** The function reads a dico and add the objects and links to the page.
         @param dico The dico.
         */
        void add(scDico dico);
        
        //! Free a object.
        /** The function removes a object from the page.
         @param object        The pointer to the object.
         */
        void remove(sObject object);
        
        //! Free a link.
        /** The function removes a link from the page.
         @param link        The pointer to the link.
         */
        void remove(sLink link);
        
        //! Replace a object with another one.
        /** The function instantiates a object with a dico that will replace an old object.
         @param object        The object to replace.
         @param dico       The dico that defines a object.
         @return A pointer to the object.
         */
        sObject replace(sObject object, sDico dico);
        
        //! Bring a object to the front of the page.
        /** The function brings a object to the front of the page. The object will be setted as if it was the last object created and will be the last object of the vector of objects.
         @param object        The pointer to the object.
         */
        void toFront(sObject object);
        
        //! Bring a object to the back of the page.
        /** The function brings a object to the back of the page. The object will be setted as if it was the first object created and will be the first object of the vector of objects.
         @param object        The pointer to the object.
         */
        void toBack(sObject object);
        
        //! Write the page in a dico.
        /** The function writes the pagein a dico.
         @param dico The dico.
         */
        void write(sDico dico) const;
        
        //! Start the dsp.
        /** The function start the dsp chain.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @return true if the page can process signal.
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
    };
    
    
    // ================================================================================ //
    //                                  PAGE LISTENER                                   //
    // ================================================================================ //
    
    //! The page listener is an abstract class that facilitates the control of a page in an application.
    /**
     The page listener should be a shared pointer to be able to bind itself to a page. Thus, like in all the kiwi classes, you should use another creation method and call the bind function in it. The page listener owns a vector of object listeners and facilitates managements of objects like the creation, the deletion, the selection, etc.
     @see Page, Page::Listener, Object::Listener
     */
    class Page::Listener
    {
    public:
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         Please use the create method instead.
         @param page The page to control.
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
        /** The function is called by the page when a object has been created.
         @param object     The object.
         */
        virtual void objectCreated(sPage page, sObject object) = 0;
        
        //! Receive the notification that a object has been removed.
        /** The function is called by the page when a object has been removed.
         @param object     The object.
         */
        virtual void objectRemoved(sPage page, sObject object) = 0;
        
        //! Receive the notification that a link has been created.
        /** The function is called by the page when a link has been created.
         @param link     The link.
         */
        virtual void linkCreated(sPage page, sLink link) = 0;
        
        //! Receive the notification that a link has been removed.
        /** The function is called by the page when a link has been removed.
         @param link    The link.
         */
        virtual void linkRemoved(sPage page, sLink link) = 0;
    };
}


#endif


