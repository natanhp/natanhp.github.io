//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#ifndef NATANHP_ID_BLOG_H
#define NATANHP_ID_BLOG_H

#include "Wt/WContainerWidget.h"
#include <Wt/WApplication.h>

class BlogApplication : public Wt::WApplication {
public:
    explicit BlogApplication(const Wt::WEnvironment &env);

private:
    Wt::WNavigationBar *initNavBar(Wt::WBorderLayout *layout);

    Wt::WStackedWidget *initStackedWidget(Wt::WBorderLayout *layout);

    static void buildLeftMenu(Wt::WNavigationBar *navBar, Wt::WStackedWidget *stackedWidget);
};

#endif //NATANHP_ID_BLOG_H
