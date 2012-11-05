

#include "sgIOUtil.h"

namespace Sagitta{

    UInt32 sgGetFileLength(std::ifstream &stream)
    {
        if(!stream.good()) return 0;
        
        /*UInt32 pos=*/stream.tellg();
        stream.seekg(0, std::ios::end);
        UInt32 len = stream.tellg();
        stream.seekg(std::ios::beg);
        
        return len;
    }
    
} // namespace Sagitta
