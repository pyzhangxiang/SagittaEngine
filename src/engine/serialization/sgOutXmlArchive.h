#ifndef __SGOUTXMLARCHIVE_H__
#define __SGOUTXMLARCHIVE_H__

#include "sgXmlArchive.h"

namespace Sagitta
{   
    namespace serialization
    {
        class _SG_KernelExport sgOutXmlArchive : public sgXmlArchive
	    {
            //SG_SERIALIZATION_ARCHIVE(sgOutXmlArchive)

        public:
            sgOutXmlArchive(const std::string filename, size_t version = 0);
            virtual ~sgOutXmlArchive(void);
            
            void write(void);

        protected:
            // add a document
            virtual Doc *addDocument(const std::string &filename);
                       
        public:
            /// @ret false => this document has been serialized, just ref
            bool start_doc(const char *filename);            
            bool end_doc(const char *filename);
            
            void start_node(const char *name);   
            bool end_node(const char *name);
            
            template<typename T>
            void value_node(const char *name, T &value)
            {
                // get current document
                Doc *doc = mDocStack.back();
                // get curent node
                rapidxml::xml_node<> *node = doc->mNodeStack.back();

                char *str_name = doc->mDoc.allocate_string( name );
                char *str_v = doc->mDoc.allocate_string( strtk::type_to_string(value).c_str() );
                rapidxml::xml_node<> *new_node = doc->mDoc.allocate_node(rapidxml::node_element, str_name, str_v);
                node->append_node(new_node);
            }
            
            /// @ret false => existed, just ref, don't serialize the content again
            template<typename T>
            bool value_pointer(T * &ptr)
            {
                void *vptr = (void*)ptr;
                unsigned long long ptr_value = reinterpret_cast<unsigned long long>(ptr);
                value_attribute(MS_PTR_REF, ptr_value);
                
                // get current document
                Doc *doc = mDocStack.back();
                
                // if there's the same pointer has been serialized
                PtrMap::iterator it = doc->mPtrMap.find(vptr);
                if(it != doc->mPtrMap.end())
                {
                    // has serialized, don't serialize the content again
                    return false;
                }
                else
                {
                    // 
                    doc->mPtrMap.insert(std::make_pair(vptr, vptr));
                    return true;
                }
                
            }
            
            template<typename T>
            void value_attribute(const char *name, T &value)
            {
                // get current document
                Doc *doc = mDocStack.back();
                 // get curent node           
                rapidxml::xml_node<> *node = doc->mNodeStack.back();
                
                char *str_name = doc->mDoc.allocate_string( name );
                char *str_v = doc->mDoc.allocate_string( strtk::type_to_string(value).c_str() );
                rapidxml::xml_attribute<> *attr = doc->mDoc.allocate_attribute(str_name, str_v);
                node->append_attribute(attr);
            }
            
        };
        
    }

} // namespace Sagitta

#endif // __SGOUTXMLARCHIVE_H__
