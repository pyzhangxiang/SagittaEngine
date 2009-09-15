//////////////////////////////////////////////////////
// file: SResourceDelegate.h 
// created by zhangxiang on 09-01-09
// declare of the class SResourceDelegate
// SResourceDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SRESOURCEDELEGATE_H__
#define __SRESOURCEDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SResource;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SResourceDelegate : public SDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. */
		virtual void loadImpl(const StdString &aFileName, SResource *outResource) = 0;

	public:
		void load(const StdString &aFileName, int aLoadOption, SResource *outResource);

	}; //#### end class SResourceDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRESOURCEDELEGATE_H__