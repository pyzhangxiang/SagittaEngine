//////////////////////////////////////////////////////
// file: SWidget.h @ 2008-12-11 by Zhang Xiang
// declares of the class SWidget
// SWidget is a class ...
//////////////////////////////////////////////////////
#ifndef __SWIDGET_H__
#define __SWIDGET_H__

// INCLUDES //////////////////////////////////////////
#include "SWindow.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SMainFrame;
	class SWidgetDelegate;
	class SEvent;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SWidget : public SWindow{
	// enum defines
	public:
		enum SizePolicy{
			SP_FIXED,
			SP_RELEVANT,
			SP_FILLED
		};

	// member variables
	private:
		SizePolicy m_SizePolicy;

		/** Stores if parent's properties are changed. */
		/*	--- Outmoded ---
		bool m_bParentChanged;
		*/

		// relevant properties to parent
		/** Stores the scale of the x coordinate of the upper-left corner to parent's. */
		int m_iScreenX;
		/** Stores the scale of the y coordinate of the upper-left corner to parent's. */
		int m_iScreenY;
		/** Stores the scale value to parent's width. */
		float m_fWidthScale;
		/** Stores the scale value to parent's height. */
		float m_fHeightScale;

	// constructors & destructor
	public:
		/** Constructor.
		*	@param
		*		aDelegate Must be a SWidgetDelegate.
		*		aParent Cannot be zero, or will cannot create window and will throw a sgInternalErrorException.
		*/
		SWidget(SWidgetDelegate *aDelegate, SMainFrame *aParent, const std::string &aTitle, SRect aRect);
		SWidget(SMainFrame *aParent, const std::string &aTitle, SRect aRect);
		virtual ~SWidget(void) = 0;

	// member functions
	protected:
		/** Handles events, overrides from SWindow.		--- Outmoded ---
		*	@remarks
		*		Specially handle paint event for check if need update.
		*		For other event, call for SWindow::handleEvent to process.
		*/
		//SStatus handleEvent(const SEvent &aEvent);

	private:
		/** Moves to a new position(parent's client-coordinate) and scales this widget size by size policy. */
		bool policyMove(int aParentNewX, int aParentNewY, int aParentNewWidth, int aParentNewHeight);

	protected:
		/** Only called in pain event handler. Before paint, update.*/
		virtual void _update(void);

	public:
		/** Gets size policy. */
		SizePolicy sizepolicy(void) const;
		/** Sets size policy. */
		void setSizePolicy(SizePolicy aSizePolicy);

		/** While parent's properties are changed, notifies children that should update. */
		void _needUpdate(void);


	}; //#### end class SWidget

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SWIDGET_H__