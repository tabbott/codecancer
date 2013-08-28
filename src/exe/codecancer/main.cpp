#include "dupscan/CloneHunter.hpp"
#include "dupscan/SourceIndex.hpp"
#include "dupscan/SourceFileList.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Give source file list and min region size!\n";
        return 1;
    }

    size_t minRegion = atoi(argv[2]);

    SourceFileList files = SourceFileList::fromFile(argv[1]);
    cout << "Found " << files.nonEmptyFiles().size() << " files, "
        << files.totalSourceBytes() << " bytes.\n";

    SourceIndex sidx;
    sidx.addSources(files);

    CloneHunter<uint32_t> hunter(sidx, minRegion);
    hunter.goHunting();
    hunter.debugPrintDuplicates();

    return 0;
}
