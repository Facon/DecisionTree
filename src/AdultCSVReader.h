#pragma once

#include <string_view>
#include <vector>
#include <optional>

class AdultCSVReader
{
public:
	struct FileContent
	{
		std::vector<std::string> headers;
		std::vector<std::vector<std::string>> data;
	};

	AdultCSVReader(std::string_view filePath);

	FileContent read();

private:
	
	std::string filePath;
};