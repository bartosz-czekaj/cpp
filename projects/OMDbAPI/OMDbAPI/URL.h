#pragma once
class CLASS_EXPORT URL
{
	static std::string mOMDdApiURL;
	enum class optionType
	{
		title,
		id
	};
public:
	boost::optional<std::string> urlMaker(const std::string &searchParam, const commonElements &options);
private:
	boost::optional<std::string> createUrl(const std::string &mainElement, const commonElements &options, optionType otype);
};

