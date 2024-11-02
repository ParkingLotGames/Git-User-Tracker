#pragma once
#include <gtk/gtk.h>
#include <cairo.h>
#include <string>
#include <cmath>

class IconGenerator {
public:
    static constexpr int ICON_SIZE = 16;
    GdkPixbuf* createColoredIcon(const std::string& colorHex);
};