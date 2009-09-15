//////////////////////////////////////////////////////
// file: SBitOP.h @ 2008-12-14 by Zhang Xiang
// declares of the class SBitOP
// SBitOP is a class ...
//////////////////////////////////////////////////////
#ifndef __SBITOP_H__
#define __SBITOP_H__

// INCLUDES //////////////////////////////////////////

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _CommonExport SBitOP{
	// member variables

	// constructors & destructor

	// member functions
	public:
		//  [12/14/2008 zhangxiang]
		/* Get the lower 16-bit value from an 32-bit word. */
		static unsigned long lo16(unsigned long x){ return x & 0xffff; }
		static unsigned long hi16(unsigned long x){ return x >> 16; }

	}; //#### end class SMenu

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SBITOP_H__