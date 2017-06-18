#include <gtk/gtk.h>
#include <unistd.h>
#include <pthread.h>
#include "core.h"
#include "config.h"

#define X_OFFSET 40
#define Y_OFFSET 20
#define MAGNIFICATION 50
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 315

gboolean on_window_configure_event(GtkWidget * da, GdkEventConfigure * event, gpointer user_data);
gboolean on_window_expose_event(GtkWidget * da, GdkEventExpose * event, gpointer user_data);
void *do_draw(void *ptr);
gboolean timer_exe(GtkWidget * window);

static GdkPixmap *pixmap = NULL; //serves as our buffer
static int currently_drawing = 0;
static unsigned int currentSimulationStep = 0;

/* 
 * Is executed when the window's size, position or stacking is changed.
 * If necessary, creates a new properly sized pixmap and destroys the old one.
 */
gboolean on_window_configure_event(GtkWidget * da, GdkEventConfigure * event, gpointer user_data)
{
    static int oldw = 0;
    static int oldh = 0;
    if (oldw != event->width || oldh != event->height)
    {
        GdkPixmap *tmppixmap = gdk_pixmap_new(da->window, event->width,  event->height, -1);
        //copy the contents of the old pixmap to the new pixmap.
        int minw = oldw, minh = oldh;
        if( event->width < minw)
        {
            minw =  event->width;
        }
        if(event->height < minh)
        {
            minh =  event->height;
        }
        gdk_draw_drawable(tmppixmap, da->style->fg_gc[GTK_WIDGET_STATE(da)], pixmap, 0, 0, 0, 0, minw, minh);
        g_object_unref(pixmap); //delete old pixmap
        pixmap = tmppixmap; //use new pixmap
    }
    oldw = event->width;
    oldh = event->height;
    return TRUE;
}

/*
 * Is executed when the window is going to be redrawn.
 */
gboolean on_window_expose_event(GtkWidget * da, GdkEventExpose * event, gpointer user_data)
{
    gdk_draw_drawable(da->window,
        da->style->fg_gc[GTK_WIDGET_STATE(da)], pixmap,
        // Only copy the area that was exposed.
        event->area.x, event->area.y,
        event->area.x, event->area.y,
        event->area.width, event->area.height);
    return TRUE;
}

