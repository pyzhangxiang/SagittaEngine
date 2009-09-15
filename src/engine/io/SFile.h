//////////////////////////////////////////////////////
// file: SFile.h @ 2008-8-18 by Zhang Xiang
// declares of the class SFile
// SFile is a class ...
//////////////////////////////////////////////////////
#ifndef __SFILE_H__
#define __SFILE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
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
	class _SagittaExport SFile : public SObject, public std::fstream{
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

		StdString m_sFileName;

//		OpenMode m_OpenMode;

//		bool m_bClosed;

	// constructors & destructor
	public:
		SFile(void);
		/** Open a named file.
			@remarks 
				for vs' slt, there's a third parameter: int aProtection = (int)std::ios_base::_Openprot
		*/
		SFile(const StdString &name, std::ios_base::openmode om = std::ios_base::in | std::ios_base::out);
		virtual ~SFile();

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

	}; //#### end class SFile

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SFILE_H__