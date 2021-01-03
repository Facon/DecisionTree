#include "AdultCSVReader.h"

#include "csv.h"

constexpr auto NUMBER_OF_COLUMNS = 10u;
using CSVReader = io::CSVReader<NUMBER_OF_COLUMNS, io::trim_chars<' ', '\t'>, io::no_quote_escape<';'>>;

bool readRow(CSVReader& csvReader, std::vector<std::string>& row);

AdultCSVReader::AdultCSVReader(std::string_view filePath) : filePath(filePath)
{}

AdultCSVReader::FileContent AdultCSVReader::read()
{
	CSVReader csvReader(filePath);

	FileContent fileContent{ {"# Age", "Workclass", "Education",
		"Marital-status", "Occupation", "Relationship", "Race", "Sex", "Native-country",
		"Income"}, {} };

	const auto& headers = fileContent.headers;

	csvReader.read_header(io::ignore_extra_column, headers[0], headers[1], headers[2], headers[3], 
		headers[4], headers[5], headers[6], headers[7], headers[8], headers[9]);

	std::vector<std::string> row(NUMBER_OF_COLUMNS);

	while (readRow(csvReader, row))
	{
		fileContent.data.emplace_back(row);
	}

	return fileContent;
}

bool readRow(CSVReader& csvReader, std::vector<std::string>& row)
{
	return csvReader.read_row(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], 
		row[9]);
}
