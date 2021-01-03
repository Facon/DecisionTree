#pragma once

#include <string>
#include <ostream>

class Feature
{
public:
	Feature() = default;
	Feature(const std::string& itemClass);

	const std::string_view get();

private:
	friend bool operator==(const Feature& left, const Feature& right);
	friend bool operator<(const Feature& left, const Feature& right);
	friend std::ostream& operator<<(std::ostream& out, const Feature& itemClass);

	const std::string feature;
};

bool operator==(const Feature& left, const Feature& right);
bool operator<(const Feature& left, const Feature& right);
std::ostream& operator<<(std::ostream& out, const Feature& itemClass);