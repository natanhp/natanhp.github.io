

#include <Wt/WServer.h>
#include "blog.h"

int main(int argc, char **argv) {
    Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

    server.addEntryPoint(Wt::EntryPointType::Application,
                         [&](const Wt::WEnvironment &env) {
                             return std::make_unique<BlogApplication>(env);
                         },
                         "\0",
                         "resources/icons/favicon.ico"
    );

    server.run();

    return 0;
}
