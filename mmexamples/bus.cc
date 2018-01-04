/*
 * This example presents basic usage of the Gst::Bus class.
 */
#include <gstreamermm.h>
#include <glibmm/main.h>
#include <iostream>

Glib::RefPtr<Glib::MainLoop> main_loop;

// Message watch function
bool bus_message_watch(const Glib::RefPtr<Gst::Bus>& /* bus */,
        const Glib::RefPtr<Gst::Message>& message) {
    // Print type of the message posted on the bus, and the source object name.
    std::cout << "Got message of type "
            << Gst::Enums::get_name(message->get_message_type()) << '\n';

    if (message->get_source()) {
        std::cout << "Source object: "
                << message->get_source()->get_name() << '\n';
    }

    switch (message->get_message_type()) {
    // Handle ERROR message - print error and debug information
    case Gst::MESSAGE_ERROR: {
        auto error_msg = Glib::RefPtr<Gst::MessageError>::cast_static(message);
        std::cout << "Error: " << error_msg->parse_error().what() << '\n';
        break;
    }
    // Handle EOS message - quit the loop
    case Gst::MESSAGE_EOS:
        main_loop->quit();
        break;
    case Gst::MESSAGE_STATE_CHANGED: {
        auto state_changed_msg = Glib::RefPtr<Gst::MessageStateChanged>::cast_static(message);
        std::cout << "Old state: "
                << Gst::Enums::get_name(state_changed_msg->parse_old_state()) << '\n';
        std::cout << "New state: "
                << Gst::Enums::get_name(state_changed_msg->parse_new_state()) << '\n';
        break;
    }
    // Unhandled MessageStateChanged
    default:
        break;
    }

    return true;
}

int main(int argc, char *argv[]) {
    Gst::init(argc, argv);

    // Create some elements
    auto source = Gst::ElementFactory::create_element("videotestsrc", "source");
    auto sink = Gst::ElementFactory::create_element("autovideosink", "sink");

    // Create pipeline
    auto pipeline = Gst::Pipeline::create("my-pipeline");

    // Add elements to a pipeline
    try {
        pipeline->add(source)->add(sink);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while adding: " << ex.what() << '\n';
        return 1;
    }

    // Link elements
    try {
        source->link(sink);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while linking: " << ex.what() << '\n';
    }

    // Get a bus object of the pipeline
    Glib::RefPtr<Gst::Bus> bus = pipeline->get_bus();

    // Add watch to the bus
    bus->add_watch(sigc::ptr_fun(bus_message_watch));

    // Start pipeline
    pipeline->set_state(Gst::STATE_PLAYING);

    main_loop = Glib::MainLoop::create();
    main_loop->run();

    // Stop playing the pipeline
    pipeline->set_state(Gst::STATE_NULL);
    return 0;
}
