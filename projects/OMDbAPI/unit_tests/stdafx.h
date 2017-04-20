// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "Common.h"
#include <stdio.h>
#include <tchar.h>
#include <functional>
#include <string>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "gmock/gmock.h"
#include "OMDbApi.h"
#include "URL.h"


// TODO: reference additional headers your program requires here
