//////////////////////////////////////////////////////
// file: sgSceneManager.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgSceneManager
// sgSceneManager is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSCENEMANAGER_H__
#define __SGSCENEMANAGER_H__

// INCLUDES //////////////////////////////////////////
#include "sgScene.h"
#include "../renderer/sgRenderOption.h"
#include "../../math/sgColor.h"
#include "../../common/utils/sgIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgSubScene;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSceneManager : public sgScene{
	// type defines
	protected:
		typedef std::map<SSceneJoint*, sgSubScene*> SubSceneMap;
		typedef std::map<int, SubSceneMap*> TypedSubSceneMap;

	public:
		typedef sgConstMapIterator<SubSceneMap> ConstTypedSubSceneIterator;
		typedef sgConstCMPMapIterator<TypedSubSceneMap, SubSceneMap> ConstTotalSubSceneIterator;

	// member variables
	protected:
		/// the global ambient color
		Color m_GLobalAmbientColor;

		/// global render option
		sgRenderOption m_GlobalRenderOption;

		int m_iCurrentFrame;

		/// mapped by type
		TypedSubSceneMap m_SubSceneMap;

	// constructors & destructor
	public:
		sgSceneManager(void);
		sgSceneManager(const StdString &aName);
		virtual ~sgSceneManager(void);

	// member functions
	public:
		/** Gets the globe ambient color. */
		const Color &globalAmbientColor(void) const;

		/** Sets the globe ambient color. */
		void setGlobalAmbientColor(const Color &aColor);

		/** Gets global render option. */
		sgRenderOption &globalRenderOption(void);
		/** Sets global render option. */
		void setGlobalRenderOption(const sgRenderOption &aRO);

		/** Adds a sub scene. */
		void addSubScene(sgSubScene *aSubScene);

		/** Removes a sub scene. */
		void removeSubScene(sgSubScene *aSubScene);

		/** Gets SubScenes iterator by type. */
		ConstTypedSubSceneIterator getConstSubSceneIterator(int aType) const;

		/** Get total SubScenes iterator. */
		ConstTotalSubSceneIterator getConstSubSceneIterator(void) const;

		/** Sets current frame. Overrides from sgScene. 
			@remarks Sets all sub-scenes' current frame.
		*/
		void setCurrentFrame(size_t aFrame);

		/** Step up or down current frame Overrides from sgScene.
			@remarks
				If the current frame is out of range,
				it will be intercepted into 0 - m_iFrameNum.
				Step frame for all sub-scenes too.
		*/
		void stepFrame(int aSteps);

		/** Update all SceneNode's from Animation by current frame. Overrides from sgScene.
			@par
				Renderer will call this method before rendering.
				So you need not call it.
				Step frames.
				For visible sub_scenes too.
		*/
		void _updateFromAnimation(void);

	}; //#### end class sgSceneManager

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENEMANAGER_H__

