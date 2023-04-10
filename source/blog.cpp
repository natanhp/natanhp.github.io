//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include <Wt/WBootstrap5Theme.h>
#include <Wt/WLineEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include "Wt/WContainerWidget.h"
#include "blog.h"
#include "about.h"


BlogApplication::BlogApplication(const Wt::WEnvironment &env) : Wt::WApplication(env) {
    setTitle("natanhp.id");
    /* useStyleSheet("resources/css/main.css"); */
    setTheme(std::make_shared<Wt::WBootstrap5Theme>());

    auto *navigation = initNavBar();
    auto *contentsStack = initStackedWidget();
    buildLeftMenu(navigation, contentsStack);
    buildRightMenu(navigation);
}

Wt::WNavigationBar *BlogApplication::initNavBar() {
    auto container = root()->addNew<Wt::WContainerWidget>();
    auto *navigation = container->addNew<Wt::WNavigationBar>();
    navigation->setResponsive(true);
    navigation->addStyleClass("navbar");

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

    leftMenu_->addItem("Home", std::make_unique<AboutPage>())
            ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/"));
    leftMenu_->addStyleClass("me-auto");
}
