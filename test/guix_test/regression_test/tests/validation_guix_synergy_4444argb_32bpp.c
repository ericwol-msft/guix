/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_synergy_4444argb_32bpp", /* Test name */
    19, 14, 189, 184  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_dave2d_graphics_driver_setup_24xrgb  
#undef win32_dave2d_graphics_driver_setup_24xrgb  
#endif
#define win32_dave2d_graphics_driver_setup_24xrgb  gx_validation_dave2d_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_synergy_4444argb_32bpp.c"
/* Define the move values for the horizontal slider. */

/* Define the move values for the vertical slider.*/

/* Define the alpha settings. */

/* Define the compressed settings. */
int format_value[4] = {GX_PIXELMAP_ID_RED_APPLE,GX_PIXELMAP_ID_ICON_FOOT_ALPHA,GX_PIXELMAP_ID_RAINBOW200_ALPHA_NONCOMPRESS,GX_PIXELMAP_ID_RAINBOW200_ALPHA_COMPRESS};
/* Define the y coordinate of mouse click.*/

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
    int format_index;
    int frame_id = 1;


        for(format_index = 0 ; format_index <(int)(sizeof(format_value)/sizeof(int)); format_index++)
        {
	    format = format_value[format_index];
	    /* Inform the validation system 
	    (1) Frame ID, which identifies a specific test configuration;
	    (2) Start recording frame on the next toggle operation.
	    */
	    gx_validation_set_frame_id(frame_id);

	    /* Set the vertical slider value */
	    switch (format)
	    {

	    case GX_PIXELMAP_ID_RED_APPLE:
		strcat(test_comment, "uncompressed for 4444BGRA\n ");
		break;

	    case GX_PIXELMAP_ID_ICON_FOOT_ALPHA:
		strcat(test_comment, "compressed for 4444BGRA\n ");
		break;

	    case GX_PIXELMAP_ID_RAINBOW200_ALPHA_COMPRESS:
		strcat(test_comment, "compressed for 4444ARGB\n ");
		break;

	    case GX_PIXELMAP_ID_RAINBOW200_ALPHA_NONCOMPRESS:
		strcat(test_comment, "uncompressed for 4444ARGB\n ");
		break;

	    }
	    gx_validation_set_frame_comment(test_comment);

	    /* Mark the window "dirty" */
	    gx_system_dirty_mark(pic_window);

	    /* Force a screen refresh. */
	    gx_validation_screen_refresh();

	    /* Increment frame ID */
	    frame_id ++;
	
        }
        
    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}


