#include <gstreamermm.h>
#include <glibmm/main.h>
#include <glibmm/convert.h>
#include <iostream>
#include <cstdlib>
#include <gstreamermm/playbin.h>

namespace {

Glib::RefPtr<Glib::MainLoop> mainloop;

/* This function is used to receive asynchronous messages in the main loop. */
bool on_bus_message(const Glib::RefPtr<Gst::Bus>& /* bus*/,
        const Glib::RefPtr<Gst::Message>& message) {
    switch (message->get_message_type()) {
        case Gst::MESSAGE_EOS:
            std::cout << std::endl << "End of stream" << '\n';
            mainloop->quit();
            return false;

        case Gst::MESSAGE_ERROR:{
            Glib::RefPtr<Gst::MessageError> msgError =
                    Glib::RefPtr<Gst::MessageError>::cast_static(message);

            if (msgError) {
                Glib::Error err = msgError->parse_error();
                std::cerr << "Error: " << err.what() << '\n';
            } else
                std::cerr << "Error." << '\n';

            mainloop->quit();
            return false;
        }

        default:
            break;
    }

    return true;
}

} // anonymous namespace

int main(int argc, char *argv[]) {
    // Initialize gstreamermm
    Gst::init(argc, argv);

    // Check input arguements:
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <media file or uri>" << '\n';
        return EXIT_FAILURE;
    }

    // Create a playbin element.
    // Glib::RefPtr<Gst::PlayBin> playbin = Gst::PlayBin::create();
    Glib::RefPtr<Gst::Element> playbin = Gst::ElementFactory::create_element("playbin");
    if (!playbin) {
        std::cerr << "The playbin2 element could not be created." << '\n';
        return EXIT_FAILURE;
    }

    // Take the commandline arguement and ensure it is a uri:
    Glib::ustring uri;

    if (gst_uri_is_valid(argv[1])) {
        uri = argv[1];
    } else {
        uri = Glib::filename_to_uri(argv[1]);
    }

    // Set the playbin2's uri property.
    playbin->set_property("uri", uri);

    // Create the main loop.
    mainloop = Glib::MainLoop::create();

    // Get the bus from the playbin, and add a bus watch to the default main
    // context with the default priority:
    Glib::RefPtr<Gst::Bus> bus = playbin->get_bus();
    bus->add_watch(sigc::ptr_fun(&on_bus_message));

    // Now set the playbin to the PLAYING state and start the main loop:
    std::cout << "Setting to PLAYING." << '\n';
    playbin->set_state(Gst::STATE_PLAYING);
    std::cout << "Running." << '\n';
    mainloop->run();

    // Clean up nicely:
    std::cout << "Returned. Setting state to NULL." << '\n';
    playbin->set_state(Gst::STATE_NULL);

    return EXIT_SUCCESS;
}
