//////////////////////////////////////////////////////
// file: SObjFactory.h 
// created by zhangxiang on 09-10-4
// declares of the class SObjFactory
// SObjFactory is a class ...
//////////////////////////////////////////////////////
#ifndef __SOBJFACTORY_H__
#define __SOBJFACTORY_H__

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
	class SObjFactory{
	// member variables
		
	// constructors & destructor
		
	// member functions
		
	}; //#### end class SObjFactory
	
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SOBJFACTORY_H__

