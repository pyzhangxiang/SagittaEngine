//////////////////////////////////////////////////////
// file: SSceneManager.h @ 2008-1-31 by Zhang Xiang
// declares of the class SSceneManager
// SSceneManager is a class ...
//////////////////////////////////////////////////////
#ifndef __SSCENEMANAGER_H__
#define __SSCENEMANAGER_H__

// INCLUDES //////////////////////////////////////////
#include "SScene.h"
#include "../renderer/SRenderOption.h"
#include "../../math/SColor.h"
#include "../../common/utils/SIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SSubScene;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSceneManager : public SScene{
	// type defines
	protected:
		typedef std::map<SSceneJoint*, SSubScene*> SubSceneMap;
		typedef std::map<int, SubSceneMap*> TypedSubSceneMap;

	public:
		typedef SConstMapIterator<SubSceneMap> ConstTypedSubSceneIterator;
		typedef SConstCMPMapIterator<TypedSubSceneMap, SubSceneMap> ConstTotalSubSceneIterator;

	// member variables
	protected:
		/// the global ambient color
		Color m_GLobalAmbientColor;

		/// global render option
		SRenderOption m_GlobalRenderOption;

		int m_iCurrentFrame;

		/// mapped by type
		TypedSubSceneMap m_SubSceneMap;

	// constructors & destructor
	public:
		SSceneManager(void);
		SSceneManager(const StdString &aName);
		virtual ~SSceneManager(void);

	// member functions
	public:
		/** Gets the globe ambient color. */
		const Color &globalAmbientColor(void) const;

		/** Sets the globe ambient color. */
		void setGlobalAmbientColor(const Color &aColor);

		/** Gets global render option. */
		SRenderOption &globalRenderOption(void);
		/** Sets global render option. */
		void setGlobalRenderOption(const SRenderOption &aRO);

		/** Adds a sub scene. */
		void addSubScene(SSubScene *aSubScene);

		/** Removes a sub scene. */
		void removeSubScene(SSubScene *aSubScene);

		/** Gets SubScenes iterator by type. */
		ConstTypedSubSceneIterator getConstSubSceneIterator(int aType) const;

		/** Get total SubScenes iterator. */
		ConstTotalSubSceneIterator getConstSubSceneIterator(void) const;

		/** Sets current frame. Overrides from SScene. 
			@remarks Sets all sub-scenes' current frame.
		*/
		void setCurrentFrame(size_t aFrame);

		/** Step up or down current frame Overrides from SScene.
			@remarks
				If the current frame is out of range,
				it will be intercepted into 0 - m_iFrameNum.
				Step frame for all sub-scenes too.
		*/
		void stepFrame(int aSteps);

		/** Update all SceneNode's from Animation by current frame. Overrides from SScene.
			@par
				Renderer will call this method before rendering.
				So you need not call it.
				Step frames.
				For visible sub_scenes too.
		*/
		void _updateFromAnimation(void);

	}; //#### end class SSceneManager

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSCENEMANAGER_H__