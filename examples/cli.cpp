
#include <huffpress/huffpress.h>
#include <huffpress/cli/cli.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    Huffpress::HuffpressCLI cli;
    // cli.run("run");
    cli.run();
    return 0;
}