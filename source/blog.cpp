#include <Wt/WApplication.h>
#include <Wt/WBootstrap5Theme.h>
#include <WServer.h>

class BlogApplication : public Wt::WApplication {
public:
    BlogApplication(const Wt::WEnvironment &env);
};


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());
}


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
