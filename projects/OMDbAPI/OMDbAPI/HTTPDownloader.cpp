#include "stdafx.h"
#include <sstream>
#include "HTTPDownloader.h"

namespace
{
	size_t write_data(void *data, size_t size, size_t nmemb, void *stream)
	{
		if (const char* strData = reinterpret_cast<const char*>(data))
		{
			std::string data(strData, static_cast<size_t>(size * nmemb));

			if (std::stringstream* outStream = reinterpret_cast<std::stringstream*>(stream))
			{
				*outStream << data;
			}
		}

		return size * nmemb;
	}
};

HTTPDownloader::HTTPDownloader()
	:mCurl(curl_easy_init())
{
}


HTTPDownloader::~HTTPDownloader()
{
	curl_easy_cleanup(mCurl);
}

std::string HTTPDownloader::GetData(const std::string &url)
{
	std::string data;

	if (mCurl)
	{
		curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(mCurl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
		curl_easy_setopt(mCurl, CURLOPT_ACCEPT_ENCODING, "deflate");

		std::stringstream out;
		out.str("");

		curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &out);
		
		CURLcode CURLResCode =curl_easy_perform(mCurl);

		if (CURLResCode == CURLE_OK)
		{
			data = out.str();
		}

		return data;
	}
}
