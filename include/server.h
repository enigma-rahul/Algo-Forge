#ifndef ALGO_FORGE_SERVER_H
#define ALGO_FORGE_SERVER_H

#include <string>

namespace algo {
namespace server {

/**
 * Starts the Algo Forge HTTP API and static file web server.
 * @param host Bind address (default "0.0.0.0" or "localhost")
 * @param port Port number (default 8080)
 * @param webDir Directory containing static web assets (default "./web")
 */
void runServer(const std::string& host, int port, const std::string& webDir);

} // namespace server
} // namespace algo

#endif // ALGO_FORGE_SERVER_H
