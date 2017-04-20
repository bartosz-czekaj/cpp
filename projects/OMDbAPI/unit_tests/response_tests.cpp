#include "stdafx.h"


BOOST_AUTO_TEST_SUITE(response_format)

BOOST_AUTO_TEST_CASE(response_xml_json_exists)
{
	OMDBApi api;
	std::stringstream stream;
	commonElements elems;
	boost::property_tree::ptree pt = api.getUrlResponse("Dexter", elems);
	BOOST_CHECK(pt.size() != 0);
	
	elems.response = responseType::xml;
	pt = api.getUrlResponse("Dexter", elems);
	BOOST_CHECK(pt.size() != 0);
}

BOOST_AUTO_TEST_CASE(response_contains_fight_club_when_club_is_a_tittle)
{
	OMDBApi api;
	commonElements elems;

	boost::property_tree::ptree pt = api.getUrlResponse("Club", elems);
	boost::property_tree::ptree child = pt.get_child("Title");

	elems.response = responseType::xml;
	pt = api.getUrlResponse("Club", elems);

	BOOST_CHECK_EQUAL(pt.get<std::string>("root.movie.<xmlattr>.title"), "Fight Club");
	
}

BOOST_AUTO_TEST_SUITE_END()

