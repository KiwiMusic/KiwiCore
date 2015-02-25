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
     The instance manages a set a top-level patchers. You can use the listener to receive the notifications of the creation, the deletion of patchers and the changes of the dsp state. The instance is also a beacon factory that can be used to bind and retrieve objects with a specific name.
     @see Patcher
     @see Beacon
     */
    class Instance : public Beacon::Factory, public DspContext
    {
    public:
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        typedef weak_ptr<Listener>          wListener;
        typedef shared_ptr<const Listener>  scListener;
        typedef weak_ptr<const Listener>    wcListener;
        
    private:        
        const string            m_name;
        set<sPatcher>           m_patchers;
        mutex                   m_patchers_mutex;
        set<wListener,
        owner_less<wListener>>  m_lists;
        mutable mutex           m_lists_mutex;
        
    public:
        
        //! The constructor.
        /** You should never use this method.
		 @param device The device manager.
         */
        Instance(sDspDeviceManager device, string const& name) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Instance();
        
        //! The instance creation method.
        /** The function allocates an instance and initialize the prototypes of objects.
         @param device The device manager.
         @return The instance.
         */
        static sInstance create(sDspDeviceManager device, string const& name);
        
        //! Retrieve the shared pointer of the instance.
        /** The function retrieves the shared pointer of the instance.
         @return The shared pointer of the instance.
         */
        inline scInstance getShared() const noexcept
        {
            return static_pointer_cast<const Instance>(DspContext::shared_from_this());
        }
        
        //! Retrieve the shared pointer of the instance.
        /** The function retrieves the shared instance of the instance.
         @return The shared pointer of the patcher.
         */
        inline sInstance getShared() noexcept
        {
            return static_pointer_cast<Instance>(DspContext::shared_from_this());
        }
        
        //! Retrieve the name of the instance.
        /** The function retrieves the name of the instance.
         @return The name of the instance.
         */
        inline string getName() const noexcept
        {
            return m_name;
        }
        
        //! Create a patcher with a dict.
        /** The function creates a patcher with a dict.
         @param dico The dico that defines of the patcher.
         @return The patcher.
         @see removePatcher, getPatchers
         */
        sPatcher createPatcher();
        
        //! Create a patcher with a dict.
        /** The function creates a patcher with a dict.
         @param dico The dico that defines of the patcher.
         @return The patcher.
         @see removePatcher, getPatchers
         */
        sPatcher createPatcher(Dico& dico);
        
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
        
        //! Creation method of the instance.
        /** The function evaluates the inputs for the creation and returns the result in the outputs.
         @param inputs The vector of atoms to evaluate.
         @param outputs The vector of atoms to return the creation.
         */
        void create(Vector const& inputs, Vector outputs);
        
        //! Remove method of the instance.
        /** The function evaluates the inputs for the deletion.
         @param inputs The vector of atoms to evaluate.
         */
        void remove(Vector const& inputs);
        
        //! Get method of the instance.
        /** The function evaluates the inputs for the getting and returns the result in the outputs.
         @param inputs The vector of atoms to evaluate.
         @param outputs The vector of atoms to return the getting.
         */
        void get(Vector const& inputs, Vector outputs) const;
        
        //! Set method of the instance.
        /** The function evaluates the inputs for the setting.
         @param inputs The vector of atoms to evaluate.
         */
        void set(Vector const& inputs);
        
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
                    int todo;
                    //Console::error("The object " + toString(rname) + " already exist !");
                }
                else
                {
                    m_creators[rname] = make_shared<CreatorTyped<T>>();
                }
            }
            else
            {
                int todo;
                //Console::error("The prototype of an object has a wrong constructor !");
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


