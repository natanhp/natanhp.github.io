//
// Created by Natan Hari Pamungkas on 5/7/22.
//

#ifndef NATANHP_ID_BLOG_H
#define NATANHP_ID_BLOG_H

#include <WApplication.h>

class BlogApplication : public Wt::WApplication {
public:
    explicit BlogApplication(const Wt::WEnvironment &env);

private:
    Wt::WNavigationBar *initNavBar();

    Wt::WStackedWidget *initStackedWidget();

    static void buildLeftMenu(Wt::WNavigationBar *navBar, Wt::WStackedWidget *stackedWidget);

    static void buildRightMenu(Wt::WNavigationBar *navBar);

    void buildFooter();
};

#endif //NATANHP_ID_BLOG_H
