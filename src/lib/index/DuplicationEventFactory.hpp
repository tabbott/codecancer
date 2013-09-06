#pragma once

#include "DuplicationEvent.hpp"
#include "LcpInterval.hpp"
#include "SourceFile.hpp"
#include "SourceIndex.hpp"

template<typename EsaType>
class DuplicationEventFactory {
public:
    DuplicationEventFactory(EsaType const& esa)
        : _esa(esa)
    {
    }

    DuplicationEvent create(LcpInterval const& interval) const {
        DuplicationEvent event{interval.lcp, {}};
        auto const& sidx = _esa.sourceIndex();
        auto const& sa = _esa.suffixArray();

        for (size_t i = interval.leftBound; i <= interval.rightBound; ++i) {
            size_t globalOffset = sa[i];
            SourceFile const& file = sidx.fileAtOffset(globalOffset);
            size_t fileOffset = file.globalToFileOffset(globalOffset);

            size_t firstLine = file.lineNumberOfIndex(fileOffset);
            size_t lastLine   = file.lineNumberOfIndex(fileOffset + interval.lcp - 1);
            event.regions.emplace_back(
                DuplicationEvent::Region{file.name, firstLine, lastLine});
        }
        return event;
    }

private:
    EsaType const& _esa;
};
