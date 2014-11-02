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

namespace Kiwi
{
    class InstanceListener;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    class Instance : public enable_shared_from_this<Instance>
    {
    public:
        class Listener;
    private:
        unordered_set<sPage>                m_pages;
        unordered_set<weak_ptr<Listener>,
        weak_ptr_hash<Listener>,
        weak_ptr_equal<Listener>>           m_listeners;
        bool                                m_dsp_running;
        
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        Instance() noexcept;
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        ~Instance();
        
        //! The instance creation method.
        /** The function allocates an instance and initialize the defaults boxes.
         */
        static shared_ptr<Instance> create();
        
        //! Create a page.
        /** The function creates a page with a dico or creates an empty one if the dico is empty.
         @param dico The dico that defines of the page.
         @return The page.
         */
        sPage createPage(sDico dico);
        
        //! Close a page.
        /** The function closes page.
         @param page The page.
         */
        void removePage(sPage page);
        
        //! Start the dsp.
        /** The function start the dsp chain of all the pages.
         @param samplerate The sample rate.
         @param vectorsize The vector size of the signal.
         */
        void startDsp(double samplerate, long vectorsize);
        
        //! Perform a tick on the dsp.
        /** The function calls once the dsp chain of all the pages.
         */
        void tickDsp() const noexcept;
        
        //! Stop the dsp.
        /** The function stop the dsp chain of all the pages.
         */
        void stopDsp();
        
        //! Check if the dsp is running.
        /** The function checks if the dsp is running
         */
        bool isDspRunning() const noexcept;
        
        //! Add an instance listener in the binding list of the instance.
        /** The function adds an instance listener in the binding list of the instance. If the instance listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see              unbind()
         */
        void bind(weak_ptr<Listener> listener);
        
        //! Remove an instance listener from the binding list of the instance.
        /** The function removes an instance listener from the binding list of the instance. If the instance listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see           bind()
         */
        void unbind(weak_ptr<Listener> listener);
        
        // ================================================================================ //
        //                              INSTANCE LISTENER                                   //
        // ================================================================================ //
        
        //! The instance listener is a virtual class that can bind itself to an instance and be notified of the several changes.
        /**
         The instance listener is a very light class with methods that receive the notifications of the creation and the deletion of pages.
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
            virtual void pageHasBeenCreated(shared_ptr<Instance> instance, sPage page){};
            
            //! Receive the notification that a page has been closed.
            /** The function is called by the instance when a page has been closed.
             @param instance    The instance.
             @param page        The page.
             */
            virtual void pageHasBeenRemoved(shared_ptr<Instance> instance, sPage page){};
        };
        
        typedef shared_ptr<Listener>    sListener;
    };
    
    typedef shared_ptr<Instance>    sInstance;
    
    typedef weak_ptr<Instance>      wInstance;
}


#endif


