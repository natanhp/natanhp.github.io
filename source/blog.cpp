#include <Wt/WApplication.h>
#include <Wt/WBootstrap5Theme.h>

class BlogApplication : public Wt::WApplication {
public:
    BlogApplication(const Wt::WEnvironment &env);
};


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());
}


int main(int argc, char **argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
        return std::make_unique<BlogApplication>(env);
    });
}
