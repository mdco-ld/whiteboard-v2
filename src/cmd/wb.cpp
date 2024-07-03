#include <iostream>
#include <string>
#include <wb/whiteboard.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::string filepath = argv[1];
        wb::runWhiteboard(filepath);
    } else {
        wb::runWhiteboard();
    }
    return 0;
}
