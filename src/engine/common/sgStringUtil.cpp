
#include "sgStringUtil.h"
#include "strtk/strtk.hpp"

namespace Sagitta
{

	void sgStringUtil::split(std::vector<std::string> &outwords,
                             const std::string &str,
                             const std::string &delimiter)
    {
		outwords.clear();

        strtk::std_string::token_list_type token_list;
        strtk::split(delimiter, str, std::back_inserter(token_list));
        
        strtk::std_string::token_list_type::iterator itr = token_list.begin();
        while(itr != token_list.end())
        {
            outwords.push_back((*itr).second);
            ++itr;
        }
    }
    
    bool sgStringUtil::start_with(const std::string &str, const std::string &start)
    {
        size_t i = str.find_first_of(start);
        if(i == 0)
            return true;
        else
            return false;
    }
    
    bool sgStringUtil::end_with(const std::string &str, const std::string &end)
    {
        size_t i = str.rfind(end);
        if(i == str.size() - end.size())
            return true;
        else
            return false;
    }

	std::string sgStringUtil::getFileExtention( const std::string &filename )
	{
		size_t dotIndex = filename.find_last_of(".");
		if(dotIndex == std::string::npos)
			return "";
		return filename.substr(dotIndex);
	}

} // namespace Sagitta

