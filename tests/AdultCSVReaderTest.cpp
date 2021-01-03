#include <doctest/doctest.h>

#include "AdultCSVReader.h"

TEST_CASE("Read adult.csv")
{
	AdultCSVReader reader("adult.csv");

	auto fileContent = reader.read();

	CHECK(fileContent.data.size() == 30162);
}