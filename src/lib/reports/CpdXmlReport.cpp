#include "CpdXmlReport.hpp"

#include "index/EsaView.hpp"
#include "index/SourceIndex.hpp"
#include "index/LcpInterval.hpp"

#include <rapidxml_print.hpp>

#include <limits> // numeric_limits
#include <algorithm> // min

CpdXmlReport::CpdXmlReport(EsaView const& esaView, std::ostream& out)
    : _esaView(esaView)
    , _out(out)
    , _docRoot(0)
{
    _docRoot = allocate_node("pmd-cpd");
    _doc.append_node(_docRoot);
}

void CpdXmlReport::observe(LcpInterval const& interval) {
    auto const& sidx = _esaView.sourceIndex();
    auto const& sa = _esaView.suffixArray();

    size_t lines = std::numeric_limits<size_t>::max();
    size_t tokens = interval.lcp;

    auto dupNode = allocate_node("duplication");
    _docRoot->append_node(dupNode);

    for (size_t i = interval.leftBound; i <= interval.rightBound; ++i) {
        size_t globalOffset = sa[i];
        SourceFile const& file = sidx.fileAtOffset(globalOffset);
        size_t fileOffset = file.globalToFileOffset(globalOffset);

        size_t startLine = file.lineNumberOfIndex(fileOffset);
        size_t endLine   = file.lineNumberOfIndex(fileOffset + interval.lcp - 1);
        size_t lineCount = endLine - startLine;

        lines = std::min(lineCount, lines);

        auto fileNode = allocate_node("file");
        add_attribute(fileNode, "path", file.name);
        add_attribute(fileNode, "line", startLine);
        dupNode->append_node(fileNode);
    }

    add_attribute(dupNode, "lines", lines);
    add_attribute(dupNode, "tokens", tokens);
}

void CpdXmlReport::finalize() {
    rapidxml::print(_out, _doc, 0);
}
