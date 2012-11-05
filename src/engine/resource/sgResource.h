//////////////////////////////////////////////////////
// file: sgResource.h 
// created by zhangxiang on 09-01-09
// declares of the class sgResource
// sgResource is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRESOURCE_H__
#define __SGRESOURCE_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgResourceCenter;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgResource : public sgObject
	{
		SG_META_DECLARE_ABSTRACT(sgResource)

        friend class sgResourceCenter;
	protected:
		sgStrHandle mFilename;

	// constructors & destructor
	public:
		sgResource(void);
		virtual ~sgResource(void) = 0;
		
		const sgStrHandle &getFilename(void) const;
        
    private:
        void setFilename(const sgStrHandle &filename);
        
    protected:
        // called in setFilename
        // if need, you could overwrite it to load your resource by the filename here
        virtual void onSetFilename(void){}

	}; //#### end class sgResource

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRESOURCE_H__
