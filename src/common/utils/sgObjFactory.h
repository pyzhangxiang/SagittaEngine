//////////////////////////////////////////////////////
// file: sgObjFactory.h 
// created by zhangxiang on 09-10-4
// declares of the class sgObjFactory
// sgObjFactory is a class ...
//////////////////////////////////////////////////////
#ifndef __SGOBJFACTORY_H__
#define __SGOBJFACTORY_H__

// INCLUDES //////////////////////////////////////////

// DECLARES //////////////////////////////////////////

namespace Sagitta{
	
	/** class representation
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	template<
		class AP,	// abstract product
		class IT,	// identifier type
		class PC = AP *(*)(),	// product creator
	>
	class sgObjFactory{
	// member variables
		
	// constructors & destructor
		
	// member functions
		
	}; //#### end class sgObjFactory
	
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGOBJFACTORY_H__


