#include "uci.h"

int tui_main() {
    UCI::uci_loop();
    return 0;
}

int main() {
    return tui_main();
}
