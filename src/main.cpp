#include "../include/server.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string host = "0.0.0.0";
    int port = 8080;
    std::string webDir = "./web";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "--host" && i + 1 < argc) {
            host = argv[++i];
        } else if (arg == "--web" && i + 1 < argc) {
            webDir = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: algo_forge_server [options]\n\n"
                      << "Options:\n"
                      << "  --port <number>  Port to listen on (default: 8080)\n"
                      << "  --host <ip>      Host address to bind to (default: 0.0.0.0)\n"
                      << "  --web <dir>      Path to static frontend files (default: ./web)\n"
                      << "  --help, -h       Display this help message\n";
            return 0;
        }
    }

    algo::server::runServer(host, port, webDir);
    return 0;
}
