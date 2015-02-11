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

#include "Patcher.h"

// TODO :
// - See how to set the input and output vector for DSP.
// - Exception (load patcher and dsp)
// - Add the attributes (read and write)
namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    //! The instance manages patchers.
    /**
     The instance manages a set a top-level patchers. You can use the listener to receive the notifications of the creation, the deletion of patchers and the changes of the dsp state. All the methods should be threadsafe but you should, of course, call the dsp tick from one thread. The instance is also a beacon factory that can be used to bind and retrieve boxes with a specific name.
     @see Patcher
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
        vector<sPatcher>           m_dsp_patchers;
        mutable mutex           m_dsp_mutex;
        atomic_bool             m_dsp_running;
        atomic_ulong            m_sample_rate;
        atomic_ulong            m_vector_size;
        
        set<sPatcher>              m_patchers;
        mutex                   m_patchers_mutex;
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
        /** The function allocates an instance and initialize the prototypes of boxes.
         @return The instance.
         */
        static sInstance create();
        
        //! Create a patcher.
        /** The function creates a patcher with a dico or creates an empty one if the dico is empty.
         @param dico The dico that defines of the patcher.
         @return The patcher.
         @see removePatcher, getPatchers
         */
        sPatcher createPatcher(sDico dico = nullptr);
        
        //! Close a patcher.
        /** The function closes patcher.
         @param patcher The patcher.
         @see createPatcher, getPatchers
         */
        void removePatcher(sPatcher patcher);
        
        //! Retreive all the patchers of the instance.
        /** The function retreives all the patchers of the instance.
         @param patchers A vector that will be filled with the patchers.
         @see createPatcher, removePatcher
         */
        void getPatchers(vector<sPatcher>& patchers);
        
        //! Start the dsp.
        /** The function start the dsp chain of all the patchers.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @see getVectorSize, getSampleRate, dspTick, dspStop
         */
        void dspStart(ulong samplerate, ulong vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain of all the patchers.
         @see getVectorSize, getSampleRate, dspStart, dspStop
         */
        inline void dspTick() const noexcept
        {
            m_dsp_mutex.lock();
            for(vector<sPatcher>::size_type i = 0; i < m_dsp_patchers.size(); i++)
            {
                m_dsp_patchers[i]->dspTick();
            }
            m_dsp_mutex.unlock();
        }
        
        //! Stop the dsp.
        /** The function stop the dsp chain of all the patchers.
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
    /** The instance listener is a very light class with methods that receive the notifications of the creation and deletion of patchers and from dsp changes. An instance listener must create a shared pointer to be binded to an instance.
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
        
        //! Receive the notification that a patcher has been created.
        /** The function is called by the instance when a patcher has been created.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        virtual void patcherCreated(sInstance instance, sPatcher patcher) = 0;
        
        //! Receive the notification that a patcher has been closed.
        /** The function is called by the instance when a patcher has been closed.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        virtual void patcherRemoved(sInstance instance, sPatcher patcher) = 0;
        
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
    //                                      OBJECT FACTORY                                 //
    // ================================================================================ //
    
    class Prototypes
    {
    private:
        
        class ObjectCreator
        {
        public:
            virtual ~ObjectCreator(){};
            virtual sObject create(Initializer const& init) = 0;
        };
        
        
        template <class T> class ObjectCreatorType : public ObjectCreator
        {
        public:
            sObject create(Initializer const& init) override
            {
                return make_shared<T>(init);
            }
        };
        
        static map<sTag, shared_ptr<ObjectCreator>> m_creators;
        static mutex m_mutex;
    public:
        
        //! Object factory
        /** This function adds a new prototype of a object. If the prototype already exists, the function doesn't do anything otherwise the object is added to the prototype list.
         @param     object The prototype of the object.
         */
        template <class T> static void add(string const& name = "")
        {
            sObject object = make_shared<T>(Initializer());
            if(object)
            {
                sTag tname;
                if(name.empty())
                {
                    tname = object->getName();
                }
                else
                {
                    tname = Tag::create(name);
                }
                
                lock_guard<mutex> guard(m_mutex);
                if(m_creators.find(tname) != m_creators.end())
                {
                    Console::error("The object " + toString(object->getName()) + " already exist !");
                }
                else
                {
                    m_creators[tname] = make_shared<ObjectCreatorType<T>>();
                }
            }
            else
            {
                Console::error("The prototype of an object has a wrong constructor !");
            }
        }
        
        //! ...
        /** ...
         */
        static sObject create(sTag name, Initializer const& init);
        
        //! ...
        /** ...
         */
        static bool has(sTag name);
        
        //! Retrieves all loaded prototype names.
        /** This function retrieves all loaded prototype names.
         @param names A vector of Tag to be filled.
         */
        static void getNames(vector<sTag>& names);
    };
}


#endif


