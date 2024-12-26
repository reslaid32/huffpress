#include <huffpress/cli/cli.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    Huffpress::HuffpressCLI cli;
    cli.runCombine(argc, argv);
    return 0;
}