/*
 * This example presents how to use Gst::ElementFactory class, getting
 * information and create specific element.
 */
#include <gstreamermm.h>
#include <iostream>

int main(int argc, char *argv[]) {
    Gst::init(argc, argv);

    // Get list of all primary demuxers in the system
    std::cout << "List of primary demuxers: " << '\n';
    for (auto factory : Gst::ElementFactory::get_elements(
            Gst::ELEMENT_FACTORY_TYPE_DEMUXER, Gst::RANK_PRIMARY)) {
        std::cout << " * " << factory->get_name() << '\n';
    }
    std::cout << '\n';

    // Get list of all primary sinks in the system
    std::cout << "List of primary sinks: " << '\n';
    for (auto factory : Gst::ElementFactory::get_elements(
            Gst::ELEMENT_FACTORY_TYPE_SINK, Gst::RANK_NONE)) {
        std::cout << " * " << factory->get_name() << '\n';
    }
    std::cout << '\n';


    auto fakesrc_factory = Gst::ElementFactory::find("fakesrc");
    if (!fakesrc_factory) {
        std::cerr << "Failed to find factory of type 'fakesrc'" << '\n';
        return -1;
    }

    // Read information about an author of the elements
    std::cout << "Author of the element '" << fakesrc_factory->get_name()
            << "' is " << fakesrc_factory->get_metadata(GST_ELEMENT_METADATA_AUTHOR)
            << "\n\n";

    // Read all available information about the element
    std::cout << "All information about element '" << fakesrc_factory->get_name()
            << "':"<< '\n';
    for (auto metadata_key : fakesrc_factory->get_metadata_keys()) {
        std::cout << " * " << metadata_key << ": "
                << fakesrc_factory->get_metadata(metadata_key) << '\n';
    }

    // Create element fakesrc

    // Method 1
    {
        auto fakesrc = fakesrc_factory->create("source");
        if (!fakesrc) {
            std::cerr << "Failed to create element of type 'fakesrc'" << '\n';
            return -1;
        }
    }

    // Method 2
    {
        auto fakesrc = Gst::ElementFactory::create_element("fakesrc", "source");
        if (!fakesrc) {
            std::cerr << "Failed to create element of type 'fakesrc'" << '\n';
            return -1;
        }
    }

    return 0;
}
