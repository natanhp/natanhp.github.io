#include <Wt/WApplication.h>
#include <Wt/WBootstrap5Theme.h>
#include <WServer.h>
#include <Wt/WLineEdit.h>
#include <Wt/WMenu.h>
#include <Wt/WMessageBox.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>

class BlogApplication : public Wt::WApplication {
public:
    BlogApplication(const Wt::WEnvironment &env);
};


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());

// Create a navigation bar with a link to a web page.
    Wt::WNavigationBar *navigation = root()->addNew<Wt::WNavigationBar>();
// It's not necessary to do this with WBootstrap5Theme, but we include it if
// you want to use another theme.
    navigation->setResponsive(true);
// WBootstrap5Theme applies no color scheme by default, so we have to add them here.
    navigation->addStyleClass("navbar-light bg-light");
    navigation->setTitle("Corpy Inc.",
                         "https://www.google.com/search?q=corpy+inc");

    Wt::WStackedWidget *contentsStack = root()->addNew<Wt::WStackedWidget>();
    contentsStack->addStyleClass("contents");

// Setup a Left-aligned menu.
    auto leftMenu = std::make_unique<Wt::WMenu>(contentsStack);
    auto leftMenu_ = navigation->addMenu(std::move(leftMenu));

    auto searchResult = std::make_unique<Wt::WText>("Buy or Sell... Bye!");
    auto searchResult_ = searchResult.get();

    leftMenu_->addItem("Home", std::make_unique<Wt::WText>("There is no better place!"));
    leftMenu_->addItem("Layout", std::make_unique<Wt::WText>("Layout contents"))
            ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/layout"));
    leftMenu_->addItem("Sales", std::move(searchResult));
    leftMenu_->addStyleClass("me-auto");

// Add a Search control.
    auto editPtr = std::make_unique<Wt::WLineEdit>();
    auto edit = editPtr.get();
    edit->setPlaceholderText("Enter a search item");

    edit->enterPressed().connect([=] {
        leftMenu_->select(2); // is the index of the "Sales"
        searchResult_->setText(Wt::WString("Nothing found for {1}.")
                                       .arg(edit->text()));
    });

    navigation->addSearch(std::move(editPtr));

// Setup a Right-aligned menu.
    auto rightMenu = std::make_unique<Wt::WMenu>();
    auto rightMenu_ = navigation->addMenu(std::move(rightMenu));

// Create a popup submenu for the Help menu.
    auto popupPtr = std::make_unique<Wt::WPopupMenu>();
    auto popup = popupPtr.get();
    popup->addItem("Contents");
    popup->addItem("Index");
    popup->addSeparator();
    popup->addItem("About");

    popup->itemSelected().connect([=](Wt::WMenuItem *item) {
        auto messageBox = popup->addChild(
                std::make_unique<Wt::WMessageBox>
                        ("Help",
                         Wt::WString("<p>Showing Help: {1}</p>").arg(item->text()),
                         Wt::Icon::Information,
                         Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            popup->removeChild(messageBox);
        });

        messageBox->show();
    });

    auto item = std::make_unique<Wt::WMenuItem>("Help");
    item->setMenu(std::move(popupPtr));
    rightMenu_->addItem(std::move(item));
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
