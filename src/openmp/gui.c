#include "gui.h"
#include <gtk/gtk.h>

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
GtkWidget *drawing_area;

static double *results;
static int size = 0;

static void clear_surface(void)
{
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if (surface)
        cairo_surface_destroy(surface);

    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
                                                CAIRO_CONTENT_COLOR,
                                                gtk_widget_get_allocated_width(widget),
                                                gtk_widget_get_allocated_height(widget));

    /* Initialize the surface to white */
    clear_surface();

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush(GtkWidget *widget, 
    gdouble x, 
    gdouble y, 
    int brushWidth, 
    gdouble r, 
    gdouble g, 
    gdouble b)
{
    cairo_t *cr;

    //printf("x=%lf, y=%lf\n", x, y);

    /* Paint to the surface, where we store our state */
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, r, g, b);

    cairo_rectangle(cr, x - brushWidth/2, y - brushWidth/2, brushWidth, brushWidth);
    cairo_fill(cr);
}

static void close_window(void)
{
    if (surface)
        cairo_surface_destroy(surface);

    gtk_main_quit();
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *frame;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(window), frame);

    drawing_area = gtk_drawing_area_new();
    /* set a minimum size */
    gtk_widget_set_size_request(drawing_area, 1100, 300);

    gtk_container_add(GTK_CONTAINER(frame), drawing_area);

    /* Signals used to handle the backing surface */
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event_cb), NULL);

    gtk_widget_show_all(window);

    //TODO: use timer for animation (http://zetcode.com/gui/gtk2/gtkevents/)

    draw();
}

int init_gui(double r[], int s)
{
    results = r;
    size = s;

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return status;
}

void draw() 
{
    clear_surface();

    for (int i=0; i<150; i++) {
        draw_brush(drawing_area, 40, i+75, 2, 0, 0, 0);
    }

    for (int i=0; i<size; i++){
        draw_brush(drawing_area, i+40, 150, 1, 0, 0, 0);
        draw_brush(drawing_area, i+40, results[i]*50+150, 4, 0, 0, 1);
    }
}