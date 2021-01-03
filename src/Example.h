#pragma once

#include <vector>

#include "Feature.h"

class Example
{
public:
	template<class Iterator>
	Example(Iterator first, Iterator last);
	Example() = default;

	const Feature& at(size_t featureIndex) const;
	size_t size() const;
	bool empty() const;

private:
	const std::vector<Feature> features;
};

template<class Iterator>
inline Example::Example(Iterator first, Iterator last) : features(first, last)
{
}
