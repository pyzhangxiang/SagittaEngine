//////////////////////////////////////////////////////
// file: sgException.cpp @ 2008-9-12 by Zhang Xiang
// defines of the class sgException
// sgException is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgException.h"
#include <sstream>

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	/// sgException //////////////////////////////////////////////////////////////////////////
	//  [9/12/2008 zhangxiang]
	sgException::sgException(int aCode, const std::string &aDescription, const std::string &aSourceFunc) :
		m_iLine(0), m_iCode(aCode), m_sDescription(aDescription), m_sSourceFunc(aSourceFunc){
	}
	
	//  [9/12/2008 zhangxiang]
	sgException::sgException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aTypeName, const char *aFileName, long aLine) :
		m_iLine(aLine),
		m_iCode(aCode),
		m_sTypeName(aTypeName),
		m_sDescription(aDescription),
		m_sSourceFunc(aSourceFunc),
		m_sFileName(aFileName){

	}

	//  [9/12/2008 zhangxiang]
	sgException::sgException(const sgException &aException) :
		m_iLine(aException.m_iLine),
		m_iCode(aException.m_iCode),
		m_sTypeName(aException.m_sTypeName),
		m_sDescription(aException.m_sDescription),
		m_sSourceFunc(aException.m_sSourceFunc),
		m_sFileName(aException.m_sFileName){

	}

	//  [9/12/2008 zhangxiang]
	void sgException::operator =(const sgException &aException){
		m_iLine = aException.m_iLine;
		m_iCode = aException.m_iCode;
		m_sTypeName = aException.m_sTypeName;
		m_sDescription = aException.m_sDescription;
		m_sSourceFunc = aException.m_sSourceFunc;
		m_sFileName = aException.m_sFileName;
	}

	//  [9/12/2008 zhangxiang]
	const std::string &sgException::fullDescription(void) const{
		if(m_sFullDesc.empty()){
			std::stringstream ss;
			ss<<"SAGITTA_EXCEPTION("<<m_iCode<<": "<<m_sTypeName<<"): "
				<<m_sDescription<<" in "<<m_sSourceFunc;

			if(m_iLine > 0){
				ss<<" at "<<m_sFileName<<" (line "<<m_iLine<<")";
			}

			m_sFullDesc = ss.str();
		}

		return m_sFullDesc;
	}

} // namespace Sagitta