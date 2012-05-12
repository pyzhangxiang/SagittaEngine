//////////////////////////////////////////////////////
// file: sgSceneDelegate.h 
// created by zhangxiang on 09-01-09
// declare of the class sgSceneDelegate
// sgSceneDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSCENEDELEGATE_H__
#define __SGSCENEDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "sgResourceDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgScene;
	class sgSceneNode;
	class sgFile;
	class sgAnimation;
	class BindingMap;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSceneDelegate : public sgResourceDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from sgResourceDelegate. */
		void loadImpl(const StdString &aFileName, sgResource *outResource);

		/** Loads the binded scene from .obj file. */
		void loadScene_obj(const StdString &aFileName, sgResource *outResource);

	}; //#### end class sgSceneDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport SSkeletonDelegate : public sgSceneDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from sgResourceDelegate. */
		void loadImpl(const StdString &aFileName, sgResource *outResource);

		/** Loads the binded scene from .bvh file. */
		void loadSkeleton_bvh(const StdString &aFileName, sgScene *outScene);

		/** Loads bvh skeleton recursively. */
		sgSceneNode *loadBVHSkeletonRecursively(sgFile &file, sgScene *outScene);

		/** Attaches bone spheres recursively. */
		void setupJointSpheres(sgSceneNode *aSNode, Real aRadius);

		/** Sets binding info recursively, by name. */
		void setBindingInfoRecursively(sgSceneNode *aSNode, sgAnimation *aAnima, BindingMap &outBindingMap);

	}; //#### end class SSkeletonDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENEDELEGATE_H__

