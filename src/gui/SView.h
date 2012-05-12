//////////////////////////////////////////////////////
// file: SView.h @ 2008-12-12 by Zhang Xiang
// declares of the class SView
// SView is a class ...
//////////////////////////////////////////////////////
#ifndef __SVIEW_H__
#define __SVIEW_H__

// INCLUDES //////////////////////////////////////////
#include "SWidget.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SViewDelegate;
	class SMainFrame;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SView : public SWidget{
	// member variables

	// constructors & destructor
	public:
		/** Constructor.
		*	@param
		*		aDelegate A pointer to SViewDelegate. Should not be zero. If don't want to
		*				create it, then use the other constructor. That will create a default delegate
		*				by itself.
		*	@param
		*		aParent The parent window for this view. Cannot be zero, or will throw an exception. 
		*/
		SView(SViewDelegate *aDelegate, SMainFrame *aParent, const std::string &aTitle, SRect aRect);
		/** Constructor
		*	@remarks
		*		Will create a default SViewDelegate by itself.
		*	@param
		*		aParent The parent window for this view. Cannot be zero, or will throw an exception. 
		*/
		SView(SMainFrame *aParent, const std::string &aTitle, SRect aRect);
		virtual ~SView(void) = 0;

	// member functions

	}; //#### end class SView

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SVIEW_H__