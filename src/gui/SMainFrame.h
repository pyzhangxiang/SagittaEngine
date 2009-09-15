//////////////////////////////////////////////////////
// file: SMainFrame.h @ 2008-12-11 by Zhang Xiang
// declares of the class SMainFrame
// SMenu is a class ...
//////////////////////////////////////////////////////
#ifndef __SMAINFRAME_H__
#define __SMAINFRAME_H__

// INCLUDES //////////////////////////////////////////
#include "SWindow.h"
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWidget;
	class SMainFrameDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SMainFrame : public SWindow{
	// member variables

	// constructors & destructor
	public:
		SMainFrame(SMainFrameDelegate *aDelegate, const std::string &aTitle, SRect aRect, SMainFrame *aParent);
		SMainFrame(const std::string &aTitle, SRect aRect, SMainFrame *aParent);
		virtual ~SMainFrame(void) = 0;

	// member functions
	protected:
		/** Before closes self, ask all children to close. Overrides from SWindow. */
		bool onClose(void);

	public:
		

	}; //#### end class SMainFrame

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SMAINFRAME_H__