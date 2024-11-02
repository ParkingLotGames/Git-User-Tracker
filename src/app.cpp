#include "app.hpp"

GitUserTracker::GitUserTracker() : 
    indicator(nullptr),  // initialize the indicator to nullptr
    menu(nullptr),      // initialize the menu to nullptr
    timer(0) {          // initialize the timer to 0
    // Load user colors from the config and store in userColors map
    userColors = configLoader.loadUserColors();
    // For each user color, create a corresponding icon and store in userIcons map
    for (const auto& [username, color] : userColors) {
        userIcons[username] = iconGenerator.createColoredIcon(color);
    }
    // Initialize GTK for the application
    initializeGtk();
    // Create the indicator for the application
    createIndicator();
    // Create the menu for the application
    createMenu();
    // Start the timer for the application
    startTimer();
}

GitUserTracker::~GitUserTracker() {
    // If the menu exists, release its resources
    if (menu) g_object_unref(menu);
    // If the timer is running, stop it
    if (timer) g_source_remove(timer);
    // For each icon in the userIcons map, release its resources
    for (auto& [_, icon] : userIcons) {
        g_object_unref(icon);
    }
    // If a temporary icon file exists, delete it
    if (!temporaryIconPath.empty()) {
        unlink(temporaryIconPath.c_str());
    }
}

void GitUserTracker::initializeGtk() {
    // Initializes GTK with no command line arguments and no additional initialization.
    gtk_init(nullptr, nullptr);
}

void GitUserTracker::createIndicator() {
    GError* error = nullptr;

    // `tempname` is an array that holds the path to a temporary icon file.
    char tempname[] = "/tmp/git-user-XXXXXX";
    // `g_mkstemp(tempname)` creates a unique temporary file and returns its file descriptor.
    int fileDescriptor = g_mkstemp(tempname);
    
    // If the file was created successfully,
    if (fileDescriptor != -1) {
        // close the file descriptor with `close(fileDescriptor)`.
        close(fileDescriptor);
        // If the `userIcons` map is not empty,
        if (!userIcons.empty()) {
            // save the first icon in the `userIcons` map to the temporary file as a PNG image.
            if (gdk_pixbuf_save(userIcons.begin()->second, tempname, "png", &error, NULL)) {
                // Success
            }
        }
    }
    
    // Create a new indicator with the ID "git-user-tracker", the path to the temporary icon file, and the category "APPLICATION_STATUS".
    indicator = app_indicator_new(
        "git-user-tracker",
        tempname,
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    
    // Set the status of the indicator to "ACTIVE".
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    // Store the path to the temporary icon file in the `temporaryIconPath` member variable.
    temporaryIconPath = tempname;
}

void GitUserTracker::createMenu() {
    // Create a new empty menu with `gtk_menu_new()`.
    menu = gtk_menu_new();

    // I don't really need a quit button so...

    // Create a new menu item with the label "Quit".
    // GtkWidget* quitItem = gtk_menu_item_new_with_label("Quit");
    // Add the "Quit" menu item to the menu.
    // gtk_menu_shell_append(GTK_MENU_SHELL(menu), quitItem);
    // Connect the "activate" signal of the "Quit" menu item to the `quitCallback` function.
    // g_signal_connect(quitItem, "activate", G_CALLBACK(quitCallback), nullptr);
    // Show all the widgets in the menu.
    gtk_widget_show_all(menu);
    // Set the menu for the systray indicator.
    app_indicator_set_menu(indicator, GTK_MENU(menu));
}

void GitUserTracker::quitCallback(GtkMenuItem*, gpointer) {
    // Quit the main event loop.
    gtk_main_quit();
}
// This is a static member function of GitUserTracker class which is a callback for the glib event loop.
// It performs the task of checking the current git user and updating the application's icon and status accordingly.
gboolean GitUserTracker::checkGitUser(gpointer data) {
    // Convert the provided gpointer data to a GitUserTracker pointer.
    auto* app = static_cast<GitUserTracker*>(data);

    // Execute the git command to get the global user name.
    std::string username = GitCommand::executeCommand("git config user.name");
    
    // If a username was successfully retrieved,
    if (!username.empty()) {
        // Remove any trailing newline characters from the username.
        size_t pos = username.find_first_of("\r\n");
        if (pos != std::string::npos) {
            username = username.substr(0, pos);
        }
        
        // If the current username is different from the previously stored username,
        if (username != app->previousUser) {
            // Update the previousUser member variable to the new username.
            app->previousUser = username;

            // Try to find the new username in the userIcons map.
            auto it = app->userIcons.find(username);

            // If the username was found in the userIcons map,
            if (it != app->userIcons.end()) {
                // Create an error pointer and initialize it to nullptr.
                GError* error = nullptr;

                // Try to save the icon for the new username to the temporary icon file path.
                if (gdk_pixbuf_save(it->second, app->temporaryIconPath.c_str(), "png", &error, NULL)) {
                    // If saving the icon was successful, set the application's indicator icon to the new icon.
                    app_indicator_set_icon_full(app->indicator, app->temporaryIconPath.c_str(), username.c_str());
                    
                    // Set a short timer to briefly change the indicator status to "ATTENTION".
                    g_timeout_add(100, [](gpointer data) -> gboolean {
                        auto* app = static_cast<GitUserTracker*>(data);
                        app_indicator_set_status(app->indicator, APP_INDICATOR_STATUS_ATTENTION);
                        
                        // Set a short timer to change the indicator status back to "ACTIVE".
                        g_timeout_add(100, [](gpointer data) -> gboolean {
                            auto* app = static_cast<GitUserTracker*>(data);
                            app_indicator_set_status(app->indicator, APP_INDICATOR_STATUS_ACTIVE);
                            return G_SOURCE_REMOVE;
                        }, app);
                        
                        return G_SOURCE_REMOVE;
                    }, app);
                }

                // If an error occurred during icon saving, free the error object.
                if (error) g_error_free(error);
            }
        }
    }

    // Return G_SOURCE_CONTINUE to indicate that the timer should continue.
    return G_SOURCE_CONTINUE;
}

// Check for git config user.name changes every 500ms
void GitUserTracker::startTimer() {
    timer = g_timeout_add(500, checkGitUser, this);
}

void GitUserTracker::run() {
    gtk_main();
}