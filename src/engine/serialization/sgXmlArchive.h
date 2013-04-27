#ifndef __SGXMLARCHIVE_H__
#define __SGXMLARCHIVE_H__

#include "sgBaseArchive.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "strtk/strtk.hpp"
#include <string>
#include <list>
#include <map>
#include <vector>


namespace Sagitta
{   
    namespace serialization
    {
        class _SG_KernelExport sgXmlArchive : public sgBaseArchive
	    {
        protected:
            typedef std::map<void*, void*> PtrMap;
	        typedef std::list< rapidxml::xml_node<>* > NodeStack;
	        struct Doc
	        {
	            rapidxml::xml_document<> mDoc;
                NodeStack mNodeStack;
                std::string mFilename;

                PtrMap mPtrMap;
            };
            typedef std::list<Doc*> DocStack;
            typedef std::map<std::string, Doc*> DocMap;
            DocStack mDocStack;         // doc stack, will push and pop
            DocMap mDocStorage;         // doc storage, only push
                             
        public:
            sgXmlArchive(size_t version);
            
            virtual ~sgXmlArchive(void);

            void dump(void);
            
        protected:
            virtual Doc *addDocument(const std::string &filename) = 0;
                        
        };
        
    }

} // namespace Sagitta

#endif // __SGXMLARCHIVE_H__
