/*
 * This example presents basic usage of dynamics Gst::Pad objects.
 */
#include <gstreamermm.h>
#include <glibmm/main.h>

#include <iostream>

int main(int argc, char *argv[]) {
    Gst::init(argc, argv);

    auto main_loop = Glib::MainLoop::create();

    // Create pipeline
    auto pipeline = Gst::Pipeline::create("my_pipeline");

    // Create elements
    auto source = Gst::ElementFactory::create_element("videotestsrc", "source");
    auto decodebin = Gst::ElementFactory::create_element("decodebin", "decoder");
    auto sink = Gst::ElementFactory::create_element("autovideosink", "videosink");

    // Add elements to a pipeline
    try {
        pipeline->add(source)->add(decodebin)->add(sink);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while adding: " << ex.what() << '\n';
        return 1;
    }

    // Link elements
    try {
        source->link(decodebin);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while linking: " << ex.what() << '\n';
    }

    // Handle messages posted on bus
    pipeline->get_bus()->add_watch([main_loop](const Glib::RefPtr<Gst::Bus>&,
        const Glib::RefPtr<Gst::Message>& message) {
            switch (message->get_message_type()) {
            case Gst::MESSAGE_EOS:
            case Gst::MESSAGE_ERROR:
                main_loop->quit();
                break;
            default:
                break;
            }
            return true;
        }
    );

    // Listen for newly created pads
    decodebin->signal_pad_added().connect(
        [decodebin, sink](const Glib::RefPtr<Gst::Pad>& pad) {
            std::cout << "New pad added to " << decodebin->get_name() << '\n';
            std::cout << "Pad name: " << pad->get_name() << '\n';

            auto ret = pad->link(sink->get_static_pad("sink"));

            if (ret != Gst::PAD_LINK_OK) {
                std::cout << "Cannot link pads. Error: " << ret << '\n';
            } else {
                std::cout << "Pads linked correctly! " << '\n';
            }
        }
    );

    // Start the pipeline
    pipeline->set_state(Gst::STATE_PLAYING);
    main_loop->run();
    pipeline->set_state(Gst::STATE_NULL);

    return 0;
}
