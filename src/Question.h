#pragma once

#include <vector>
#include <ostream>

class Feature;
class Example;

class Question
{
public:
    static void setHeaders(const std::vector<std::string>& headers);

    Question(size_t featureIndex, const Feature& feature);

    bool match(const Example& example) const;

private:
    friend std::ostream& operator<< (std::ostream& out, const Question& question);
    friend bool operator==(const Question& left, const Question& right);
    friend bool operator<(const Question& left, const Question& right);
    
    static std::vector<std::string> headers;

    const size_t featureIndex;
    const Feature& feature;
};

bool operator==(const Question& left, const Question& right);
bool operator<(const Question& left, const Question& right);
std::ostream& operator<<(std::ostream& out, const Question& question);