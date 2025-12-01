#include "stopwatch.h"
#include "flags.h"
#include "options.h"
#include "gatherer.h"
#include "finder.h"
#include "viewer.h"
#include "pipe.h"

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, nullptr);

    options options(argc, argv);
    options.parse();

    stopwatch sw;
    gatherer gather_files;
    finder find_duplicates;
    viewer report;

    options
        | gather_files
        | find_duplicates
        | report;

    if (flags::get().is_show_stats()) {
        std::cout << "Elapsed time: " << sw << std::endl;
    }

    return EXIT_SUCCESS;
}
