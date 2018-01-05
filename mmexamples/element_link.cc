#include <gstreamermm.h>
#include <iostream>

int main(int argc, char *argv[]) {
    // Initialize GStreamermm
    Gst::init(argc, argv);

    // Create pipeline
    auto pipeline = Gst::Pipeline::create("my-pipeline");

    // Create elements
    auto element_source = Gst::ElementFactory::create_element("fakesrc");
    auto element_filter = Gst::ElementFactory::create_element("identity");
    auto element_sink = Gst::ElementFactory::create_element("fakesink");

    // We must add the elements to the pipeline before linking them
    try {
        pipeline->add(element_source)->add(element_filter)->add(element_sink);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while adding: " << ex.what() << '\n';
        return 1;
    }

    // Link the elements together:
    try {
        element_source->link(element_filter)->link(element_sink);
    } catch (const std::runtime_error& error) {
        std::cerr << "Exception while linking: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
