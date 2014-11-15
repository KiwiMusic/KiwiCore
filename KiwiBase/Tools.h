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

namespace Kiwi
{
	template<typename T>
	struct enable_shared_from_this_virtual;
	
	class enable_shared_from_this_virtual_base : public std::enable_shared_from_this<enable_shared_from_this_virtual_base>
	{
		typedef std::enable_shared_from_this<enable_shared_from_this_virtual_base> base_type;
		template<typename T>
		friend struct enable_shared_from_this_virtual;
		
		std::shared_ptr<enable_shared_from_this_virtual_base> shared_from_this()
		{
			return base_type::shared_from_this();
		}
		std::shared_ptr<enable_shared_from_this_virtual_base const> shared_from_this() const
		{
			return base_type::shared_from_this();
		}
	};
	
	template<typename T>
	struct enable_shared_from_this_virtual: virtual enable_shared_from_this_virtual_base
	{
		typedef enable_shared_from_this_virtual_base base_type;
		
	public:
		std::shared_ptr<T> shared_from_this()
		{
			std::shared_ptr<T> result(base_type::shared_from_this(), static_cast<T*>(this));
			return result;
		}
		
		std::shared_ptr<T const> shared_from_this() const
		{
			std::shared_ptr<T const> result(base_type::shared_from_this(), static_cast<T const*>(this));
			return result;
		}
	};
	
	
    template <typename Type> Type clip(const Type& n, const Type& lower, const Type& upper)
    {
        return max(lower, min(n, upper));
    }
    
    inline long pow2roundup(long x)
    {
        if(x < 1)
            return 1;

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x+1;
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
    
    inline string toString(double __val)
    {
        return to_string(__val);
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
    
    inline void signalAddSignal(const sample* in1, const sample* in2, sample* out1, long vectorsize)
    {
#ifdef __APPLE__
        
#ifdef KIWI_DOUBLE
        vDSP_vaddD(in1, 1, in2, 1, out1, 1, vectorsize);
#else
        vDSP_vadd(in1, 1, in2, 1, out1, 1, vectorsize);
#endif
        
#elif __CBLAS__
        
#ifdef KIWI_DOUBLE
        cblas_dcopy(vectorsize, in1, 1, out1, 1);
        cblas_daxpy(vectorsize, 1., in2, 1, out1, 1);
#else
        cblas_scopy(vectorsize, in1, 1, out1, 1);
        cblas_saxpy(vectorsize, 1., in2, 1, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) = *(in1++) + *(in2++);
#endif
    }
    
    inline void signalAddSignal(const sample* in1, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        cblas_daxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
        cblas_saxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) += *(in1++);
#endif
    }
    
    inline void signalAddScalar(const sample* in1, const sample in2, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        vDSP_vsaddD(in1, 1, &(in2), out1, 1, vectorsize);
#else
        vDSP_vsadd(in1, 1, &(in2), out1, 1, vectorsize);
#endif
        
#elif __CBLAS__
        
#ifdef KIWI_DOUBLE
        cblas_dcopy(vectorsize, in1, 1, out1, 1);
        cblas_dscal(vectorsize, in2, out1, 1);
#else
        cblas_scopy(vectorsize, in1, 1, out1, 1);
        cblas_sscal(vectorsize, in2, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) = *(in1++) + in2;
#endif
    }
    
    inline void signalAddScalar(const sample in1, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        cblas_dscal((const int)vectorsize, in1, out1, 1);
#else
        cblas_sscal((const int)vectorsize, in1, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) += in2;
#endif
    }
    
    inline void signalMulScalar(const sample* in1, const sample in2, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        vDSP_vsmulD(in1, 1, &(in2), out1, 1, vectorsize);
#else
        vDSP_vsmul(in1, 1, &(in2), out1, 1, vectorsize);
#endif
        
#elif __CBLAS__
        
#ifdef KIWI_DOUBLE
        cblas_dcopy(vectorsize, in1, 1, out1, 1);
        cblas_dscal(vectorsize, in2, out1, 1);
#else
        cblas_scopy(vectorsize, in1, 1, out1, 1);
        cblas_sscal(vectorsize, in2, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) = *(in1++) * in2;
#endif
    }
    
    inline void signalMulScalar(const sample in1, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        cblas_dscal((const int)vectorsize, in1, out1, 1);
#else
        cblas_sscal((const int)vectorsize, in1, out1, 1);
#endif
        
#else
        while(--vectorsize)
            *(out1++) *= in1;
#endif
    }
    
    inline void signalCopy(const sample* in1, sample* out1, long vectorsize)
    {
#if defined (__APPLE__) || defined(__CBLAS__)
        
#ifdef KIWI_DOUBLE
        cblas_dcopy((const int)vectorsize, in1, 1, out1, 1);
#else
        cblas_scopy((const int)vectorsize, in1, 1, out1, 1);
#endif
        
#else
        memcpy(out1, in1, vectorsize * sizeof(sample));
#endif
    }
    
    inline void signalClear(sample* out1, long vectorsize)
    {
#ifdef __APPLE__
        
#ifdef KIWI_DOUBLE
        vDSP_vclrD(out1, 1, vectorsize);
#else
        vDSP_vclr(out1, 1, vectorsize);
#endif
        
#else
         memset(out1, 0, vectorsize * sizeof(sample));
#endif
    }
    
    inline void signalFill(const sample in1, sample* out1, long vectorsize)
    {
#ifdef __APPLE__
        
#ifdef KIWI_DOUBLE
        vDSP_vfillD(&(in1), out1, 1, vectorsize);
#else
        vDSP_vfill(&(in1), out1, 1, vectorsize);
#endif
        
#elif __CATLAS__
        
#ifdef KIWI_DOUBLE
        catlas_dset(vectorsize, in1, out1, 1);
#else
        catlas_dset(vectorsize, in1, out1, 1);
#endif

#else
        while(--vectorsize)
            *(out1++) = in1;
#endif
       
        
    }
};

#endif


