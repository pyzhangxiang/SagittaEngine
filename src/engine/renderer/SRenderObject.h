//////////////////////////////////////////////////////
// file: SRenderObject.h @ 2008-7-31 by Zhang Xiang
// declares of the class SRenderObject
// SRenderObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDEROBJECT_H__
#define __SRENDEROBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SRenderObject : public SObject{
	// member variables

	// constructors & destructor
	private:
		SRenderObject(const SRenderObject &);
		SRenderObject &operator=(const SRenderObject &);
	public:
		SRenderObject(void){}
		virtual ~SRenderObject(void) = 0;

	// member functions

	}; //#### end class SRenderObject

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDEROBJECT_H__