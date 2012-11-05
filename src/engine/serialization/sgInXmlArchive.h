#ifndef __SGINXMLARCHIVE_H__
#define __SGINXMLARCHIVE_H__

#include "sgXmlArchive.h"

namespace Sagitta
{   
    namespace serialization
    {
        class sgInXmlArchive : public sgXmlArchive
	    {
            //SG_SERIALIZATION_ARCHIVE(sgOutXmlArchive)
        private:
            typedef std::list<rapidxml::file<>*> FileList;
            FileList mFileList;

        public:
            sgInXmlArchive(const std::string filename);
            virtual ~sgInXmlArchive(void);
            
        protected:
            // read a document
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

                rapidxml::xml_node<> *new_node = node->first_node(name);
                assert(new_node);
                assert(strtk::string_to_type_converter(new_node->value(), value));
            }
            
            /// @ret false => existed, just ref, don't serialize the content again
            template<typename T>
            bool value_pointer(T * &ptr)
            {
                unsigned long long iptr;
                value_attribute(MS_PTR_REF, iptr);
                ptr = reinterpret_cast<T*>(iptr);
                
                // get current document
                Doc *doc = mDocStack.back();
                
                // if there's the same pointer has been serialized
                PtrMap::iterator it = doc->mPtrMap.find(ptr);
                if(it != doc->mPtrMap.end())
                {
                    // has serialized
                    // just let ptr pointer to the real address
                    ptr = (T*)(it->second);
                    return false;
                }
                else
                {
                    // new ptr, and serialize the data
                    // another case is checking if ptr == zero, if not, then do not new, just serialize to its data
                    // but this may cause exceptions ( if the ptr was not initialized to zero, it's value is undefined )
                    // so I choose always new a ptr
                    // although it may cause memory leak
                    T *key_ptr = ptr;
                    ptr = new T();
                    doc->mPtrMap.insert(std::make_pair(key_ptr, ptr));
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
                
                rapidxml::xml_attribute<> *attr = node->first_attribute(name);
                assert(attr);
                assert(strtk::string_to_type_converter(attr->value(), value));
            }
            
        };
        
    }

} // namespace Sagitta

#endif // __SGINXMLARCHIVE_H__
