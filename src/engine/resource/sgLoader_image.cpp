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
#include "engine/renderer/sgRenderer.h"
#include <stb_image/stb_image.h>

namespace Sagitta{

	uByte *sgLoader::load_image(const std::string &filename, int &width, int &height, int &comps)
    {
        std::string fullpath = sgResourceCenter::instance()->getResourcePath(filename);
        
        uByte *imgData = stbi_load(fullpath.c_str(), &width, &height, &comps, 0);
        if(imgData == NULL)
        {
            sgLogSystem::instance()->error(stbi_failure_reason());
            return NULL;
        }
		return imgData;
    }

	void sgLoader::free_image_data( uByte *data )
	{
		if(data)
			stbi_image_free(data);
	}
    
} // namespace Sagitta

