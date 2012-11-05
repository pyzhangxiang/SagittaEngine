
#include "sgInXmlArchive.h"
#include "strtk/strtk.hpp"
#include <fstream>
#include <string>
#include <list>
#include <cassert>

namespace Sagitta
{
    namespace serialization
    {
        sgInXmlArchive::sgInXmlArchive( const std::string filename )
            : sgXmlArchive(0)
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

        sgInXmlArchive::~sgInXmlArchive( void )
        {
            FileList::iterator it = mFileList.begin();
            for(; it!=mFileList.end(); ++it)
            {
                delete (*it);
            }
        }

        sgInXmlArchive::Doc * sgInXmlArchive::addDocument( const std::string &filename )
        {
            std::string rfilename = filename + FILE_EXT;

            // add new doc
            Doc *doc = new Doc;
            mDocStorage.insert(std::make_pair(filename, doc)); // store the doc

            doc->mFilename = filename;

            rapidxml::file<> *fdoc = new rapidxml::file<>(rfilename.c_str());
            mFileList.push_back(fdoc);
            doc->mDoc.parse<0>(fdoc->data()); 


            rapidxml::xml_node<> *node = doc->mDoc.first_node(MS_ROOT_NAME);
            assert( node );

            doc->mNodeStack.push_back(node);

            return doc;
        }

        bool serialization::sgInXmlArchive::start_doc( const char *filename )
        {
            // add an attribute to reference this file
            Doc *doc = mDocStack.back();

            std::string ref_filename = "";
            value_attribute(MS_FILE_REF, ref_filename);

            // check if the document has been serialized
            DocMap::iterator it = mDocStorage.find(filename);
            if(it != mDocStorage.end())
            {
                // push the doc onto the stack
                mDocStack.push_back(it->second);
                
                // push it's root node onto the it's stack
                rapidxml::xml_node<> *node = it->second->mDoc.first_node(MS_ROOT_NAME);
                assert( node );

                it->second->mNodeStack.push_back(node);
            }
            else
            {
                // load new document in the stack
                mDocStack.push_back(addDocument(std::string(filename)));
            }
            

            return true;
        }

        bool serialization::sgInXmlArchive::end_doc( const char *filename )
        {
            Doc *doc = mDocStack.back();
            if(strcmp(doc->mFilename.c_str(), filename) != 0)
                return false;

            // clear doc's node stack
            // because this doc may be used by other element
            doc->mNodeStack.clear();

            // pop out this doc
            mDocStack.pop_back();

            return true;
        }

        void serialization::sgInXmlArchive::start_node( const char *name )
        {
            // get current document
            Doc *doc = mDocStack.back();

            // get curent node
            rapidxml::xml_node<> *node = doc->mNodeStack.back();

            rapidxml::xml_node<> *new_node = node->first_node(name);
            assert(new_node);

            // push the new node the the stack
            // now the new node will be the latest node
            // the build-in type would be serialized as attributes for the node
            doc->mNodeStack.push_back(new_node);
        }

        bool serialization::sgInXmlArchive::end_node( const char *name )
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


