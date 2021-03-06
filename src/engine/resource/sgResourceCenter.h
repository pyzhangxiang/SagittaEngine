//////////////////////////////////////////////////////
// file: sgResourceCenter.h 
// created by zhangxiang on 09-01-11
//////////////////////////////////////////////////////
#ifndef __SGRESOURCEMANAGER_H__
#define __SGRESOURCEMANAGER_H__

#include "engine/common/sgObject.h"
#include "engine/common/Singleton.h"
#include "engine/common/sgStlAllocator.h"
#include <map>
#include <string>


namespace Sagitta{

	class sgResource;
	class sgImageLoader;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgResourceCenter 
    : public sgMemObject, public sgManuallySingleton<sgResourceCenter>
    {
		//SG_META_DECLARE(sgResourceCenter)
	// type defines
	protected:
		/// key_value is resource's id
		typedef sg_map(sgStrHandle, sgResource*) ResourceMap;
		typedef sg_map(std::string, std::string) ImageLoaderTypeMap;

	// member variables
	private:
		/// collection of resources mapped by id
		ResourceMap m_ResourceMap;
        
        std::string mRootDir;

		ImageLoaderTypeMap mImageLoaderTypeMap;

	// constructors & destructor
	public:
        sgResourceCenter(void);
		~sgResourceCenter(void);

	// member functions
	private:
		/** Adds a resource. */
		void addResource(sgResource *aResource);

		/** Removes a resource by id. */
		void removeResource(const sgStrHandle &filename);

		/** Removes a resource. */
		void removeResource(sgResource *aResource);
        
    public:
        sgResource *findResource(const sgStrHandle &filename);
        sgResource *createResource(const sgStrHandle &type, const sgStrHandle &filename);
        bool destroyResource(sgResource *resource);
        bool destroyResource(const sgStrHandle &filename);
        
        const std::string &getRootDir(void) const{ return mRootDir; }
        void setRootDir(const std::string &dir);
        std::string getResourcePath(const std::string &subname);

		/// register image loader
		/// imageFileExt e.g. .png, .tga, .bmp, .jpg ...
		bool registerImageLoader(const std::string &imageFileExt, const sgStrHandle &imageLoaderType);
		sgImageLoader *getImageLoader(const std::string &imageFileExt) const;

	}; //#### end class sgResourceCenter

} // namespace Sagitta



#endif // __SGRESOURCEMANAGER_H__
