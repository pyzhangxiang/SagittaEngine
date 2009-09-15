//////////////////////////////////////////////////////
// file: SResourceManager.h 
// created by zhangxiang on 09-01-11
// declare of the class SResourceManager
// SResourceManager is a class ...
//////////////////////////////////////////////////////
#ifndef __SRESOURCEMANAGER_H__
#define __SRESOURCEMANAGER_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../common/utils/Singleton.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SResource;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SResourceManager : public SObject, public Singleton<SResourceManager>{
	// type defines
	protected:
		/// key_value is resource's id
		typedef std::map<uLong, SResource*> ResourceMap;

	// member variables
	private:
		/// collection of resources mapped by id
		ResourceMap m_ResourceMap;

	// constructors & destructor
	public:
		~SResourceManager(void);

	// member functions
	public:
		/** Adds a resource. */
		void addResource(SResource *aResource);

		/** Removes a resource by id. */
		SResource *removeResource(uLong aID);

		/** Removes a resource. */
		void removeResource(SResource *aResource);

	}; //#### end class SResourceManager

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRESOURCEMANAGER_H__