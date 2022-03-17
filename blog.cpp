#include <Wt/WApplication.h>

class BlogApplication : public Wt::WApplication {
  public:
    BlogApplication(const Wt::WEnvironment& env);
};


BlogApplication::BlogApplication(const Wt::WEnvironment& env) : Wt::WApplication(env) {
  setTitle("natanhp.id");
}


int main(int argc, char **argv) {
  return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
      return std::make_unique<BlogApplication>(env);
      });
}
