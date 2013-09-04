#pragma once

#include <istream>
#include <memory>
#include <string>
#include <vector>

class SourceIndex;
class SourceFile;

class SourceIndexBuilder {
public:
    void addSource(std::string const& name, std::istream& src);
    std::shared_ptr<SourceIndex> build();

private:
    std::string _text;
    std::vector<SourceFile> _sourceFiles;
};
