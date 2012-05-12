//////////////////////////////////////////////////////
// file: SGraphicsFrame.h @ 2007-12-3 by Zhang Xiang
// declaration of the class SGraphicsFrame
// SGraphicsFrame is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRAPHICSFRAME_H__
#define __SGRAPHICSFRAME_H__

// INCLUSIONS ////////////////////////////////////////
#include "SMainFrame.h"

// DECLARATIONS //////////////////////////////////////
namespace Sagitta{

	class SMainFrameDelegate;
	class sgSceneManager;
	class SContext;

	class _SG_GuiExport SGraphicsFrame : public SMainFrame{
	// member variables
	protected:
		/** Stores the pointer to a SceneManager and holds the ownership. */
		sgSceneManager *m_pSceneManager;

	// constructors & destructor
	public:
		SGraphicsFrame(SMainFrameDelegate *aDelegate, const std::string &aTitle = "New Window", SRect aRect = SRect(0, 0, 170, 170), SMainFrame *aParent = 0);
		SGraphicsFrame(const std::string &aTitle = "New Window", SRect aRect = SRect(0, 0, 170, 170), SMainFrame *aParent = 0);
		virtual ~SGraphicsFrame(void);

	// member functions
	private:
		void init(void);

	protected:
		/** Take charge the erase background event, so that the os could not erase it. Overrides from SWindow. */
		bool onEraseBkgnd(const SContext &aContext);
		
	};

} // namespace Sagitta

#endif // __SGRAPHICSFRAME_H__
