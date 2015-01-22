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

#include "Box.h"
#include "Link.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //! The page manages boxes and links.
    /**
     The page is...
     */
    class Page : public AttrPage
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
        vector<sBox>                m_boxes;
        vector<sLink>               m_links;
        mutable mutex               m_mutex;
        set<wListener,
        owner_less<wListener>>      m_lists;
        mutable mutex               m_lists_mutex;
        
    private:
        
        //! @internal Trigger notification to controlers.
        void send(sBox box, Notification type);
        void send(sLink link, Notification type);
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
        /** The function allocates a page and initialize the defaults boxes.
         @param instance The instance that will manage the page.
         @param dico The dico that will initialize the page.
         @return The page.
         */
        static sPage create(sInstance instance, sDico dico = nullptr);
		
		//! Retrieve the sPage.
		/** The function sPage.
		 @return The sPage.
		 */
		inline sPage getShared() noexcept
		{
			return static_pointer_cast<Page>(shared_from_this());
		}
		
		//! Retrieve the scPage.
		/** The function scPage.
		 @return The scPage.
		 */
		inline scPage getShared() const noexcept
		{
			return static_pointer_cast<const Page>(shared_from_this());
		}
		
        //! Retrieve the instance that manages the page.
        /** The function retrieves the instance that manages the page.
         @return The instance that manages the page.
         */
        inline sInstance getInstance() const noexcept
        {
            return m_instance.lock();
        }

        //! Get the boxes.
        /** The function retrieves the boxes from the page.
         @param boxes   A vector of elements.
         */
        void getBoxes(vector<sBox>& boxes) const
        {
            lock_guard<mutex> guard(m_mutex);
            boxes = m_boxes;
        }
        
        //! Get the number of boxes.
        /** The function retrieves the number of boxes.
         @return The number of boxes.
         */
        ulong getNumberOfBoxes() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return m_boxes.size();
        }
        
        //! Get the number of boxes.
        /** The function retrieves a boxes from the page.
         @param _id   The id of the boxe.
         @return The boxe that match with the id.
         */
        sBox getBoxe(const ulong _id) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(_id < m_boxes.size())
            {
                for(vector<sBox>::size_type i = 0; i < m_boxes.size(); i++)
                {
                    if(m_boxes[i]->getId() == _id)
                    {
                        return m_boxes[i];
                    }
                }
            }
            return nullptr;
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
        /** The function reads a dico and add the boxes and links to the page.
         @param dico The dico.
         */
        void add(sDico dico);
        
        //! Free a box.
        /** The function removes a box from the page.
         @param box        The pointer to the box.
         */
        void remove(sBox box);
        
        //! Free a link.
        /** The function removes a link from the page.
         @param link        The pointer to the link.
         */
        void remove(sLink link);
        
        //! Replace a box with another one.
        /** The function instantiates a box with a dico that will replace an old box.
         @param box        The box to replace.
         @param dico       The dico that defines a box.
         @return A pointer to the box.
         */
        sBox replace(sBox box, sDico dico);
        
        //! Bring a box to the front of the page.
        /** The function brings a box to the front of the page. The box will be setted as if it was the last box created and will be the last box of the vector of boxes.
         @param box        The pointer to the box.
         */
        void toFront(sBox box);
        
        //! Bring a box to the back of the page.
        /** The function brings a box to the back of the page. The box will be setted as if it was the first box created and will be the first box of the vector of boxes.
         @param box        The pointer to the box.
         */
        void toBack(sBox box);
        
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
        
        //! Receives notification when an attribute value has changed.
        /** The function receives notification when an attribute value has changed.
         @param attr The attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        bool attributeChanged(sAttr attr) override;
		
		//! Add a listener to the box.
		/** The function adds a listener to the box.
		 @param list    The listener.
		 */
		void addListener(sListener list);
        
        //! Remove a listener from the box.
        /** The function removes a listener from the box.
         @param list    The listener.
         */
        void removeListener(sListener list);
    };
    
    
    // ================================================================================ //
    //                                  PAGE LISTENER                                   //
    // ================================================================================ //
    
    //! The page listener is an abstract class that facilitates the control of a page in an application.
    /**
     The page listener should be a shared pointer to be able to bind itself to a page. Thus, like in all the kiwi classes, you should use another creation method and call the bind function in it. The page listener owns a vector of box listeners and facilitates managements of boxes like the creation, the deletion, the selection, etc.
     @see Page, Page::Listener, Box::Listener
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
        
        //! Receive the notification that a box has been created.
        /** The function is called by the page when a box has been created.
         @param box     The box.
         */
        virtual void boxCreated(sPage page, sBox box) = 0;
        
        //! Receive the notification that a box has been removed.
        /** The function is called by the page when a box has been removed.
         @param box     The box.
         */
        virtual void boxRemoved(sPage page, sBox box) = 0;
        
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
        
        //! Receives notification when an value has changed.
        /** The function receives notification when an attribute has changed.
         @param attr The attribute.
         */
        virtual void attributeChanged(sPage page, sAttr attr) = 0;
    };
}


#endif


