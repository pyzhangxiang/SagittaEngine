//////////////////////////////////////////////////////
// file: SResource.h 
// created by zhangxiang on 09-01-09
// declares of the class SResource
// SResource is a class ...
//////////////////////////////////////////////////////
#ifndef __SRESOURCE_H__
#define __SRESOURCE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SResourceDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SResource : public SObject{
	// enum declares
	public:
		enum LoadOption{
			LOP_REFRESH,
			LOP_MERGE
		};

	// static members
		static uLong ms_iResourceCount;

	// member variables
	private:
		/// resource name
		StdString m_sName;

		/// resource id
		const uLong mc_iRsID;
		
		/// loader and exporter
		SResourceDelegate *m_pDelegate;

		/// if this scene is unloading
		bool m_bUnloading;

	// constructors & destructor
	public:
		SResource(SResourceDelegate *aDelegate);
		SResource(const StdString &aName, SResourceDelegate *aDelegate);
		virtual ~SResource(void) = 0;

	// member functions
	private:
		/** Internal hook does unloading. */
		virtual void unLoadImpl(void) = 0;

	protected:
		/** Gets my delegate. */
		SResourceDelegate *del(void) const;

	public:
		/** Gets this resource name. */
		const StdString &name(void) const;
		/** Sets this resource name. */
		void setName(const StdString &aName);

		/** Gets resource id. */
		uLong getRsID(void) const;

		/** Loads from file. */
		void load(const StdString &aFileName, int aLoadOption = LOP_MERGE);

		/** Unloads this resource. */
		void unLoad(void);

		/** Check this scene is unloading. */
		bool isUnloading(void) const;

	}; //#### end class SResource

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRESOURCE_H__