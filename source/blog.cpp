//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include <WBootstrap5Theme.h>
#include <Wt/WLineEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include "blog.h"
#include "profile.h"


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());

    auto *navigation = initNavBar();
    auto *contentsStack = initStackedWidget();
    buildLeftMenu(navigation, contentsStack);
    buildRightMenu(navigation);
}

Wt::WNavigationBar *BlogApplication::initNavBar() {
    auto *navigation = root()->addNew<Wt::WNavigationBar>();
    navigation->addStyleClass("navbar-dark bg-dark");

    return navigation;
}

Wt::WStackedWidget *BlogApplication::initStackedWidget() {
    auto *contentsStack = root()->addNew<Wt::WStackedWidget>();
    contentsStack->addStyleClass("contents");

    return contentsStack;
}

void BlogApplication::buildLeftMenu(Wt::WNavigationBar *navBar, Wt::WStackedWidget *stackedWidget) {
    auto leftMenu = std::make_unique<Wt::WMenu>(stackedWidget);
    auto leftMenu_ = navBar->addMenu(std::move(leftMenu));

    leftMenu_->addItem("Home", std::make_unique<ProfilePage>())
            ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/"));
    leftMenu_->addStyleClass("me-auto");
}

void BlogApplication::buildRightMenu(Wt::WNavigationBar *navBar) {
    auto rightMenu = std::make_unique<Wt::WMenu>();
    auto rightMenu_ = navBar->addMenu(std::move(rightMenu));
}
