//  [11/5/2012 fabiozhang]

#ifndef __SGLOGSYSTEM_H__
#define __SGLOGSYSTEM_H__

#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"
#include "Singleton.h"
#include "sgStlAllocator.h"
#include <vector>
#include <list>
#include <string>
#include <fstream>

namespace Sagitta
{
	typedef void(*LogHandler)(int, const std::string&, const std::string&);

	//void sgDefaultLogHandler(int type, const std::string &category, const std::string &content);
    
    class _SG_KernelExport sgLogSystem : public sgManuallySingleton<sgLogSystem>
    {
	public:
		enum LogType
		{
			LT_INFO,
			LT_WARNING,
			LT_ERROR,
		};
	private:
		struct LogPiece
		{
			LogType type;
			std::string category;
			std::string content;
		};
        typedef sg_list(LogPiece) LogPieceList;
		LogPieceList mLogPieceList;

		typedef sg_vector(LogHandler) HandlerList;
		HandlerList mHandlerList;

		Float32 mAccumulateTime;
		static std::ofstream msLogFile;

		static const size_t HandleLogPerFrame;
		static const size_t PiecesMaxNum;
		static const std::string LogFilename;
		static const Float32 FlushFileTimeout;

		static void internalFileLogHandler(int type, const std::string &category, const std::string &content);
        
    public:
        sgLogSystem(void);
        ~sgLogSystem(void);

		void addLogHandler(LogHandler handler);
		void removeLogHandler(LogHandler handler);

		void update(Float32 deltaTime);
		void log(LogType type, const std::string &category, const std::string &content);

		void info(const std::string &category, const std::string &content);
		void warning(const std::string &category, const std::string &content);
		void error(const std::string &category, const std::string &content);

		// default category
		void info(const std::string &content);
		void warning(const std::string &content);
		void error(const std::string &content);

		

		
	};


}
#endif  // __SGLOGSYSTEM_H__

