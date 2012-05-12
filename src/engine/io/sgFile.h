//////////////////////////////////////////////////////
// file: sgFile.h @ 2008-8-18 by Zhang Xiang
// declares of the class sgFile
// sgFile is a class ...
//////////////////////////////////////////////////////
#ifndef __SGFILE_H__
#define __SGFILE_H__

// INCLUDES //////////////////////////////////////////
//#include "../../common/sgObject.h"
#include "../../common/configs/config.h"
#include <fstream>
#include <string>
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgFile : public std::fstream{// : public sgObject, public std::fstream{
	// enum declares
	public:
		//enum OpenMode{
		//	OM_IN = 1,
		//	OM_OUT,
		//	OM_INANDOUT,
		//	OM_BINARY
		//};

	// member variables
	protected:
//		std::fstream m_fstream;

		StdString m_sgFileName;

//		OpenMode m_OpenMode;

//		bool m_bClosed;

	// constructors & destructor
	public:
		sgFile(void);
		/** Open a named file.
			@remarks 
				for vs' slt, there's a third parameter: int aProtection = (int)std::ios_base::_Openprot
		*/
		sgFile(const StdString &name, std::ios_base::openmode om = std::ios_base::in | std::ios_base::out);
		virtual ~sgFile();

	// member functions
	protected:
//		int openModeConv(OpenMode aOM) const;
	public:
		/** Get line from the current pos.
			@param
				outWords Destination for reading, the buffer is seperated by spr to be words.
						If the value is 0, just eat the line.
		*/
		void getLine(std::vector<StdString> *outWords, const StdString &sprs = " //");

		/** Open a named file. */
	//	bool open(const StdString &aName, OpenMode aOM);

		/** Close file. */
	//	void close(void);

	}; //#### end class sgFile

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGFILE_H__
