
#include "sgOutXmlArchive.h"
//#include "strtk/strtk.hpp"
#include <fstream>
#include <string>
#include <list>

namespace Sagitta
{
    namespace serialization
    {
        sgOutXmlArchive::sgOutXmlArchive( const std::string filename, size_t version /*= 0*/ )
            : sgXmlArchive(version)
        {
            std::string fname = filename;
            if(filename.size() > FILE_EXT.size() &&
                filename.substr(filename.size() - FILE_EXT.size()) == FILE_EXT)
            {
                fname = filename.substr(0, filename.size() - FILE_EXT.size());
            }
            // push the root document
            mDocStack.push_back(addDocument(fname));
        }

        sgOutXmlArchive::~sgOutXmlArchive( void )
        {

        }

        sgOutXmlArchive::Doc * sgOutXmlArchive::addDocument( const std::string &filename )
        {
            // add new doc
            Doc *doc = new Doc;
            mDocStorage.insert(std::make_pair(filename, doc)); // store the doc

            doc->mFilename = filename;

            rapidxml::xml_node<> *dec = doc->mDoc.allocate_node(rapidxml::node_pi, doc->mDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
            doc->mDoc.append_node(dec);

            rapidxml::xml_node<> *node = doc->mDoc.allocate_node(rapidxml::node_element, MS_ROOT_NAME);
            doc->mDoc.append_node(node);
            doc->mNodeStack.push_back(node);

            return doc;
        }  

        void sgOutXmlArchive::write( void )
        {
            DocMap::iterator it = mDocStorage.begin();
            for(; it!=mDocStorage.end(); ++it)
            {
                Doc *doc = it->second;
                std::ofstream out((doc->mFilename + FILE_EXT).c_str());
                if(out.fail())
                {
                    throw doc->mFilename + " failed";
                }
                out << doc->mDoc;
            }
        }

        bool serialization::sgOutXmlArchive::start_doc( const char *filename )
        {
            // add an attribute to reference this file
            Doc *doc = mDocStack.back();
            value_attribute(MS_FILE_REF, filename);

            // check if the document has been serialized
            DocMap::iterator it = mDocStorage.find(filename);
            if(it != mDocStorage.end())
            {
                return false;
            }

            // push new document in the stack
            mDocStack.push_back(addDocument(std::string(filename)));

            return true;
        }

        bool serialization::sgOutXmlArchive::end_doc( const char *filename )
        {
            Doc *doc = mDocStack.back();
            if(strcmp(doc->mFilename.c_str(), filename) != 0)
                return false;

            mDocStack.pop_back();
            return true;
        }

        void serialization::sgOutXmlArchive::start_node( const char *name )
        {
            // get current document
            Doc *doc = mDocStack.back();

            // get curent node
            rapidxml::xml_node<> *node = doc->mNodeStack.back();

            char *str_name = doc->mDoc.allocate_string( name );
            rapidxml::xml_node<> *new_node = doc->mDoc.allocate_node(rapidxml::node_element, str_name);
            node->append_node(new_node);

            // push the new node the the stack
            // now the new node will be the latest node
            // the build-in type would be serialized as attributes for the node
            doc->mNodeStack.push_back(new_node);
        }

        bool serialization::sgOutXmlArchive::end_node( const char *name )
        {
            // get current document
            Doc *doc = mDocStack.back();            
            // get curent node
            rapidxml::xml_node<> *node = doc->mNodeStack.back();

            if(strcmp(node->name(), name) != 0)
                return false;

            doc->mNodeStack.pop_back();
            return true;
        }
    }
    

} // namespace Sagitta


