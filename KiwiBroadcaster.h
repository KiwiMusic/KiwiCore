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

#ifndef __DEF_KIWI_BROADCASTER__
#define __DEF_KIWI_BROADCASTER__

#include "KiwiAtom.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  BROADCASTER                                     //
    // ================================================================================ //
    
    //! The broadcaster is a class that manages a list of listeners.
    /** The broadcaster manages a list of listeners and allows to retrieves them easily and threadsafely.
     */
    template<class T> class Broadcaster
    {
    public:
        typedef shared_ptr<T>        sListener;
        typedef weak_ptr<T>          wListener;
        typedef shared_ptr<const T>  scListener;
        typedef weak_ptr<const T>    wcListener;
        
    private:
        set<wListener,
        owner_less<wListener>>  m_listeners;
        mutex                   m_mutex;
    public:
        
        //! Destructor.
        /** The function frees the list of listeners.
         */
        ~Broadcaster()
        {
            lock_guard<mutex> guard(m_mutex);
            m_listeners.clear();
        }
        
        //! Add an instance listener in the binding list of the manager.
        /** The function adds an instance listener in the binding list of the manager.
         @param listener  The listener.
         */
        void addListener(sListener listener)
        {
            if(listener)
            {
                lock_guard<mutex> guard(m_mutex);
                m_listeners.insert(listener);
            }
        }
        
        //! Remove an instance listener from the binding list of the manager.
        /** The function removes an instance listener from the binding list of the manager.
         @param listener  The listener.
         */
        void removeListener(sListener listener)
        {
            if(listener)
            {
                lock_guard<mutex> guard(m_mutex);
                m_listeners.erase(listener);
            }
        }
        
    protected:
        
        //! Gets the listeners.
        /** The functions gets the liteners from the attribute.
         @return The listeners.
         */
        vector<sListener> getListeners() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            vector<sListener> listeners;
            for(auto it : m_listeners)
            {
                sListener listener = (it).lock();
                if(listener)
                {
                    listeners.push_back(listener);
                }
            }
            return listeners;
        }
        
        //! Gets the listeners.
        /** The functions gets the liteners from the attribute and removes the deprecated listeners.
         @return The listeners.
         */
        vector<sListener> getListeners() noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            vector<sListener> listeners;
            for(auto it = m_listeners.begin(); it != m_listeners.end();)
            {
                sListener listener = (*it).lock();
                if(listener)
                {
                    listeners.push_back(listener); ++it;
                }
                else
                {
                    it = m_listeners.erase(it);
                }
            }
            return listeners;
        }
    };
};


#endif


