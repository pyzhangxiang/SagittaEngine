#ifndef __SGSTLALLOCATOR_H__
#define __SGSTLALLOCATOR_H__

#include "sgMalloc.h"
#include <string>

namespace Sagitta
{
    template<typename T>
    class  sgStlAllocator
    {
    public:
        typedef T value_type;

        typedef value_type* pointer;
        typedef value_type& reference;
        typedef const value_type* const_pointer;
        typedef const value_type& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        template<typename U>
        struct rebind
        {	
            typedef sgStlAllocator<U> other;
        };

        pointer address(reference val) const
        {	
            return &val;
        }

        const_pointer address(const_reference val) const
        {	
            return &val;
        }

        sgStlAllocator()
        {	
        }

        sgStlAllocator(const sgStlAllocator<T>&)
        {	
        }

        template<typename U>
        sgStlAllocator(const sgStlAllocator<U>&)
        {	
        }

        template<typename U>
        sgStlAllocator<T> &operator=(const sgStlAllocator<U>&)
        {	
            return (*this);
        }

        void deallocate(pointer ptr, size_type)
        {	
            sgFree(ptr);
        }

        pointer allocate(size_type cnt)
        {	
            return reinterpret_cast<pointer>(sgMalloc(cnt * sizeof(T)));
        }

        pointer allocate(size_type cnt, const void*)
        {	
            return reinterpret_cast<pointer>(sgMalloc(cnt * sizeof(T)));
        }

        inline void construct(pointer p, const T& t) { new(p) T(t); }
        inline void destroy(pointer p) { p->~T(); }

        size_t max_size() const
        {	
            size_t cnt= (size_t)(-1) / sizeof (T);
            return (0 < cnt ? cnt : 1);
        }
    };

    template <typename T, typename U>
    inline bool operator==(const sgStlAllocator<T>&, const sgStlAllocator<U>&)
    {	
        return true;
    }

    template <typename T, typename U>
    inline bool operator!=(const sgStlAllocator<T>&, const sgStlAllocator<U>&)
    {	
        return false;
    }

#ifndef sg_list
#define sg_list(value_type) \
    std::list<value_type, Sagitta::sgStlAllocator<value_type> >
#endif

#ifndef sg_vector
#define sg_vector(value_type) \
    std::vector<value_type, Sagitta::sgStlAllocator<value_type> >
#endif

#ifndef sg_deque
#define sg_deque(value_type) \
    std::deque<value_type, Sagitta::sgStlAllocator<value_type> >
#endif

#ifndef sg_queue
#define sg_queue(value_type) \
    std::queue<value_type, std::deque<value_type, Sagitta::sgStlAllocator<value_type> > >
#endif

#ifndef	sg_priority_queue
#define sg_priority_queue(value_type, compare_func) \
    std::priority_queue<value_type, std::vector< value_type, Sagitta::sgStlAllocator<value_type> >, compare_func>
#endif

#ifndef sg_map
#define sg_map(key_type, value_type) \
    std::map<key_type, value_type, std::less<key_type>, Sagitta::sgStlAllocator<std::pair<const key_type, value_type> > >
#endif

#ifndef sg_set
#define sg_set(key_type) \
    std::set<key_type, std::less<key_type>, Sagitta::sgStlAllocator<key_type> >
#endif

#ifndef sg_multimap
#define sg_multimap(key_type, value_type) \
    std::multimap<key_type, value_type, std::less<key_type>, Sagitta::sgStlAllocator<std::pair<const key_type, value_type> > >
#endif

#ifndef sg_hashmap
#define sg_hashmap(key_type, value_type) \
    stdext::hash_map<key_type, value_type, stdext::hash_compare<key_type, std::less<key_type> >, Sagitta::sgStlAllocator<std::pair<const key_type, value_type> > >
#endif

#ifndef sg_hashmultimap
#define sg_hashmultimap(key_type, value_type) \
    stdext::hash_multimap<key_type, value_type, stdext::hash_compare<key_type, std::less<key_type> >, Sagitta::sgStlAllocator<std::pair<const key_type, value_type> > >
#endif

    typedef std::basic_string<char, std::char_traits<char>, Sagitta::sgStlAllocator<char> > sg_string;

}


#endif  // __SGSTLALLOCATOR_H__
