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
    class InstanceListener;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    //! The instance manages pages.
    /**
     The instance manages a set a top-level pages. You can use the listener to receive the notifications of the creation, the deletion of pages and the changes of the dsp state. All the methods should be threadsafe but you should, of course, call the dsp tick from one thread. The instance is also a beacon factory that can be used to bind and retrieve boxes with a specific name.
     @see Page
     @see Beacon
     */
    class Instance : public Beacon::Factory, public enable_shared_from_this<Instance>
    {
    public:
        class Listener;
    private:
        vector<sPage>                       m_dsp_pages;
        mutable mutex                       m_dsp_mutex;
        atomic_bool                         m_dsp_running;
        atomic_long                         m_sample_rate;
        atomic_long                         m_vector_size;
        
        unordered_set<sPage>                m_pages;
        mutex                               m_pages_mutex;
        
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>           m_listeners;
        mutex                               m_listeners_mutex;
        
    public:
        
        //! The constructor.
        /** You should never use this method.
         */
        Instance() noexcept;
        
        //! The constructor.
        /** You should never use this method.
         */
        ~Instance();
        
        //! The instance creation method.
        /** The function allocates an instance and initialize the prototypes of boxes.
         @return The instance.
         */
        static sInstance create();
        
        //! Create a page.
        /** The function creates a page with a dico or creates an empty one if the dico is empty.
         @param dico The dico that defines of the page.
         @return The page.
         @see removePage()
         @see getPages()
         */
        sPage createPage(scDico dico = nullptr);
        
        //! Close a page.
        /** The function closes page.
         @param page The page.
         @see createPage()
         @see getPages()
         */
        void removePage(sPage page);
        
        //! Retreive all the pages of the instance.
        /** The function retreives all the pages of the instance.
         @param pages A vector that will be filled with the pages.
         @see createPage()
         @see removePage()
         */
        void getPages(vector<sPage>& pages);
        
        //! Start the dsp.
        /** The function start the dsp chain of all the pages.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         @see getVectorSize()
         @see getSampleRate()
         @see tickDsp()
         @see stopDsp()
         */
        void startDsp(long samplerate, long vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain of all the pages.
         @see getVectorSize()
         @see getSampleRate()
         @see startDsp()
         @see stopDsp()
         */
        inline void tickDsp() const noexcept
        {
            m_dsp_mutex.lock();
            for(vector<sPage>::size_type i = 0; i < m_dsp_pages.size(); i++)
            {
                m_dsp_pages[i]->tickDsp();
            }
            m_dsp_mutex.unlock();
        }
        
        //! Stop the dsp.
        /** The function stop the dsp chain of all the pages.
         @see getVectorSize()
         @see getSampleRate()
         @see startDsp()
         @see tickDsp()
         */
        void stopDsp();
        
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
         @see getVectorSize()
         @see startDsp()
         */
        inline long getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        //! Retrieve the current vector size of the signal.
        /** The function retrieve the current or the last vector size of the signal.
         @return the vector size of the signal.
         @see getSampleRate()
         @see startDsp()
         */
        inline long getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        //! Add an instance listener in the binding list of the instance.
        /** The function adds an instance listener in the binding list of the instance. If the instance listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see              unbind()
         */
        void bind(shared_ptr<Listener> listener);
        
        //! Remove an instance listener from the binding list of the instance.
        /** The function removes an instance listener from the binding list of the instance. If the instance listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see           bind()
         */
        void unbind(shared_ptr<Listener> listener);
        
        // ================================================================================ //
        //                              INSTANCE LISTENER                                   //
        // ================================================================================ //
        
        //! The instance listener is a virtual class that can bind itself to an instance and be notified of several changes.
        /**
         The instance listener is a very light class with methods that receive the notifications of the creation and deletion of pages and from dsp changes. An instance listener must create a shared pointer to be binded to an instance.
         @see Instance
         */
        class Listener
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
            virtual void pageHasBeenCreated(sInstance instance, sPage page){};
            
            //! Receive the notification that a page has been closed.
            /** The function is called by the instance when a page has been closed.
             @param instance    The instance.
             @param page        The page.
             */
            virtual void pageHasBeenRemoved(sInstance instance, sPage page){};
            
            //! Receive the notification that the dsp has been started.
            /** The function is called by the instance when the dsp has been started.
             @param instance    The instance.
             */
            virtual void dspHasBeenStarted(sInstance instance){};
            
            //! Receive the notification that the dsp has been stopped.
            /** The function is called by the instance when the dsp has been stopped.
             @param instance    The instance.
             */
            virtual void dspHasBeenStopped(sInstance instance){};
        };
        
        typedef shared_ptr<Listener>    sListener;
    };
}


#endif


