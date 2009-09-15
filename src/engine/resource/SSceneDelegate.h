//////////////////////////////////////////////////////
// file: SSceneDelegate.h 
// created by zhangxiang on 09-01-09
// declare of the class SSceneDelegate
// SSceneDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SSCENEDELEGATE_H__
#define __SSCENEDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "SResourceDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SScene;
	class SSceneNode;
	class SFile;
	class SAnimation;
	class BindingMap;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSceneDelegate : public SResourceDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from SResourceDelegate. */
		void loadImpl(const StdString &aFileName, SResource *outResource);

		/** Loads the binded scene from .obj file. */
		void loadScene_obj(const StdString &aFileName, SResource *outResource);

	}; //#### end class SSceneDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSkeletonDelegate : public SSceneDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from SResourceDelegate. */
		void loadImpl(const StdString &aFileName, SResource *outResource);

		/** Loads the binded scene from .bvh file. */
		void loadSkeleton_bvh(const StdString &aFileName, SScene *outScene);

		/** Loads bvh skeleton recursively. */
		SSceneNode *loadBVHSkeletonRecursively(SFile &file, SScene *outScene);

		/** Attaches bone spheres recursively. */
		void setupJointSpheres(SSceneNode *aSNode, Real aRadius);

		/** Sets binding info recursively, by name. */
		void setBindingInfoRecursively(SSceneNode *aSNode, SAnimation *aAnima, BindingMap &outBindingMap);

	}; //#### end class SSkeletonDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSCENEDELEGATE_H__