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

#ifndef __DEF_KIWI_TOOLS__
#define __DEF_KIWI_TOOLS__

#include "Defs.h"
#include "ToolsMath.h"
#include "ToolsSignal.h"

namespace Kiwi
{
	template <class InputContainer, class T> typename InputContainer::size_type find_position(InputContainer const& container, const T& val)
    {
        typename InputContainer::size_type position = 0;
        for(; position < container.size(); position++)
        {
            if(container[position] == val)
            {
                return position;
            }
        }
        return position;
    }
    
    template <class _InputIterator, class _Tp> inline _InputIterator find_weak(_InputIterator __first, _InputIterator __last, const _Tp& __value_)
    {
        for (; __first != __last; ++__first)
            if ((*__first).lock() == __value_)
                break;
        return __first;
    }
	
    inline string toString(int __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(float __val)
    {
        return to_string(__val);
    }
    
    inline string toString(double __val, bool removeTrailingPoint = false)
    {
		size_t len = snprintf(0, 0, "%.10f", __val);
		string s(len+1, 0);
		// technically non-portable, see below
		snprintf(&s[0], len+1, "%.10f", __val);
		// remove nul terminator
		s.pop_back();
		// remove trailing zeros
		s.erase(s.find_last_not_of('0') + 1, string::npos);
		
		// remove trailing point
		if(removeTrailingPoint && s.back() == '.')
			s.pop_back();
		
		return s;
    }
    
    inline string toString(long double __val)
    {
        return to_string(__val);
    }
    
    template< typename T > string toHex( T i )
    {
        std::stringstream stream;
        stream << "0x" << setfill('0') << setw(sizeof(T)*2) << hex << i;
        return stream.str();
    }
    
    template< typename T > string toId( T i )
    {
        std::stringstream stream;
        stream << "u" << dec << i;
        return stream.str();
    }
    
    template<typename T>
    struct weak_ptr_hash : public std::unary_function<std::weak_ptr<T>, size_t> {
        size_t operator()(const std::weak_ptr<T>& wp)
        {
            auto sp = wp.lock();
            return std::hash<decltype(sp)>()(sp);
        }
    };
    
    template<typename T>
    struct weak_ptr_equal : public std::unary_function<std::weak_ptr<T>, bool> {
        
        bool operator()(const std::weak_ptr<T>& left, const std::weak_ptr<T>& right)
        {
            return !left.owner_before(right) && !right.owner_before(left);
        }
    };
};

#endif


