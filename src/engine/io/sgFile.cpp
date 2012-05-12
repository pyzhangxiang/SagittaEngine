//////////////////////////////////////////////////////
// file: sgFile.cpp @ 2008-8-18 by Zhang Xiang
// defines of the class sgFile
// sgFile is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgFile.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/19/2008 zhangxiang]
	sgFile::sgFile(void){

	}

	//  [8/19/2008 zhangxiang]
	sgFile::sgFile(const StdString &name, std::ios_base::openmode om/* = std::ios_base::in | std::ios_base::out */)
	: std::fstream(name.c_str(), om){

	}

	//  [8/19/2008 zhangxiang]
	sgFile::~sgFile(){
		/*if(!m_bClosed){
			try{
				close();
			}catch(...){
				;
			}
		}*/
	}

	//  [8/19/2008 zhangxiang]
	//int sgFile::openModeConversioin(OpenMode aOM) const{
	//	switch(aOM){
	//		case OM_IN:
	//			return std::ios::in;

	//		case OM_OUT:
	//			return std::ios::out;

	//		case OM_INANDOUT:
	//			return std::ios::in | std::ios::out;

	//		case OM_BINARY:
	//			return std::ios::binary;

	//		default:
	//			return 0;
	//	}
	//}

	//  [8/19/2008 zhangxiang]
	//bool sgFile::open(const StdString &aName, OpenMode aOM){
	//	m_sgFileName = aName;
	//	m_OpenMode = aOM;
	//	
	//	m_fstream.open(m_sgFileName.c_str(), (std::ios_base::openmode)openModeConv(m_OpenMode));

	//	if(!m_fstream){
	//		return false;
	//	}

	//	return true;
	//}

	////  [8/19/2008 zhangxiang]
	//void sgFile::close(){
	//	if(!m_bClosed){
	//		m_fstream.close();
	//		m_bClosed = true;
	//	}
	//}

	//  [8/19/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	void sgFile::getLine(std::vector<StdString> *outWords, const StdString &sprs /* = " //" */){
	/*	if(m_fstream){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"No file available",
							"sgFile::getLine");
		}*/
		char buf[256];
		while(std::fstream::getline(buf, 256)){
			// check if need output
			if(!outWords){
				break;
			}

			outWords->clear();
			StdString word;
			word.clear();
			char *cp = buf;
			while(*cp != '\n' && *cp != '\r'){
				if(sprs.find(*cp) == StdString::npos){
					word += *cp;
				}else{
					if(!word.empty()){
						outWords->push_back(word);
						word.clear();
					}
				}

				++cp;
			}

			break;
		}
	}

} // namespace Sagitta