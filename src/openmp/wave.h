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

/* 
 * Is executed when the window's size, position or stacking is changed and
 * adjusts these appropriately.
 * If necessary, creates a new properly sized pixmap and destroys the old one.
 *
 * @param da the drawable gtk-widget
 * @param event additional data of the configure event (e.g. width, height of the window)
 * @param user_data unused
 */
gboolean on_window_configure_event(GtkWidget * da, GdkEventConfigure * event, gpointer user_data);

/*
 * Is executed when the window is going to be redrawn.
 * Makes sure only the area is drawn which is visible to the user.
 *
 * @param da the drawable gtk-widget
 * @param event additional data of the expose event (e.g. width, height of the window)
 * @param user_data unused
 */
gboolean on_window_expose_event(GtkWidget * da, GdkEventExpose * event, gpointer user_data);

/*
 * Is executed when the X-window button is pressed and exits
 * the application.
 */
void close_window(void);

/*
 * Draws all of the content of the GUI, i.e. axes and graph.
 *
 * @param ptr
 */
void *do_draw(void *ptr);

/*
 * Triggers a new simulation step and a redrawing.
 *
 * @param window the application window
 */
gboolean timer_exe(GtkWidget * window);

static GdkPixmap *pixmap = NULL; //serves as our buffer
static int currently_drawing = 0;
static unsigned int currentSimulationStep = 0;