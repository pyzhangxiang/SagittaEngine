#ifndef __SGOUTXMLARCHIVE_H__
#define __SGOUTXMLARCHIVE_H__

#include "sgXmlArchive.h"

/*
int main()
{    
     file<> fdoc("config.xml");
     std::cout<<fdoc.data()<<std::endl; 
     xml_document<>  doc;    
     doc.parse<0>(fdoc.data()); 

     std::cout<<doc.name()<<std::endl;
     
     //! 获取根节�?     xml_node<>* root = doc.first_node();
     std::cout<<root->name()<<std::endl;

     //! 获取根节点第一个节�?     xml_node<>* node1 = root->first_node();
     std::cout<<node1->name()<<std::endl; 

     xml_node<>* node11 = node1->first_node();
     std::cout<<node11->name()<<std::endl;
     std::cout<<node11->value()<<std::endl;
     
     //! 修改之后再次保存
     xml_node<>* size = root->first_node("size");
     size->append_node(doc.allocate_node(node_element,"w","0"));
     size->append_node(doc.allocate_node(node_element,"h","0"));

     std::string text;  
     rapidxml::print(std::back_inserter(text),doc,0);  
 
     std::cout<<text<<std::endl; 
    
     std::ofstream out("config.xml");
     out << doc;

     system("PAUSE");
     return EXIT_SUCCESS;
}
*/

namespace Sagitta
{   
    namespace serialization
    {
        class sgOutXmlArchive : public sgXmlArchive
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
