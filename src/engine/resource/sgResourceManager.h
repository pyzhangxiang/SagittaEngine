//////////////////////////////////////////////////////
// file: sgResourceManager.h 
// created by zhangxiang on 09-01-11
// declare of the class sgResourceManager
// sgResourceManager is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRESOURCEMANAGER_H__
#define __SGRESOURCEMANAGER_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgObject.h"
#include "../../common/utils/Singleton.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgResource;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgResourceManager : public sgObject, public Singleton<sgResourceManager>{
	// type defines
	protected:
		/// key_value is resource's id
		typedef std::map<uLong, sgResource*> ResourceMap;

	// member variables
	private:
		/// collection of resources mapped by id
		ResourceMap m_ResourceMap;

	// constructors & destructor
	public:
		~sgResourceManager(void);

	// member functions
	public:
		/** Adds a resource. */
		void addResource(sgResource *aResource);

		/** Removes a resource by id. */
		sgResource *removeResource(uLong aID);

		/** Removes a resource. */
		void removeResource(sgResource *aResource);

	}; //#### end class sgResourceManager

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRESOURCEMANAGER_H__
