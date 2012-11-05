#include "sgXmlArchive.h"
#include "sg_serialization_nvp.h"
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <vector>


namespace Sagitta
{   
    namespace serialization
    {
        sgXmlArchive::sgXmlArchive( size_t version )
            : sgBaseArchive(".xml", version)
        {

        }

        sgXmlArchive::~sgXmlArchive( void )
        {
            DocMap::iterator it = mDocStorage.begin();
            for(; it!=mDocStorage.end(); ++it)
            {
                delete it->second;
            }
        }

        void sgXmlArchive::dump( void )
        {
            DocMap::iterator it = mDocStorage.begin();
            for(; it!=mDocStorage.end(); ++it)
            {
                Doc *doc = it->second;
                std::cout << "\nStart dumping xml: " << doc->mFilename + FILE_EXT << "\n";
                print(std::cout, doc->mDoc, 0);
                std::cout << "End   dumping  xml: " << doc->mFilename + FILE_EXT << "\n";
            }
        }

        
    }


    

} // namespace Sagitta


