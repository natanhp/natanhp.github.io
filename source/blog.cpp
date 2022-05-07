//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include <WBootstrap5Theme.h>
#include <Wt/WLineEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include "blog.h"


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());

    auto *navigation = root()->addNew<Wt::WNavigationBar>();

    navigation->addStyleClass("navbar-dark bg-dark");

    auto *contentsStack = root()->addNew<Wt::WStackedWidget>();
    contentsStack->addStyleClass("contents");

    auto leftMenu = std::make_unique<Wt::WMenu>(contentsStack);
    auto leftMenu_ = navigation->addMenu(std::move(leftMenu));

    leftMenu_->addItem("Home", std::make_unique<Wt::WText>("There is no better place!"));
    leftMenu_->addItem("Layout", std::make_unique<Wt::WText>("Layout contents"))
            ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/layout"));
    leftMenu_->addStyleClass("me-auto");

// Add a Search control.
    auto editPtr = std::make_unique<Wt::WLineEdit>();
    auto edit = editPtr.get();
    edit->setPlaceholderText("Enter a search item");

    edit->enterPressed().connect([=] {
        leftMenu_->select(2); // is the index of the "Sales"
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
