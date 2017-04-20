#pragma once
class HTTPDownloader
{
public:
	HTTPDownloader();
	~HTTPDownloader();

	std::string GetData(const std::string &url);

private:
	CURL *mCurl;
};

