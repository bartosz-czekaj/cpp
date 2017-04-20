// OMDbAPI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <sstream>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "URL.h"
#include "HTTPDownloader.h"
#include "OMDbAPI.h"


boost::property_tree::ptree OMDBApi::getUrlResponse(const std::string &searchParam, const commonElements &options)
{
	URL url;
	HTTPDownloader http;
	std::stringstream stream;

	auto urlAddress = *(url.urlMaker(searchParam, options));

	stream <<  http.GetData(urlAddress);

	boost::property_tree::ptree propertyTree;
	try
	{
		if (options.response == responseType::xml)
		{
			boost::property_tree::read_xml(stream, propertyTree);
		}
		else
		{
			boost::property_tree::read_json(stream, propertyTree);
		}
	}
	catch (std::exception & e)
	{
		throw;
	}
	return propertyTree;
}
