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

#ifndef __DEF_KIWI_BEACON__
#define __DEF_KIWI_BEACON__

#include "KiwiTools.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      BEACON                                      //
    // ================================================================================ //
    
    //! The beacon is unique and matchs to a "unique" string in the scope of a beacon factory and can be used to bind beacon's castaways.
    /**
     The beacon are uniques in the scope of a beacon factory and matchs to a string. If you create a beacon with a string that already matchs to a beacon of the beacon factory, it will return this beacon otherwise it will create a new beacon. Thus, the beacons can be used to bind, unbind and retrieve castways. After recovering a castaway, you should cast dynamicaly it the class you expect. More often, this will be an kiwi object.
     @see Beacon::Factory
     @see Beacon::Castaway
     */
    class Beacon
    {
    public:
        class Factory;
        class Castaway;
        typedef shared_ptr<Castaway>sCastaway;
        typedef weak_ptr<Castaway>  wCastaway;
    private:
        const string        m_name;
        vector<wCastaway>   m_castaways;
        mutable mutex       m_mutex;
    public:
        
        //! The constructor.
        /** You should never use this method except.
         */
        inline Beacon(string const& name) noexcept : m_name(name) {}
        
        //! The constructor.
        /** You should never use this method except.
         */
        inline Beacon(string&& name) noexcept : m_name(name){}
        
        //! The destructor.
        /** You should never use this method except.
         */
        inline ~Beacon() noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            m_castaways.clear();
        }
        
        //! Retrieve the name of the beacon.
        /** The function retrieves the unique name of the beacon.
         @return The name of the beacon in the string format.
         */
        inline string name() const noexcept { return m_name; }
        
        //! Retrieve the number of castaways in the binding list of the beacon.
        /** The function retrieves the number of castaways in the binding list of the beacon.
         @return The number of castaways binded to the beacon.
         */
        inline ulong size() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_castaways.size();
        }
        
        //! Retrieve an castaway from the binding list of the beacon.
        /** The function retrieves an castaway from the binding list of the beacon at a defined position.
         @param index   The position of the castaway in the binding list.
         @return        The pointer of the binded castaways or null is the index is less than 0 or greater or equal to the number of castaways in the binding list.
         */
        inline sCastaway get(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < m_castaways.size())
            {
                return m_castaways[index].lock();
            }
            else
            {
                return sCastaway();
            }
        }
        
        //! Retrieve all the castaways from the binding list of the beacon.
        /** The function retrieves all the castaways from the binding list of the beacon at a defined position.
         @return A vector of castaways.
         */
        inline vector<wCastaway> get() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return m_castaways;
        }
        
        //! Add an castaways in the binding list of the beacon.
        /** The function adds an castaway in the binding list of the beacon. If the castaway is already in the binding list, the function doesn't do anything.
         @param castaway  The pointer of the castaway.
         @see        unbind()
         */
        void bind(const sCastaway castaway);
        
        //! Remove an castaways from the binding list of the beacon.
        /** The function removes an castaway from the binding list of the beacon. If the castaway isn't in the binding list, the function doesn't do anything.
         @param castaway  The pointer of the castaway.
         @see        bind()
         */
        void unbind(const sCastaway castaway);
        
        // ================================================================================ //
        //                                  BEACON CASTAWAY                                 //
        // ================================================================================ //
        
        //! The beacon castaway can be attached to a beacon.
        /**
         The beacon castaway an empty class but the only one that can be attached to a beacon. If you want your class to be retrievable from a beacon, you should inherit from the castaway. Important, your class should be allocated with a shared pointer.
         @see Beacon
         @see Beacon::Factory
         */
        class Castaway
        {
            ;
        };
        
        // ================================================================================ //
        //                                  BEACON FACTORY                                  //
        // ================================================================================ //
        
        //! The beacon factory creates beacons.
        /**
         The beacon factory is the only class that should create and delete beacons. Important, a beacon factory is always created with a patcher instance to manage global instance's beacon ans most often this beacon factory will be sufficient for your needs. If you want another beacon factory, don't forget that the beacons created with it won't be retrievable by the natives classes or other extern classes. With other words, if you just want to bind your castaway to a global beacon use the method of your kiwi object.
         @see Beacon
         @see Beacon::Castaway
         */
        class Factory
        {
        private:
            friend Beacon;
            map<string,
            sBeacon>    m_beacons;
            mutex       m_factory_mutex;
            
        protected:
            
            //! Beacon creator.
            /** This function checks if a beacon with this name has already been created and returns it, otherwise it creates a new beacon with this name.
             @param     The name of the beacon to retrieve.
             @return    The beacon that match with the name.
             */
            sBeacon createBeacon(string const& name) noexcept;
            
            //! Beacon creator.
            /** This function checks if a beacon with this name has already been created and returns it, otherwise it creates a new beacon with this name.
             @param     The name of the beacon to retrieve.
             @return    The beacon that match with the name.
             */
            sBeacon createBeacon(string&& name) noexcept;
            
        public:
            
            //! The constructor.
            /** You should never use this method except if you really know what you do.
             */
            inline Factory() noexcept{ }
            
            //! The destructor.
            /** You should never use this method except if you really know what you do.
             */
            inline ~Factory() noexcept
            {
                lock_guard<mutex> guard(m_factory_mutex);
                m_beacons.clear();
            }
        };
    };
};


#endif


