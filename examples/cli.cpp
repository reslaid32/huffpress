
#include <huffpress/huffpress.h>
#include <huffpress/cli/cli.h>

int main(int argc, char* argv[]) {
    Huffpress::HuffpressCLI cli;
    cli.run(argc, argv);
    return 0;
}