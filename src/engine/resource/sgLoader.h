
#ifndef __SGLOADER_H__
#define __SGLOADER_H__

#include "engine/common/sgObject.h"
#include <string>

namespace Sagitta{

    class sgSceneObject;
    class sgSkeleton;
    class sgAnimation;
    class sgTexture;
    
	class _SG_KernelExport sgLoader
	{
	public:
		static sgSceneObject *load_obj(const std::string &filename);
        static sgSkeleton *load_bvh_skeleton(const std::string &filename);
        static sgAnimation *load_bvh_animation(const std::string &filename);
        static sgTexture *load_texture(const std::string &filename);
	};
	
	
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGLOADER_H__
