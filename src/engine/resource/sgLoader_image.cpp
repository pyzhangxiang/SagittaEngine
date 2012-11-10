// [10/11/2012  zhangxiang]

#include "sgLoader.h"
#include "sgResource.h"
#include "sgResourceCenter.h"
#include "sgTexture.h"
#include "engine/buffer/sgFrameBuffer.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgUtil.h"
#include "engine/common/sgException.h"
#include "engine/common/sgLogSystem.h"
#include <stb_image/stb_image.h>

namespace Sagitta{

	sgTexture *sgLoader::load_texture(const std::string &filename)
    {

        
        std::string fullpath = sgResourceCenter::instance()->getResourcePath(filename);
        
        int width, height, comps;
        uByte *imgData = stbi_load(fullpath.c_str(), &width, &height, &comps, 0);
        if(imgData == NULL)
        {
            sgLogSystem::instance()->error(stbi_failure_reason());
            return NULL;
        }
        
        sgTexture *pTexture = (sgTexture*)sgResourceCenter::instance()->createResource(sgTexture::GetClassName(), filename.c_str());
        
        sgFrameBuffer *buffer = pTexture->getBuffer();
        buffer->resize(width, height, comps * sizeof(uByte));
        memcpy(buffer->data(), imgData, width * height * comps * sizeof(uByte));
        
        stbi_image_free(imgData);
        
        return pTexture;
        
    }
    
} // namespace Sagitta

