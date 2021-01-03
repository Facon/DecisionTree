#include "Feature.h"

Feature::Feature(const std::string& itemClass) : feature(itemClass)
{}

const std::string_view Feature::get()
{
	return feature;
}

bool operator==(const Feature& left, const Feature& right)
{
	return left.feature == right.feature;
}

bool operator<(const Feature& left, const Feature& right)
{
	return left.feature < right.feature;
}

std::ostream& operator<<(std::ostream& out, const Feature& feature)
{
	out << feature.feature;

	return out;
}
