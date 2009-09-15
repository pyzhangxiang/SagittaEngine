//////////////////////////////////////////////////////
// file: SSubScene.h 
// created by zhangxiang on 09-01-06
// declares of the class SSubScene
// SSubScene is a class ...
//////////////////////////////////////////////////////
#ifndef __SSUBSCENE_H__
#define __SSUBSCENE_H__

// INCLUDES //////////////////////////////////////////
#include "SScene.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SSceneDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSubScene : public SScene{
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
		SSubScene(int aType = ST_UNKNOWN);
		SSubScene(const StdString &aName, int aType = ST_UNKNOWN);
		SSubScene(SSceneDelegate *aDelegate, int aType = ST_UNKNOWN);
		SSubScene(const StdString &aName, SSceneDelegate *aDelegate, int aType = ST_UNKNOWN);
		virtual ~SSubScene(void) = 0;

	// member functions
	public:
		/** Gets my type. */
		int type(void) const;

	}; //#### end class SSubScene

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSUBSCENE_H__