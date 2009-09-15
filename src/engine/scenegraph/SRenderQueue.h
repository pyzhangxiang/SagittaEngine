//////////////////////////////////////////////////////
// file: SRenderQueue.h @ 2008-2-2 by Zhang Xiang
// declares of the class SRenderQueue
// SRenderQueue is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDERQUEUE_H__
#define __SRENDERQUEUE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../common/utils/SIterator.h"
#include <list>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SRenderable;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SRenderQueue : public SObject{
	protected:
		typedef std::list<SRenderable*> RenderableList;

	public:
		typedef SVectorIterator<RenderableList> Iterator;
		typedef SConstVectorIterator<RenderableList> ConstIterator;
	// member variables
	protected:
		RenderableList m_RenderableList;

	// constructors & destructor
	public:
		SRenderQueue(void);
		~SRenderQueue(void);

	// member functions
	public:
		void clear(void);
		void addRenderable(SRenderable *aObject);
		Iterator getIterator(void);
		ConstIterator getConstIterator(void) const;
		

	}; //#### end class SRenderQueue

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDERQUEUE_H__