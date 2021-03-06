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

#ifndef __DEF_KIWI_LISTENERSET__
#define __DEF_KIWI_LISTENERSET__

#include "KiwiAtom.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  LISTENER SET                                    //
    // ================================================================================ //
    
    //! The listener set is a class that manages a list of listeners.
    /** The listener set manages a list of listeners and allows to retrieves them easily and threadsafely.
     */
    template <class ListenerClass> class ListenerSet
    {
    private:
        typedef shared_ptr<ListenerClass>		sListener;
        typedef weak_ptr<ListenerClass>			wListener;
        
        set<wListener,
        owner_less<wListener>>  m_listeners;
        mutable mutex           m_listeners_mutex;
        
    public:
        
        //! Creates an empty listener set.
        inline ListenerSet() noexcept {}
        
        //! Destructor.
        inline ~ListenerSet() noexcept {m_listeners.clear();}
        
        //! Add a listener to the set.
        /** The function adds a new listener to the set.
         If the listener was allready present in the set, the function does nothing.
         @param listener The new listener to be added.
         @return True if success, false otherwise.
         */
        bool add(sListener listener) noexcept
        {
            if(listener)
            {
                lock_guard<mutex> guard(m_listeners_mutex);
                if(m_listeners.insert(listener).second)
                {
                    return true;
                }
            }
            return false;
        }
        
        //! Remove a listener from the set.
        /** The function removes a listener from the set.
         If the listener wasn't in the set, the function does nothing.
         @param listener The listener to be removed.
         @return True if success, false otherwise.
         */
        bool remove(sListener listener) noexcept
        {
            if(listener)
            {
                lock_guard<mutex> guard(m_listeners_mutex);
                if(m_listeners.erase(listener))
                {
                    return true;
                }
            }
            return false;
        }
        
        //! Remove all invalid listeners from the set.
        /** The function removes all invalid listeners from the set.
         */
        void clean() noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            for(auto it = m_listeners.begin(); it != m_listeners.end();)
            {
                if((*it).lock()) ++it;
                else it = m_listeners.erase(it);
            }
        }
        
        //! Returns the number of listeners (including invalid ones).
        /** The function return the number of listeners (including invalid ones).
         @return The number of listeners.
         */
        ulong size() const noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            return m_listeners.size();
        }
        
        //! Returns true if any listeners are registered (including invalid ones).
        /** The function returns true if any listeners are registered (including invalid ones).
         @return True if any listeners are registered.
         */
        bool empty() const noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            return m_listeners.empty();
        }
        
        //! Remove all listener from the set.
        /** The function removes all listener from the set.
         */
        void clear() noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            m_listeners.clear();
        }
        
        //! Returns true if the set contains a given listener.
        /** The function returns true if the set contains a given listener.
         @param listener The listener.
         @return True if the set contains a given listener, false otherwise.
         */
        bool contains(wListener listener) const noexcept
        {
            return m_listeners.find(listener) != m_listeners.end();
        }
        
        //! Retrieve the listeners.
        /** The functions retrieves the listeners, removing all invalid listeners.
         @return The listeners.
         */
        inline vector<sListener> getListeners() noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            vector<sListener> listeners;
            for(auto it = m_listeners.begin(); it != m_listeners.end();)
            {
                sListener l = (*it).lock();
                if(l)
                {
                    listeners.push_back(l);
                    ++it;
                }
                else
                {
                    it = m_listeners.erase(it);
                }
            }
            return listeners;
        }
        
        //! Retrieve the listeners.
        /** The functions retrieves the listeners
         @return The listeners.
         */
        inline vector<sListener> getListeners() const noexcept
        {
            lock_guard<mutex> guard(m_listeners_mutex);
            vector<sListener> listeners;
            for(auto elem : m_listeners)
            {
                sListener l = elem.lock();
                if(l)
                {
                    listeners.push_back(l);
                }
            }
            return listeners;
        }
        
        //! Calls a given method for each listener of the set.
        /** The function calls a given method for each listener of the set.
         @param fun The listener's method to call.
         @param arguments optional arguments.
         */
        template<class T, class ...Args> void call(T fun, Args&& ...arguments)
        {
            for(auto listener : getListeners())
            {
                (listener.get()->*(fun))(arguments...);
            }
        }
        
        //! Calls a given method for each listener of the set, removing all invalid listeners.
        /** The function calls a given method for each listener of the set, removing all invalid listeners.
         @param fun The listener's method to call.
         @param arguments optional arguments.
         */
        template<class T, class ...Args> void call(T fun, Args&& ...arguments) const
        {
            for(auto listener : getListeners())
            {
                (listener.get()->*(fun))(arguments...);
            }
        }
    };
}

#endif
