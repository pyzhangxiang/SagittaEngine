//////////////////////////////////////////////////////
// file: SFile.cpp @ 2008-8-18 by Zhang Xiang
// defines of the class SFile
// SFile is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SFile.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/19/2008 zhangxiang]
	SFile::SFile(void){

	}

	//  [8/19/2008 zhangxiang]
	SFile::SFile(const StdString &name, std::ios_base::openmode om/* = std::ios_base::in | std::ios_base::out */)
	: std::fstream(name.c_str(), om){

	}

	//  [8/19/2008 zhangxiang]
	SFile::~SFile(){
		/*if(!m_bClosed){
			try{
				close();
			}catch(...){
				;
			}
		}*/
	}

	//  [8/19/2008 zhangxiang]
	//int SFile::openModeConversioin(OpenMode aOM) const{
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
	//bool SFile::open(const StdString &aName, OpenMode aOM){
	//	m_sFileName = aName;
	//	m_OpenMode = aOM;
	//	
	//	m_fstream.open(m_sFileName.c_str(), (std::ios_base::openmode)openModeConv(m_OpenMode));

	//	if(!m_fstream){
	//		return false;
	//	}

	//	return true;
	//}

	////  [8/19/2008 zhangxiang]
	//void SFile::close(){
	//	if(!m_bClosed){
	//		m_fstream.close();
	//		m_bClosed = true;
	//	}
	//}

	//  [8/19/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	void SFile::getLine(std::vector<StdString> *outWords, const StdString &sprs /* = " //" */){
	/*	if(m_fstream){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
							"No file available",
							"SFile::getLine");
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