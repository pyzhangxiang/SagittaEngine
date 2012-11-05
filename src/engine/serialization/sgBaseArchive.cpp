#include "sgBaseArchive.h"

namespace Sagitta
{   
    namespace serialization
    {
        const char *sgBaseArchive::MS_PTR_REF = "__sg_internal_ptr__";
        const char *sgBaseArchive::MS_FILE_REF = "__sg_internal_ref__";
        const char *sgBaseArchive::MS_ROOT_NAME = "root";

        sgBaseArchive::sgBaseArchive( const std::string &file_ext, size_t version ) : FILE_EXT(file_ext), mVersion(version)
        {

        }
    }

} // namespace Sagitta


