#include "Example.h"

const Feature& Example::at(size_t featureIndex) const
{
    return features.at(featureIndex);
}

size_t Example::size() const
{
    return features.size();
}

bool Example::empty() const
{
    return features.empty();
}
