#include "stdafx.h"
#include "URL.h"

std::string URL::mOMDdApiURL = "http://www.omdbapi.com";

boost::optional<std::string> URL::urlMaker(const std::string &searchParam, const commonElements &options)
{
	if (searchParam.empty())
		return boost::none;

	const boost::regex expression("^(tt[0-9]*)$");

	auto type = optionType::title;

	if (boost::regex_match(searchParam, expression))
		type = optionType::id;

	return createUrl(searchParam, options, type);
}

boost::optional<std::string> URL::createUrl(const std::string &searchParam, const commonElements &options, optionType otype)
{
	std::string url(mOMDdApiURL);

	if (otype == optionType::id)
	{
		url.append("?i=").append(searchParam);
	}
	else
	{
		url.append("?t=").append(searchParam);
	}


	url.append("&plot=").append(options.plot == plotType::getFull ? "full" : "short");

	if (options.year)
	{
		url.append("&y=").append(std::to_string(options.year));
	}
	url.append("&r=").append(options.response == responseType::json ? "json" : "xml");


	if (options.type != searchedType::any)
	{
		url.append("&type=");

		switch (options.type)
		{
		case searchedType::episode:
			url.append("episode");
			break;
		case searchedType::movie:
			url.append("movie");
			break;
		case searchedType::series:
			url.append("series");
			break;
		default:
			break;
		}
	}

	return url;
}