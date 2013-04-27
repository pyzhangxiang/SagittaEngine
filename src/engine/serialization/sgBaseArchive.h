#ifndef __SGBASEARCHIVE_H__
#define __SGBASEARCHIVE_H__

#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"
#include "engine/common/tmp_define.h"
#include "sg_serialization_nvp.h"
//#include "strtk/strtk.hpp"
#include "../common/sgStringUtil.h"
#include <string>
#include <vector>

namespace Sagitta
{   
    namespace serialization
    {
	    class _SG_KernelExport sgBaseArchive
	    {
        protected:
            size_t mVersion;
            const std::string FILE_EXT;
            
            static const char *MS_PTR_REF;
            static const char *MS_FILE_REF;
            static const char *MS_ROOT_NAME;

        public:
            sgBaseArchive(const std::string &file_ext, size_t version);
            
            virtual ~sgBaseArchive(void){}
            
        private:
            
        public:
            /// @ret false => this document has been serialized, just ref
            virtual bool start_doc(const char *filename) = 0;
            /// @ret true => same filename as the top doc in the stack            
            virtual bool end_doc(const char *filename) = 0;
            
            virtual void start_node(const char *name) = 0;
            /// @ret true => same node name as the top node in the stack
            virtual bool end_node(const char *name) = 0;
            
            template<typename T>
            void value_node(const char *name, T &value){}
            
            /// @ret false => existed, just ref, don't serialize the content again
            template<typename T>
            bool value_pointer(T * &ptr){ return false; }
            
            template<typename T>
            void value_attribute(const char *name, T &value){}
           
        /*  member template function cannot be specialized
        
            template<>
            void addNode(const char *name, const char *v)
            {
                rapidxml::xml_node<> *node = mNodeStack.back();
                rapidxml::xml_node<> *new_node = mDoc.allocate_node(rapidxml::node_element, name, v);
                node->append_node(new_node);
            }
            
            template<>
            void addNode(const char *name, const std::string &v)
            {
                rapidxml::xml_node<> *node = mNodeStack.back();
                rapidxml::xml_node<> *new_node = mDoc.allocate_node(rapidxml::node_element, name, v.c_str);
                node->append_node(new_node);
            }
          */  
        };

#ifndef SG_SERIALIZATION_ARCHIVE
#define SG_SERIALIZATION_ARCHIVE(archive_class) \
    template<typename T> \
    friend archive_class & operator&(archive_class &archive, const Sagitta::serialization::nvp<T> &v); \
    \
    template<typename T> \
    friend void serialize(archive_class &archive, const serialization::nvp<T> &v); \
    /*\
    template<typename T, bool ispointer> friend struct SerializePointer<archive_class, T, ispointer>; \
    template<typename T, bool isbuildin> friend struct SerializeBuildin<archive_class, T, isbuilding>; \
    template<typename T, bool isarray> friend struct SerializeArray<archive_class, T, isarray>; */
#endif    
        
        ///////////////////////////////////
        template<class TArchive, typename T, bool ispointer>
        struct SerializePointer{};
        
        template<class TArchive, typename T>
        struct SerializePointer<TArchive, T, true>
        {
            // insert a ptr node
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                if( archive.value_pointer(v.value()) )
                {
                    // content pointed the this pointer is not been serialized
                    // so, do it
                    archive & SG_MAKE_NVP( v.name(),  *(v.value()) );
                }
                return archive;
            }
        };
        
        template<class TArchive, typename T>
        struct SerializePointer<TArchive, T, false>
        {
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                archive & v.value();
                return archive;
            }
        };
        
        ////////////////////////////////////
        template<class TArchive, typename T, bool isbuildin>
        struct SerializeBuildin{};

        template<class TArchive, typename T>
        struct SerializeBuildin<TArchive, T, true>
        {
            // build in type
            // add an attribute to the latest node
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                if(v.is_array_element())
                {
                    archive.value_node(v.name(), v.value());
                }
                else
                {
                    archive.value_attribute( v.name(), v.value() );
                }
                return archive;
            }
        };

        template<class TArchive, typename T>
        struct SerializeBuildin<TArchive, T, false>
        {
            // user defined type
            // add a node
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                archive.start_node(v.name());
                
                SerializePointer<TArchive, T, tmp::is_pointer<T>::value>::serialize(archive, v);
                
                archive.end_node(v.name());

                return archive;
            }
        };
        
        /////////////////////////////////////
        template<class TArchive, typename T, bool isarray>
        struct SerializeArray{};
        
        template<class TArchive, typename T>
        struct SerializeArray<TArchive, T, true>
        {
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                archive.start_node(v.name());
                
                T &arr = v.value();
                size_t count = sizeof(arr) / sizeof(arr[0]);
                for(size_t i=0; i<count; ++i)
                {
                    archive & serialization::make_nvp(sgStringUtil::to_string(i).c_str(), arr[i], true);
                }
                
                archive.end_node(v.name());
                return archive;
            }
        };
        
        template<class TArchive, typename T>
        struct SerializeArray<TArchive, T, false>
        {
            static TArchive & serialize(TArchive &archive, const serialization::nvp<T> &v)
            {
                return SerializeBuildin<TArchive, T, tmp::is_buildin_type<T>::value>::serialize(archive, v);
            }

        };
            
            
        template<class TArchive, typename T>
	    TArchive & operator&(TArchive &archive, const serialization::nvp<T> &v)
	    {
            std::string filename = v.filename();
	        if( ! filename.empty())
	        {
	            // reference file
	            // add node first
                archive.start_node(v.name());
                if( archive.start_doc(filename.c_str()) )
                {
                    // this file has not been serialized
                    serialize(archive, v);
                    archive.end_doc(filename.c_str());
                }
                archive.end_node(v.name());
                
	        }
	        else
	        {
	            serialize(archive, v);
	        }
            
            return archive;
	    }
    	
	    template<class TArchive, typename T>
        void serialize(TArchive &archive, const serialization::nvp<T> &v)
        {
            SerializeArray<TArchive, T, tmp::is_array<T>::value>::serialize(archive, v);
        }


	    template<class TArchive>
	    void serialize(TArchive &archive, const serialization::nvp<std::string> &v)
        {
            archive.value_attribute(v.name(), v.value());
        }
    	
	    /*template<class TArchive>
	    void serialize(TArchive &archive, const serialization::nvp<const char*> &v)
        {
            archive.value_attribute(v.name(), v.value());
        }*/
    	
	    /*template<class TArchive>
	    void serialize(TArchive &archive, const serialization::nvp<char*> &v)
        {
            archive.value_attribute(v.name(), v.value());
        }*/
    	
	    template<class TArchive, typename T>
	    void serialize(TArchive &archive, const serialization::nvp<std::vector<T> > &v)
	    {
	        archive.start_node(v.name());

            std::vector<T> &arr = v.value();
            size_t count = arr.size();
            archive.value_attribute("vec_size", count);
            arr.resize(count);
            
            for(size_t i=0; i<count; ++i)
            {
                archive & serialization::make_nvp(sgStringUtil::to_string(i).c_str(), arr[i], true);
            }
            
            archive.end_node(v.name());
	    }
    }

} // namespace Sagitta

#endif // __SGBASEARCHIVE_H__
