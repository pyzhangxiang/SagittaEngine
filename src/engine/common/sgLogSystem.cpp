
#include "sgLogSystem.h"

namespace Sagitta
{
	const size_t sgLogSystem::HandleLogPerFrame = 10;
	const size_t sgLogSystem::PiecesMaxNum = 1024;
	const std::string sgLogSystem::LogFilename = "sagitta_internal_log.txt";
	const Float32 sgLogSystem::FlushFileTimeout = 30.0f;
	std::ofstream sgLogSystem::msLogFile;

	sgLogSystem::sgLogSystem( void )
		: mAccumulateTime(0.0f)
	{
		addLogHandler(&(sgLogSystem::internalFileLogHandler));
	}

	sgLogSystem::~sgLogSystem( void )
	{

	}

	void sgLogSystem::addLogHandler( LogHandler handler )
	{
		if(!handler)
			return ;

		mHandlerList.push_back(handler);
	}

	void sgLogSystem::removeLogHandler( LogHandler handler )
	{
		if(!handler)
			return ;
		
		HandlerList::iterator it = mHandlerList.begin();
		for(; it!=mHandlerList.end(); )
		{
			if(*it == handler)
				it = mHandlerList.erase(it);
			else
				++it;
		}
	}

	void sgLogSystem::update( Float32 deltaTime )
	{
		if(mHandlerList.empty())
			return ;

		int count = 0;
		while(!mLogPieceList.empty() && count < HandleLogPerFrame)
		{
			LogPiece &lp = mLogPieceList.front();
			for(size_t i=0; i<mHandlerList.size(); ++i)
			{
				mHandlerList[i](lp.type, lp.category, lp.content);
			}
			mLogPieceList.pop_front();
			++count;
		}

		mAccumulateTime += deltaTime;
		if(mAccumulateTime >= FlushFileTimeout && msLogFile.good())
		{
			mAccumulateTime = 0.0f;
			msLogFile.flush();
		}
	}
    
	void sgLogSystem::log( LogType type, const std::string &category, const std::string &content )
	{
		LogPiece lp;
		lp.type = type;
		lp.category = category;
		lp.content = content;
		mLogPieceList.push_back(lp);
		if(mLogPieceList.size() > PiecesMaxNum)
		{
			mLogPieceList.pop_front();
		}
	}

	void sgLogSystem::info( const std::string &category, const std::string &content )
	{
		log(LT_INFO, category, content);
	}

	void sgLogSystem::info( const std::string &content )
	{
		info("default", content);
	}
	void sgLogSystem::warning( const std::string &category, const std::string &content )
	{
		log(LT_WARNING, category, content);
	}

	void sgLogSystem::warning( const std::string &content )
	{
		warning("default", content);
	}
	void sgLogSystem::error( const std::string &category, const std::string &content )
	{
		log(LT_ERROR, category, content);
	}

	void sgLogSystem::error( const std::string &content )
	{
		error("default", content);
	}

	void sgLogSystem::internalFileLogHandler( int type, const std::string &category, const std::string &content )
	{
		if(!msLogFile.is_open())
		{
			msLogFile.open(LogFilename.c_str());
		}

		if(!msLogFile.good())
			return ;

		std::string strType = "";
		if(type == sgLogSystem::LT_INFO)
			strType = "Info";
		else if(type == sgLogSystem::LT_WARNING)
			strType = "Warning";
		else if(type == sgLogSystem::LT_ERROR)
			strType = "Error";

		msLogFile << "<" << category << "> [" << strType << "] -- " << content << "\n";
	}
}
