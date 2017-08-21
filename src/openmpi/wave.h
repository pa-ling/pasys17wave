/**
 * @file config.h
 * @author Patrick Fehling
 * @date 7 Aug 2017
 * @brief File which initializes the program and presents a GUI to the user
 */

#include <gtk/gtk.h>
#include <unistd.h>
#include <pthread.h>
#include "core.h"
#include "config.h"
#include "mpi.h"

#define X_OFFSET 40
#define Y_OFFSET 20
#define MAGNIFICATION 50
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 315
#define MASTER 0

/* 
 * Is executed when the window's size, position or stacking is changed and
 * adjusts these appropriately.
 * If necessary, creates a new properly sized pixmap and destroys the old one.
 *
 * @param da the drawable gtk-widget
 * @param event additional data of the configure event (e.g. width, height of the window)
 * @param user_data unused
 * @return unused
 */
gboolean on_window_configure_event(GtkWidget *da, GdkEventConfigure *event, gpointer user_data);

/*
 * Is executed when the window is going to be redrawn.
 * Makes sure only the area is drawn which is visible to the user.
 *
 * @param da the drawable gtk-widget
 * @param event additional data of the expose event (e.g. width, height of the window)
 * @param user_data unused
 * @return unused
 */
gboolean on_window_expose_event(GtkWidget *da, GdkEventExpose *event, gpointer user_data);

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
 * @return unused
 */
gboolean timer_exe(GtkWidget *window);

/*
 * Wrapper to execute the simulation for n steps
 *
 * @param nSteps number of steps to execute
 * @param dataSize number of data points which have to be distributed
 * @param taskid taskid of the specific
 * @param numtask total number of operation nodes
 */
void calcWave_without_GUI(unsigned int nSteps, unsigned int dataSize, int taskid, int numtasks);


static GdkPixmap *pixmap = NULL; //serves as our buffer
static int currently_drawing = 0;
static unsigned int currentSimulationStep = 0;


