/**
 * Linux x86
 * Compile with: `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This is a graphical user interface to dim LED's
 */

#include <gtk/gtk.h>
#include <math.h>

GtkWidget *window;
double ledValues[] = {1, 0.5, 0, 1, 0.5, 0, 1, 0.5};
int sliderAddresses[8];
int i;

/**
 * GTK event that is triggered when the LED sliders are moved
 */
static void slider_changed(GtkAdjustment *adj)
{
    for (i = 0; i < sizeof(sliderAddresses) / sizeof(int); i++) {
        if (sliderAddresses[i] == (int) adj) {
            ledValues[i] = (double) adj->value / 100;
            gtk_widget_queue_draw(window);
            return;
        }
    }
}

/**
 * Repaints the interface
 */
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    for (i = 0; i < sizeof(ledValues) / sizeof(double); i++) {
        cairo_t *cr;
        cr = gdk_cairo_create(widget->window);

        cairo_set_line_width(cr, 7);
        cairo_set_source_rgb(cr, 0.5, 0.5, 0);
        cairo_arc(cr, 30 + (i * 50), 140, 15, 0, 2 * M_PI);
        cairo_stroke_preserve(cr);
        cairo_set_source_rgb(cr, 1, 1 - ledValues[i], 0);
        cairo_fill(cr);

        cairo_destroy(cr);
    }
    return FALSE;
}

void create_controls(void)
{
    GtkWidget *fixed;
    GtkWidget *vscale;
    GtkObject *adj;

    /* build window and fixed panel */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "LED dimmers");
    gtk_window_set_default_size(GTK_WINDOW(window), 415, 170);
    gtk_widget_set_app_paintable(window, TRUE);
    g_signal_connect(window, "expose-event", G_CALLBACK(on_expose_event), NULL);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    /* add sliders */
    for (i = 0; i < sizeof(ledValues) / sizeof(double); i++) {
        adj = gtk_adjustment_new(ledValues[i] * 100, 0, 100, 1, 1, 0);
        g_signal_connect(adj, "value_changed", G_CALLBACK(slider_changed), NULL);
        sliderAddresses[i] = (int) adj;
        vscale = gtk_vscale_new(GTK_ADJUSTMENT(adj));
        gtk_scale_set_digits(GTK_SCALE(vscale), 0);
        gtk_fixed_put(GTK_FIXED(fixed), vscale, 15 + (i * 50), 10);
        gtk_widget_set_size_request(vscale, 30, 100);
    }

    /* show window and components */
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    create_controls();
    gtk_main();

    return 0;
}

