#pragma once

class CLASS_EXPORT OMDBApi
{
private:
	
public:
	boost::property_tree::ptree getUrlResponse(const std::string &searchParam, const commonElements &options);
private:
	
};
