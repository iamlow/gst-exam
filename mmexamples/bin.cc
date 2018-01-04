/*
 * This example presents basic usage of the Gst::Bin class.
 */
#include <gstreamermm.h>
#include <iostream>

int main(int argc, char *argv[]) {
    Gst::init(argc, argv);

    // Create some elements
    auto fakesrc = Gst::ElementFactory::create_element("fakesrc", "source");
    auto fakesink = Gst::ElementFactory::create_element("fakesink", "sink");

    // Create empty Bin
    auto bin = Gst::Bin::create("my-bin");

    // Add elements to a bin
    try {
        bin->add(fakesrc)->add(fakesink);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Exception while adding: " << ex.what() << '\n';
        return 1;
    }

    // Some of the elements are actually a bins, so we can cast them.
    auto playbin = Gst::ElementFactory::create_element("playbin");
    // Glib::RefPtr<Gst::Bin> playbin_bin = playbin_bin.cast_static(playbin);
    auto playbin_bin = Glib::RefPtr<Gst::Bin>::cast_static(playbin);

    if (!playbin_bin) {
        std::cerr << "Cannot find playbin element" << '\n';
        return 1;
    }

    // We can also iterate through the elements in the bin
    auto it = playbin_bin->iterate_recurse();
    std::cout << "List of elements in the contailer: " << '\n';
    while (it.next()) {
        std::cout << " * " << it->get_name() << '\n';
    }

    // We can also connect to signals emitted by bin, e.g. when the element has been removed.
    bin->signal_element_removed().connect(
        [] (const Glib::RefPtr<Gst::Element>& removed_element) {
            std::cout << "Element '" << removed_element->get_name()
                    << "' has been removed from the bin" << '\n';
        }
    );

    bin->remove(fakesink);
    std::cout << "returning from application..." << '\n';
    return 0;
}
