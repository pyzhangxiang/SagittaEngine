//////////////////////////////////////////////////////
// file: STerrain.h @ 2008-1-31 by Zhang Xiang
// declares of the class STerrain
// STerrain is a class ...
//////////////////////////////////////////////////////
#ifndef __STERRAIN_H__
#define __STERRAIN_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderable.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport STerrain : public SRenderable{
	// member variables

	// constructors & destructor
	public:
		STerrain(void);
		~STerrain(void);

	// member functions
	public:
		void computeCenter(void);
		void computeRadius(void);

	public:

	}; //#### end class STerrain

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __STERRAIN_H__