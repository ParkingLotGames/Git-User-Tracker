#pragma once
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
#include <map>
#include <string>
#include <memory>
#include "config.hpp"
#include "icon.hpp"
#include "cmd.hpp"

class GitUserTracker {
public:
    GitUserTracker();
    ~GitUserTracker();
    void run();

private:
    AppIndicator* indicator;
    GtkWidget* menu;
    guint timer;
    std::map<std::string, std::string> userColors;
    std::map<std::string, GdkPixbuf*> userIcons;
    std::string previousUser;
    std::string temporaryIconPath;
    ConfigLoader configLoader;
    IconGenerator iconGenerator;
    GitCommand gitCommand;

    void initializeGtk();
    void createIndicator();
    void createMenu();
    void startTimer();
    static void quitCallback(GtkMenuItem* item, gpointer data);
    static gboolean checkGitUser(gpointer data);
};