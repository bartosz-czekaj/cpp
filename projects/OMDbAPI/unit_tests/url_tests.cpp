#include "stdafx.h"
#include "url_mock.h"
BOOST_AUTO_TEST_SUITE(url_basics)
BOOST_AUTO_TEST_CASE(url_by_tile_has_content)
{
	URL	url;
	auto urlRequest = *(url.urlMaker("batman", {}));

	BOOST_CHECK(urlRequest.length() > 0);
}

BOOST_AUTO_TEST_CASE(url_by_umbdid_has_content)
{
	URL	url;
	auto urlRequest = *(url.urlMaker("tt0903624", {}));

	BOOST_CHECK(urlRequest.length() > 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(url_correctnes)
BOOST_AUTO_TEST_CASE(url_returns_none_when_main_param_empty)
{
	URL	url;

	BOOST_CHECK_EQUAL(url.urlMaker("", {}), boost::none);
}


BOOST_AUTO_TEST_CASE(url_has_proper_page_address)
{
	URL	url;

	const boost::regex expression("^(http:\/\/){0,1}((www\.omdbapi\.com\/){1})?.*$");

	if (boost::regex_match(*url.urlMaker("batman", {}), expression) == false)
	{
		BOOST_ERROR("Invalid url, by title");
	}

	if (boost::regex_match(*url.urlMaker("tt0903624", {}), expression) == false)
	{
		BOOST_ERROR("Invalid url, by imdbId");
	}
}

BOOST_AUTO_TEST_CASE(url_contains_proper_imdbID)
{
	URL	url;

	const boost::regex expression("^.*(i=tt[0-9]*).*$");

	if (boost::regex_match(*url.urlMaker("tt0903624", {}), expression) == false)
	{
		BOOST_ERROR("Invalid imdbID");
	}
}

BOOST_AUTO_TEST_CASE(url_contains_proper_plot_param)
{
	URL	url;
	commonElements elems;
	const boost::regex expression("^.*(plot=short|full){1}.*$");

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
	elems.plot = plotType::getFull;

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
}

BOOST_AUTO_TEST_CASE(url_contains_proper_return_param)
{
	URL	url;
	commonElements elems;
	const boost::regex expression("^.*(r=json|xml){1}.*$");

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
	elems.response = responseType::xml;

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
}

BOOST_AUTO_TEST_CASE(url_contains_proper_year)
{
	URL	url;
	commonElements elems;
	const boost::regex expression("^.*(r=json|xml){1}.*$");

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
	elems.response = responseType::xml;

	BOOST_CHECK(boost::regex_match(*url.urlMaker("tt0903624", elems), expression));
}

BOOST_AUTO_TEST_CASE(test_ulr_mock) 
{
	//using ::testing::Return;

	//MockURL mURL;
	//EXPECT_CALL(mURL, urlMaker(std::string(""), {}));// .Times(1).WillOnce(Return(boost::none));
	//EXPECT_CALL(mholder, SetProperty(std::string("test2"), 555));
}

BOOST_AUTO_TEST_SUITE_END()