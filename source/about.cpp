//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include "about.h"
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include "util.h"

AboutPage::AboutPage() : Wt::WContainerWidget() {
    this->setStyleClass("mx-5 my-3 px-5");
    auto vbox = this->setLayout(std::make_unique<Wt::WVBoxLayout>());
    auto content = Util::ReadHTMLFile("about.html");
    auto item = std::make_unique<Wt::WText>(content);
    vbox->addWidget(std::move(item));
}
