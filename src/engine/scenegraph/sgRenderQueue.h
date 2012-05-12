//////////////////////////////////////////////////////
// file: sgRenderQueue.h @ 2008-2-2 by Zhang Xiang
// declares of the class sgRenderQueue
// sgRenderQueue is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRENDERQUEUE_H__
#define __SGRENDERQUEUE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgObject.h"
#include "../../common/utils/sgIterator.h"
#include <list>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgRenderable;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgRenderQueue : public sgObject{
	protected:
		typedef std::list<sgRenderable*> RenderableList;

	public:
		typedef sgVectorIterator<RenderableList> Iterator;
		typedef sgConstVectorIterator<RenderableList> ConstIterator;
	// member variables
	protected:
		RenderableList m_RenderableList;

	// constructors & destructor
	public:
		sgRenderQueue(void);
		~sgRenderQueue(void);

	// member functions
	public:
		void clear(void);
		void addRenderable(sgRenderable *aObject);
		Iterator getIterator(void);
		ConstIterator getConstIterator(void) const;
		

	}; //#### end class sgRenderQueue

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRENDERQUEUE_H__

