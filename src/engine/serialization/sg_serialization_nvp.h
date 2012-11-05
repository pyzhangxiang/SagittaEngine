#ifndef __SG_SERIALIZATION_NVP_H__
#define __SG_SERIALIZATION_NVP_H__

#ifdef DEBUG
#include <iostream>
#endif
#include <cassert>

namespace Sagitta{

    namespace serialization
    {
        // nvp from boost::serialization:nvp
        template<class T>
        struct nvp : 
            public std::pair<const char *, T *>
        {
            std::string _filename;
            bool _is_array_element;

            explicit nvp(const char * name_, T & t) :
            // note: redundant cast works around borland issue
            // note: added _ to suppress useless gcc warning
            std::pair<const char *, T *>(name_, (T*)(& t)), _is_array_element(false)
            {}

            explicit nvp(const std::string &filename, const char * name_, T & t) :
            // note: redundant cast works around borland issue
            // note: added _ to suppress useless gcc warning
            std::pair<const char *, T *>(name_, (T*)(& t)), _filename(filename), _is_array_element(false)
            {}
            
            explicit nvp(const char * name_, T & t, bool is_array_element) :
            // note: redundant cast works around borland issue
            // note: added _ to suppress useless gcc warning
            std::pair<const char *, T *>(name_, (T*)(& t)), _is_array_element(is_array_element)
            {}

            nvp(const nvp & rhs) : 
            // note: redundant cast works around borland issue
            std::pair<const char *, T *>(rhs.first, (T*)rhs.second)
            {}
            
            std::string filename() const{ return _filename; }
            
            bool is_array_element() const{ return _is_array_element; }

            const char * name() const {
                return this->first;
            }
            T & value() const {
                return *(this->second);
            }

            const T & const_value() const {
                return *(this->second);
            }
        };

        template<class T>
        inline nvp<T> make_nvp(const char * name, T & t)
        {
            return nvp<T>(name, t);
        }

        template<class T>
        inline nvp<T> make_nvp(const std::string &filename, const char * name, T & t)
        {
            return nvp<T>(filename, name, t);
        }
        
        template<class T>
        inline nvp<T> make_nvp(const char * name, T & t, bool is_array_element)
        {
            return nvp<T>(name, t, is_array_element);
        }

    }

#define SG_MAKE_NVP(name, v) Sagitta::serialization::make_nvp((name), (v));
#define SG_MAKE_NVP_REF(filename, name, v) Sagitta::serialization::make_nvp((filename), (name), (v));

} // namespace Sagitta

#endif // __SG_SERIALIZATION_NVP_H__
