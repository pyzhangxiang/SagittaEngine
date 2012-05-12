//////////////////////////////////////////////////////
// file: sgObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class sgObject
// sgObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SGOBJECT_H__
#define __SGOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "configs/SagittaPlatform.h"
#include "configs/SagittaPrerequisites.h"

// DECLARES //////////////////////////////////////////
namespace Sagitta{

	class _SG_CommonExport sgObject{
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
		sgObject(const sgObject &);
		sgObject &operator=(const sgObject &);

	public:
		sgObject(void);
		virtual ~sgObject(void) = 0;

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
/*
	// memory management
	public:
		/// operator new, with debug line info
		static void *operator new(size_t sz, const char *file, int line, const char *func);
		
		static void *operator new(size_t sz);
		
		/// placement operator new
		static void *operator new (size_t sz, void *ptr);

		/// array operator new, with debug line info
		static void *operator new[](size_t sz, const char *file, int line, const char *func);

		static void *operator new[](size_t sz);

		static void operator delete(void *ptr);

		/// corresponding operator for placement delete (second param same as the first)
		static void operator delete(void *ptr, void*);

		/// only called if there is an exception in corresponding "new"
		void operator delete(void *ptr, const char*, int, const char*);

		void operator delete[](void *ptr);

		void operator delete[](void *ptr, const char*, int, const char*);
        */
	};

} // namespace Sagitta

#endif // __SGOBJECT_H__

