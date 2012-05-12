//////////////////////////////////////////////////////
// file: sgResourceDelegate.h 
// created by zhangxiang on 09-01-09
// declare of the class sgResourceDelegate
// sgResourceDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRESOURCEDELEGATE_H__
#define __SGRESOURCEDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgResource;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgResourceDelegate : public sgDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. */
		virtual void loadImpl(const StdString &aFileName, sgResource *outResource) = 0;

	public:
		void load(const StdString &aFileName, int aLoadOption, sgResource *outResource);

	}; //#### end class sgResourceDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRESOURCEDELEGATE_H__
