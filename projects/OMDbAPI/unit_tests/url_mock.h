
class MockURL : public URL {
public:
	MockURL() { }
	virtual ~MockURL() { }

	MOCK_METHOD2(urlMaker, boost::optional<std::string>(const std::string &searchParam, const commonElements &options));
};