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

#include "Defs.h"
#include "Console.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      BEACON                                      //
    // ================================================================================ //
    
    //! The beacon is an unique object that matchs to a "unique" string in the scope of a beacon factory and can be used to bind boxes.
    /**
     The beacon are uniques in the scope of a beacon factory and matchs to a string. If you create a beacon with a string that already matchs to a beacon of the beacon factory, it will return this beacon otherwise it will create a new beacon. Thus, the beacons are very similar to the beacon except that it works in a limited scope and that the beacons can be used to bind (and unbind) boxes and retrieve them.
     @see Factory
     */
    class Beacon
    {
    private:
        const string        m_name;
        vector<wObject>     m_objects;
        mutable mutex       m_mutex;
    public:
        
        //! The constructor.
        /** You should never use this method except.
         */
        Beacon(string const& name);
        
        //! The destructor.
        /** You should never use this method except.
         */
        ~Beacon();
        
        //! Retrieve the name of the beacon.
        /** The function retrieves the unique name of the beacon.
         @return The name of the beacon in the string format.
         */
        inline string getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the number of boxes in the binding list of the beacon.
        /** The function retrieves the number of boxes in the binding list of the beacon.
         @return The number of boxes binded to the beacon.
         */
        inline ulong size() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_objects.size();
        }
        
        //! Retrieve an object from the binding list of the beacon.
        /** The function retrieves an object from the binding list of the beacon at a defined position.
         @param index   The position of the object in the binding list from 0 to the number of boxes in the binding list -1.
         @return        The pointer of the binded boxes or NULL is the index is less than 0 or greater or equal to the number of boxes in the binding list.
         */
        inline sObject getBox(const ulong index) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            if(index < m_objects.size())
            {
                return m_objects[index].lock();
            }
            else
            {
                return nullptr;
            }
        }
        
        //! Add an boxes in the binding list of the beacon.
        /** The function adds an object in the binding list of the beacon. If the object is already in the binding list, the function doesn't do anything.
         @param object  The pointer of the object.
         @see        unbind()
         */
        void bind(sObject object);
        
        //! Remove an boxes from the binding list of the beacon.
        /** The function removes an object from the binding list of the beacon. If the object isn't in the binding list, the function doesn't do anything.
         @param object  The pointer of the object.
         @see        bind()
         */
        void unbind(sObject object);
        
        // ================================================================================ //
        //                                  BEACON FACTORY                                  //
        // ================================================================================ //
        
        //! The beacon factory creates beacons. More often, you shouldn't need to create one and use the instance that inherite from it.
        /**
         The beacon factory is the only class that should create and delete beacons. Important, a beacon factory is always created with a kiwi instance to manage global instance's beacon ans most often this beacon factory will be sufficient for your needs. If you want another beacon factory, don't forget that the beacons created with it won't be retrievable by the natives classes or other extern classes. With other words, if you just want to bind your object to a global beacon use the method of your object.
         @see Beacon
         @see Instance
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
            sBeacon createBeacon(string const& name);
            
        public:
            
            //! The constructor.
            /** You should never use this method except if you really know what you do.
             */
            Factory();
            
            //! The destructor.
            /** You should never use this method except if you really know what you do.
             */
            ~Factory();
        };
        
        //! The beacon creation method.
        /** The function allocates beacon for a object.
         @param object     The object that want to create the beacon.
         @param name    The name of the beacon.
         @return The beacon.
         */
        static sBeacon create(sObject object, string const& name);
    };
    
    inline string toString(sBeacon __val)
    {
        return __val->getName();
    }
};


#endif


