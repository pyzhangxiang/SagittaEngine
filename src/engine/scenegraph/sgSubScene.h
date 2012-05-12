//////////////////////////////////////////////////////
// file: sgSubScene.h 
// created by zhangxiang on 09-01-06
// declares of the class sgSubScene
// sgSubScene is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSUBSCENE_H__
#define __SGSUBSCENE_H__

// INCLUDES //////////////////////////////////////////
#include "sgScene.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgSceneDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSubScene : public sgScene{
	// enum declares
	public:
		// scene type
		enum SceneType{ ST_UNKNOWN = 0 };

	// member variables
	private:
		/// scene type
		const int mc_iType;

	// constructors & destructor
	public:
		sgSubScene(int aType = ST_UNKNOWN);
		sgSubScene(const StdString &aName, int aType = ST_UNKNOWN);
		sgSubScene(sgSceneDelegate *aDelegate, int aType = ST_UNKNOWN);
		sgSubScene(const StdString &aName, sgSceneDelegate *aDelegate, int aType = ST_UNKNOWN);
		virtual ~sgSubScene(void) = 0;

	// member functions
	public:
		/** Gets my type. */
		int type(void) const;

	}; //#### end class sgSubScene

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSUBSCENE_H__