void *do_draw(void *ptr)
{
    currently_drawing = 1;

    int width, height;
    char currentMark[3];

    gdk_threads_enter();
    gdk_drawable_get_size(pixmap, &width, &height);
    gdk_threads_leave();

    //create a gtk-independant surface to draw on
    cairo_surface_t *cst = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(cst);

    simulate();
    if (0 != getSimulationSteps())
    {
        currentSimulationStep++;
    }

    //clear area
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint(cr);

    //draw y-axis with scale marks
    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_move_to(cr, X_OFFSET, Y_OFFSET);
    cairo_line_to(cr, X_OFFSET, height-Y_OFFSET);

    for (int i=MAGNIFICATION; i<height/2-MAGNIFICATION; i+=MAGNIFICATION)
    {
        sprintf(currentMark, "%d", i/MAGNIFICATION);
        cairo_move_to(cr, X_OFFSET-15, -i + height/2+4);
        cairo_show_text(cr, currentMark);
        cairo_move_to(cr, X_OFFSET-5, -i + height/2);
        cairo_line_to(cr, X_OFFSET+5, -i + height/2);

        sprintf(currentMark, "%d", -i/MAGNIFICATION);
        cairo_move_to(cr, X_OFFSET-18, i + height/2+4);
        cairo_show_text(cr, currentMark);
        cairo_move_to(cr, X_OFFSET-5, i + height/2);
        cairo_line_to(cr, X_OFFSET+5, i + height/2);
    }

    cairo_close_path(cr);

    //draw x-axis with scale marks
    cairo_move_to(cr, X_OFFSET-15, height/2+4);
    cairo_show_text(cr, "0");
    cairo_move_to(cr, X_OFFSET, height/2);
    cairo_line_to(cr, width-X_OFFSET, height/2);

    for (int i=MAGNIFICATION+X_OFFSET; i < width-X_OFFSET-MAGNIFICATION; i+=MAGNIFICATION)
    {
        sprintf(currentMark, "%d", i/MAGNIFICATION);
        cairo_move_to(cr, i-4, height/2+15);
        cairo_show_text(cr, currentMark);
        cairo_move_to(cr, i, height/2-5);
        cairo_line_to(cr, i, height/2+5);
    }

    cairo_stroke(cr);

    //draw arrow heads
    cairo_move_to(cr, X_OFFSET, Y_OFFSET-5);
    cairo_rel_line_to(cr, 5, 10);
    cairo_rel_line_to(cr, -10, 0);
    cairo_rel_line_to(cr, 5, -10);

    cairo_move_to(cr, width-X_OFFSET+5, height/2);
    cairo_rel_line_to(cr, -10, -5);
    cairo_rel_line_to(cr, 0, 10);
    cairo_rel_line_to(cr, 10, -5);
    cairo_fill(cr);

    //draw curve
    double* results = getNewValues();
    int size = getArraySize();
    cairo_set_source_rgb (cr, 0, 0, 1);
    for(int i = 1; i < size; i++)
    {
        cairo_move_to(cr, i-1+X_OFFSET, -results[i-1]*MAGNIFICATION+height/2);
        cairo_line_to(cr, i+X_OFFSET, -results[i]*MAGNIFICATION+height/2);
        cairo_stroke(cr);
    }
    cairo_destroy(cr);

    //When dealing with gdkPixmap's, we need to make sure not to
    //access them from outside gtk_main().
    gdk_threads_enter();

    cairo_t *cr_pixmap = gdk_cairo_create(pixmap);
    cairo_set_source_surface (cr_pixmap, cst, 0, 0);
    cairo_paint(cr_pixmap);
    cairo_destroy(cr_pixmap);

    gdk_threads_leave();

    cairo_surface_destroy(cst);

    currently_drawing = 0;

    return NULL;
}

gboolean timer_exe(GtkWidget * window)
{
    //use a safe function to get the value of currently_drawing so
    //we don't run into the usual multithreading issues
    int drawing_status = g_atomic_int_get(&currently_drawing);
    
    unsigned int simulationSteps = getSimulationSteps();
    if (0 != simulationSteps) 
    {
        unsigned int simulationStatus = g_atomic_int_get(&currentSimulationStep);
        if (simulationStatus > simulationSteps)
        {
            gtk_main_quit();
        }
    }

    //if we are not currently drawing anything, launch a thread to
    //update our pixmap
    if(drawing_status == 0)
    {
        static pthread_t thread_info;
        pthread_join(thread_info, NULL);
        pthread_create( &thread_info, NULL, do_draw, NULL); //do_draw is executed in a separate thread
    }

    //tell our window it is time to draw our animation.
    int width, height;
    gdk_drawable_get_size(pixmap, &width, &height);
    gtk_widget_queue_draw_area(window, 0, 0, width, height);

    return TRUE;
}

int main (int argc, char *argv[])
{
    readConfig();
    init(getC()); //init calculation

    if (!isGui())
    {
        for(int i=0; i < getSimulationSteps(); i++)
        {
            simulate();
        }
        output();
        return 0;
    }
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Wave Equation");
    //gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(on_window_expose_event), NULL);
    g_signal_connect(G_OBJECT(window), "configure_event", G_CALLBACK(on_window_configure_event), NULL);
    gtk_widget_show_all(window);
    pixmap = gdk_pixmap_new(window->window,500,500,-1);
    //turn off because because painting is done programmatically
    gtk_widget_set_app_paintable(window, TRUE);
    gtk_widget_set_double_buffered(window, FALSE);

    (void)g_timeout_add(33, (GSourceFunc)timer_exe, window); //equals approx. 30 fps

    gtk_main();
    gdk_threads_leave();

    return 0;
}