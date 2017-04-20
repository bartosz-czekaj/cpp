// unit_tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <curl.h>


BOOST_AUTO_TEST_CASE(OMDbApi_sends_any_response)
{
	URL	a;
	//OMDbApi a;
}
/*
{
size_t year = 0;
responseType response = responseType::json;
searchedType type = searchedType::any;
plotType plot = plotType::getShort;

};
*/

//BOOST_AUTO_TEST_CASE(OMDbApi_sends_any_response)
//{
//
//	byTitle options{ "batman", {1999, responseType::json, searchedType::any, plotType::getShort } };
//
//	OMDBApi api;
//
//	auto serverResponse = api.searchByTitle(options);
//
//	BOOST_CHECK(serverResponse.length() > 0);
//}

/*BOOST_AUTO_TEST_CASE(OMDbApi_returns_json)
{
	std::string title = "batman";
	OMDBApi api;

	auto serverResponse = api.searchByTitle(title);

	BOOST_CHECK(serverResponse.length() > 0);
}*/
