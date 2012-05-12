//////////////////////////////////////////////////////
// file: sgResource.h 
// created by zhangxiang on 09-01-09
// declares of the class sgResource
// sgResource is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRESOURCE_H__
#define __SGRESOURCE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgResourceDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgResource : public sgObject{
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
		sgResourceDelegate *m_pDelegate;

		/// if this scene is unloading
		bool m_bUnloading;

	// constructors & destructor
	public:
		sgResource(sgResourceDelegate *aDelegate);
		sgResource(const StdString &aName, sgResourceDelegate *aDelegate);
		virtual ~sgResource(void) = 0;

	// member functions
	private:
		/** Internal hook does unloading. */
		virtual void unLoadImpl(void) = 0;

	protected:
		/** Gets my delegate. */
		sgResourceDelegate *del(void) const;

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

	}; //#### end class sgResource

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRESOURCE_H__
