//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include <Wt/WBootstrap5Theme.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WHBoxLayout.h>
#include <memory>
#include <utility>
#include <Wt/WText.h>
#include "Wt/WLength.h"
#include "blog.h"
#include "about.h"

BlogApplication::BlogApplication(const Wt::WEnvironment &env)
    : Wt::WApplication(env) {
  setTitle("natanhp.id");
  setTheme(std::make_shared<Wt::WBootstrap5Theme>());

  auto *layout = initMainLayout();
  auto *navigation = initNavBar(layout);
  auto *contentsStack = initStackedWidget(layout);
  buildLeftMenu(navigation, contentsStack);
  buildFooter(layout);
}

void BlogApplication::buildFooter(Wt::WBorderLayout *layout) {
  auto footerItem = std::make_unique<Wt::WContainerWidget>();
  auto footer =
      layout->addWidget(std::move(footerItem), Wt::LayoutPosition::South);
  auto footerLayout = footer->setLayout(std::make_unique<Wt::WHBoxLayout>());
  auto image = std::make_unique<Wt::WImage>(Wt::WLink("resources/icons/favicon.ico"));
  image->setAlternateText("natanhp.id");
  image->setMaximumSize(Wt::WLength(35), Wt::WLength(35));
  footer->setStyleClass("d-flex flex-wrap justify-content-between align-items-center p-3 mt-4 border-top");
  footerLayout->addWidget(std::move(image));
  auto creatorMarkText = std::make_unique<Wt::WText>("Natan Hari Pamungkas 2022");
  creatorMarkText->setStyleClass("fw-bolder fs-6");
  footerLayout->addWidget(std::move(creatorMarkText));
}

Wt::WNavigationBar *BlogApplication::initNavBar(Wt::WBorderLayout *layout) {
    auto navBarItem = std::make_unique<Wt::WNavigationBar>();
    auto *navigation =layout->addWidget(std::move(navBarItem), Wt::LayoutPosition::North);
    navigation->addStyleClass("navbar");

    return navigation;
}

Wt::WStackedWidget *BlogApplication::initStackedWidget(Wt::WBorderLayout *layout) {
    auto contentStackItem = std::make_unique<Wt::WStackedWidget>();
    auto *contentsStack = layout->addWidget(std::move(contentStackItem), Wt::LayoutPosition::Center);

    return contentsStack;
}

void BlogApplication::buildLeftMenu(Wt::WNavigationBar *navBar, Wt::WStackedWidget *stackedWidget) {
    auto leftMenu = std::make_unique<Wt::WMenu>(stackedWidget);
    auto leftMenu_ = navBar->addMenu(std::move(leftMenu));

    leftMenu_->addItem("Home", std::make_unique<AboutPage>())
            ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/"));
    leftMenu_->addStyleClass("me-auto");
}

Wt::WBorderLayout *BlogApplication::initMainLayout() {
    auto container = root()->addNew<Wt::WContainerWidget>();
    return container->setLayout(std::make_unique<Wt::WBorderLayout>());
}
