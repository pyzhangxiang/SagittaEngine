//////////////////////////////////////////////////////
// file: sgRenderObject.h @ 2008-7-31 by Zhang Xiang
// declares of the class sgRenderObject
// sgRenderObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRENDEROBJECT_H__
#define __SGRENDEROBJECT_H__

// INCLUDES //////////////////////////////////////////
//#include "../../common/sgObject.h"
#include "../../common/configs/config.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgRenderObject{// : public sgObject{
	// member variables

	// constructors & destructor
	private:
		sgRenderObject(const sgRenderObject &);
		sgRenderObject &operator=(const sgRenderObject &);
	public:
		sgRenderObject(void){}
		virtual ~sgRenderObject(void) = 0;

	// member functions

	}; //#### end class sgRenderObject

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRENDEROBJECT_H__

