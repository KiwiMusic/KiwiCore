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

#ifndef __DEF_KIWI_INSTANCE__
#define __DEF_KIWI_INSTANCE__

#include "Page.h"

// TODO :
// - See how to set the input and output vector for DSP.
// - Exception (load page and dsp)
// - Add the attributes (read and write)
namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    //! The instance manages pages.
    /**
     The instance manages a set a top-level pages. You can use the listener to receive the notifications of the creation, the deletion of pages and the changes of the dsp state. All the methods should be threadsafe but you should, of course, call the dsp tick from one thread. The instance is also a beacon factory that can be used to bind and retrieve objects with a specific name.
     @see Page
     @see Beacon
     */
    class Instance : public Beacon::Factory, public enable_shared_from_this<Instance>
    {
    public:
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        typedef weak_ptr<Listener>          wListener;
        typedef shared_ptr<const Listener>  scListener;
        typedef weak_ptr<const Listener>    wcListener;
        
    private:
        vector<sPage>           m_dsp_pages;
        mutable mutex           m_dsp_mutex;
        atomic_bool             m_dsp_running;
        atomic_ulong            m_sample_rate;
        atomic_ulong            m_vector_size;
        
        set<sPage>              m_pages;
        mutex                   m_pages_mutex;
        set<wListener,
        owner_less<wListener>>  m_lists;
        mutable mutex           m_lists_mutex;
        
    public:
        
        //! The constructor.
        /** You should never use this method, please use the create method instead.
		 @see create
         */
        Instance() noexcept;
        
        //! The destructor.
        ~Instance();
        
        //! The instance creation method.
        /** The function allocates an instance and initialize the prototypes of objects.
         @return The instance.
         */
        static sInstance create();
        
        //! Create a page.
        /** The function creates a page with a dico or creates an empty one if the dico is empty.
         @param dico The dico that defines of the page.
         @return The page.
         @see removePage, getPages
         */
        sPage createPage(sDico dico = nullptr);
        
        //! Close a page.
        /** The function closes page.
         @param page The page.
         @see createPage, getPages
         */
        void removePage(sPage page);
        
        //! Retreive all the pages of the instance.
        /** The function retreives all the pages of the instance.
         @param pages A vector that will be filled with the pages.
         @see createPage, removePage
         */
        void getPages(vector<sPage>& pages);
        
        //! Start the dsp.
        /** The function start the dsp chain of all the pages.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @see getVectorSize, getSampleRate, dspTick, dspStop
         */
        void dspStart(ulong samplerate, ulong vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain of all the pages.
         @see getVectorSize, getSampleRate, dspStart, dspStop
         */
        inline void dspTick() const noexcept
        {
            m_dsp_mutex.lock();
            for(vector<sPage>::size_type i = 0; i < m_dsp_pages.size(); i++)
            {
                m_dsp_pages[i]->dspTick();
            }
            m_dsp_mutex.unlock();
        }
        
        //! Stop the dsp.
        /** The function stop the dsp chain of all the pages.
         @see getVectorSize, getSampleRate, dspStart, dspTick
         */
        void dspStop();
        
        //! Check if the dsp is running.
        /** The function checks if the dsp is running
         @return true if the dsp can be ticked otherwise false.
         */
        inline bool isDspRunning() const noexcept
        {
            return m_dsp_running;
        }
        
        //! Retrieve the current sample rate.
        /** The function retrieve the current or the last sample rate used for dsp.
         @return the sample rate.
         @see getVectorSize, dspStart
         */
        inline ulong getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        //! Retrieve the current vector size of the signal.
        /** The function retrieve the current or the last vector size of the signal.
         @return the vector size of the signal.
         @see getSampleRate, dspStart
         */
        inline ulong getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        //! Add an instance listener in the binding list of the instance.
        /** The function adds an instance listener in the binding list of the instance. 
		 If the instance listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see unbind
         */
        void addListener(sListener listener);
        
        //! Remove an instance listener from the binding list of the instance.
        /** The function removes an instance listener from the binding list of the instance. 
		 If the instance listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see bind
         */
        void removeListener(sListener listener);
    };
    
    // ================================================================================ //
    //                              INSTANCE LISTENER                                   //
    // ================================================================================ //
    
    //! The instance listener is a virtual class that can bind itself to an instance and be notified of several changes.
    /** The instance listener is a very light class with methods that receive the notifications of the creation and deletion of pages and from dsp changes. An instance listener must create a shared pointer to be binded to an instance.
     @see Instance
     */
    class Instance::Listener
    {
    public:
        //! The constructor.
        /** The constructor does nothing.
         */
        Listener()
        {
            ;
        }
        
        //! The destructor.
        /** The destructor does nothing.
         */
        virtual ~Listener()
        {
            ;
        }
        
        //! Receive the notification that a page has been created.
        /** The function is called by the instance when a page has been created.
         @param instance    The instance.
         @param page        The page.
         */
        virtual void pageCreated(sInstance instance, sPage page) = 0;
        
        //! Receive the notification that a page has been closed.
        /** The function is called by the instance when a page has been closed.
         @param instance    The instance.
         @param page        The page.
         */
        virtual void pageRemoved(sInstance instance, sPage page) = 0;
        
        //! Receive the notification that the dsp has been started.
        /** The function is called by the instance when the dsp has been started.
         @param instance    The instance.
         */
        virtual void dspStarted(sInstance instance) = 0;
        
        //! Receive the notification that the dsp has been stopped.
        /** The function is called by the instance when the dsp has been stopped.
         @param instance    The instance.
         */
        virtual void dspStopped(sInstance instance) = 0;
    };
    
    // ================================================================================ //
    //                                      OBJECT FACTORY                              //
    // ================================================================================ //
    
    //! The factory
    /** The factory is the kiwi's counterpart of Andy Warhol factory.
     */
    class Factory
    {
    private:
        
        class Creator
        {
        public:
            virtual ~Creator(){};
            virtual sObject create(Detail const& init) = 0;
        };
        
        
        template <class T> class CreatorTyped : public Creator
        {
        public:
            sObject create(Detail const& init) override
            {
                return make_shared<T>(init);
            }
        };
        
        static map<sTag, shared_ptr<Creator>> m_creators;
        static mutex m_mutex;
    public:
        
        //! Add an object to the factory.
        /** This function adds a new object to the factory. If the name of the object already exists, the function doesn't do anything otherwise the object is added to the factory.
         @name  The alias name of the object.
         */
        template <class T,
        typename = typename enable_if<
        is_base_of<Object, T>::value &&
        is_base_of<Sketcher, T>::value &&
        !is_abstract<T>::value &&
        is_constructible<T, Detail const&>::value        
        >::type>
        static void add(sTag name = Tag::create(""))
        {
            sObject object = make_shared<T>(Detail());
            if(object)
            {
                const sTag rname = (name == Tag::create("")) ? object->getName() : name;
                lock_guard<mutex> guard(m_mutex);
                if(m_creators.find(rname) != m_creators.end())
                {
                    Console::error("The object " + toString(rname) + " already exist !");
                }
                else
                {
                    m_creators[rname] = make_shared<CreatorTyped<T>>();
                }
            }
            else
            {
                Console::error("The prototype of an object has a wrong constructor !");
            }
        }
        
        //! Create an object.
        /** This function creates an object.
         @param name The name of the object.
         @param node The detail to initialize the object.
         @return An object.
         */
        static sObject create(sTag name, Detail const& detail);
        
        //! Retrieves if an object exist.
        /** This function retrieves if an object exist.
         @return true if the object exist, otherwise false.
         */
        static bool has(sTag name);
        
        //! Retrieves all the names of the objects.
        /** This function retrieves all the names of the object.
         @return A vector of tags with the names.
         */
        static vector<sTag> names();
    };
}


#endif


