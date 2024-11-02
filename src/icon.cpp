#include "icon.hpp"

// createColoredIcon generates a colored icon and returns it as a GdkPixbuf pointer.
// It takes a string representing the color in hexadecimal.
GdkPixbuf* IconGenerator::createColoredIcon(const std::string& colorHex) {
    // Create a new cairo image surface with a size of ICON_SIZE x ICON_SIZE and ARGB32 format.
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ICON_SIZE, ICON_SIZE);
    // Create a cairo context for the surface.
    cairo_t* context = cairo_create(surface);

    unsigned int red, green, blue;
    // Extract the red, green, and blue values from the colorHex string.
    sscanf(colorHex.c_str(), "#%02x%02x%02x", &red, &green, &blue);

    // Set the source color to transparent and clear the surface.
    cairo_set_source_rgba(context, 0, 0, 0, 0);
    cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
    cairo_paint(context);
    // Reset the operator to the default (over).
    cairo_set_operator(context, CAIRO_OPERATOR_OVER);
    // Set the source color to the specified color.
    cairo_set_source_rgb(context, red/255.0, green/255.0, blue/255.0);
    // Draw a circle on the surface.
    cairo_arc(context, ICON_SIZE/2.0, ICON_SIZE/2.0, ICON_SIZE/2.0 - 1, 0, 2 * M_PI);
    // Set the antialiasing to the best quality.
    cairo_set_antialias(context, CAIRO_ANTIALIAS_BEST);
    // Fill the circle with the specified color.
    cairo_fill(context);

    // Convert the surface to a GdkPixbuf.
    GdkPixbuf* pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, ICON_SIZE, ICON_SIZE);
    // Destroy the context and surface to free up resources.
    cairo_destroy(context);
    cairo_surface_destroy(surface);
    // Return the GdkPixbuf pointer.
    return pixbuf;
}

