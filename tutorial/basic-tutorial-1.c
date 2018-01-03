#include <gst/gst.h>

int main(int argc, char const *argv[]) {
    GstElement *pipeline;
    GstBus *bus;
    GstStateChangeReturn ret;
    GstMessage *msg = NULL;

    /* Initialize gstreamer */
    gst_init(&argc, &argv);

    /* Build the pipeline */
    pipeline = gst_parse_launch("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);

    /* Start Playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    } else if (ret == GST_STATE_CHANGE_NO_PREROLL) {
        g_printerr("GST_STATE_CHANGE_NO_PREROLL Unable to set the pipeline to the playing state.\n");
        // data.is_live = TRUE;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
#if 0   // XXX: FATAL on MacOS
    msg = gst_bus_timed_pop_filtered(bus,
            GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    // msg = gst_bus_pop(bus);
#else   // GOOD on MacOS
    GMainLoop *main_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(main_loop);
#endif
    /* Free resources */
    if (msg != NULL) {
        gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}
