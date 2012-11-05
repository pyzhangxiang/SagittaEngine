///////////////////////////////////////////////////////////////////////////////
///  @file lvd_types.h
///  Header for the types used.
///
///  @author Liam Devine
///  @email
///  See http://www.liamdevine.co.uk for contact details.
///  @licence
///  See licence.txt for more details. \n 
///////////////////////////////////////////////////////////////////////////////

#ifndef BASIC_TYPES_H_
#	define BASIC_TYPES_H_

#include <loki/Typelist.h>
#include <string>

namespace Sagitta
{
    namespace tmp
    {
        template< bool Test>
    	struct If_
    	{
    		enum {RESULT = 1};
    	};

    	template< >
    	struct If_<false>
    	{
    		enum {RESULT = 0};
    	};

    	template<int T,int T1>
    	struct If_Or
    	{
    	 	   enum {value =1};
     	};
    	template<>
    	struct If_Or<0,0>
    	{
    	 	   enum {value =0};
     	};

    	template< bool B, typename Then = void, typename Else = void >
    	struct If_Else
    	{
    		typedef Then type;
    	};

    	template< typename Then, typename Else >
    	struct If_Else< false, Then, Else >
    	{
    		typedef Else type;
    	};
    	
    	/////////////////////////////
    	template<typename T>
     	struct is_buildin_type
     	{
            typedef Loki::TL::MakeTypelist<
            		bool,
    				char,unsigned char, signed char,
    				short,unsigned short, signed short,
    				int,unsigned int, signed int,
    				long, unsigned long, signed long,
    				float,
    				double, long double>::Result Buildin;
    		enum {value = Loki::TL::IndexOf<Buildin,T>::value == -1 ? false : true};
    	};


        //////////////////////////
        template<typename T>
        struct is_array
        {
            static const bool value = false;
        };

        template<typename T>
        struct is_array< T[] >
        {
            static const bool value = true;
        };

        template<typename T, std::size_t N>
        struct is_array< T[N] >
        {
            static const bool value = true;
        };
        
        ////////////////////////////
        
        template<typename T>
        struct is_pointer
        {
            static const bool value = false;
        };
        
        template<typename T>
        struct is_pointer<T*>
        {
            static const bool value = true;
        };
        
        template<typename T>
        struct is_pointer<const T*>
        {
            static const bool value = true;
        };
        
        //////////////////////////////////

        template<bool> struct static_assert_failure;
        template<> struct static_assert_failure<true>{ enum{ value = 1 }; };
        template<int x> struct static_assert_test{};

#define SG_STATIC_ASSERT(B) \
    typedef static_assert_test<sizeof(static_assert_failure<(bool)(B)>)> static_assert_##__LINE__;
    }

}//end of TYPE


#endif//BASIC_TYPES_H_
