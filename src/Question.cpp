#include "Question.h"

#include "Example.h"

std::vector<std::string> Question::headers;

void Question::setHeaders(const std::vector<std::string>& headers)
{
    Question::headers = headers;
}

Question::Question(size_t featureIndex, const Feature& feature) : featureIndex(featureIndex), feature(feature)
{}

bool Question::match(const Example& example) const
{
    const auto& value = example.at(featureIndex);
    
    // No numeric data checking here, all categorical! :)

    return value == this->feature;
}

bool operator==(const Question& left, const Question& right)
{
    return left.featureIndex == right.featureIndex && left.feature == right.feature;
}

bool operator<(const Question& left, const Question& right)
{
    return left.featureIndex < right.featureIndex && left.feature < right.feature;
}

std::ostream& operator<<(std::ostream& out, const Question& question)
{
    using namespace std::literals;

    out << "Is "sv << Question::headers.at(question.featureIndex) 
        << " == "sv << question.feature << "?"sv;

    return out;
}