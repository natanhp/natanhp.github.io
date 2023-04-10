//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#include "about.h"
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>

AboutPage::AboutPage() : Wt::WContainerWidget() {
    this->setStyleClass("mx-5 my-3 px-5");
    auto vbox = this->setLayout(std::make_unique<Wt::WVBoxLayout>());

    auto item = std::make_unique<Wt::WText>("Site Is Under Construction ^_^");
    item->setStyleClass("h2 text-center mb-4");
    vbox->addWidget(std::move(item));

    item = std::make_unique<Wt::WText>(
            "<p>Unfortunately this website is still under construction. "
            "Here's a picture of Mitsuha Miyamuzu to cure your disappointment :).</p>");
    item->setStyleClass("text-start");
    vbox->addWidget(std::move(item));

    auto image = std::make_unique<Wt::WImage>(Wt::WLink("resources/images/mitsuha-miyamuzu.webp"));
    image->setAlternateText("Kojima Kana");
    image->setMaximumSize(500, Wt::WLength::Auto);
    image->addStyleClass("rounded float-right img-fluid img-thumbnail mx-auto mb-3");
    vbox->addWidget(std::move(image));

    item = std::make_unique<Wt::WText>("<p>I'm working on this project while you're staring at her. "
                                       "After I deleted my never-finished projects in various frameworks and "
                                       "architecture, I finally (hopefully can) stick to this project that I write in C++."
                                       " I hope I can finish it this time without switching to another framework or "
                                       "architecture. DM me on Twitter "
                                       "<a href=\"https://twitter.com/Natan_____\" target=\"_blank\">@Natan_____</a> "
                                       "if you have any suggestions. Thank you.</p>");
    item->setStyleClass("text-start");
    vbox->addWidget(std::move(item));
}
