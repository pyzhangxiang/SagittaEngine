//////////////////////////////////////////////////////
// file: SObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class SObject
// SObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SOBJECT_H__
#define __SOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "configs/SagittaPlatform.h"
#include "configs/SagittaPrerequisites.h"

// DECLARES //////////////////////////////////////////
namespace Sagitta{

	class _CommonExport SObject{
	// static members
		static uLong ms_oid;

	// member variables
	private:
		/// my id
		uLong m_iOId;

		/** if i'm disposing.
			@remarks
				Only in destructor, it will be set to be true.
		*/
		bool m_bDisposing;
	
	// constructors & destructor
	private:
		// disable compiler version copying
		SObject(const SObject &);
		SObject &operator=(const SObject &);

	public:
		SObject(void);
		virtual ~SObject(void) = 0;

	// member functions
	protected:
		/** Notifies i'm disposing.
			@remarks
				Should only be called in destructor.
		*/
		void _setDisposing(void);

	public:
		/** Check if i'm disposing, that mean i'm in destructor. */
		bool isDisposing(void) const;

		/** Gets this object id. */
		uLong id(void) const;
	};

} // namespace Sagitta

#endif // __SOBJECT_H__