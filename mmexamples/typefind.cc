#include <gstreamermm.h>
#include <glibmm/main.h>
#include <glibmm/exceptionhandler.h>
#include <iostream>

Glib::RefPtr<Glib::MainLoop> mainloop;
bool have_type = false;

static bool on_structure_foreach(const Glib::QueryQuark& id, const Glib::ValueBase& value) {
    const Glib::ustring str_id = id;
    gchar* str_value = g_strdup_value_contents(value.gobj());
    std::cout << "Structure field: id=" << str_id << ", value=" << str_value << '\n';
    g_free(str_value);

    return true;
}

static void on_typefind_have_type(guint probability, const Glib::RefPtr<Gst::Caps>& caps) {
    have_type = true;

    std::cout << "have-type: probability=" << probability << '\n';

    if (!caps) {
        std::cerr << "on_typefind_have_type(): caps is null" << '\n';
        return;
    }

    if (!caps->size()) {
        std::cerr << "on_typefind_have_type(): caps is empty" << '\n';
        return;
    }

    Gst::Structure structure = caps->get_structure(0);
    const Glib::ustring mime_type = structure.get_name();
    std::cout << "have-type: mime_type=" << mime_type << '\n';

    structure.foreach(sigc::ptr_fun(&on_structure_foreach));

    if (mainloop) {
        mainloop->quit();
    } else {
        std::cerr << "on_typefind_have_type(): mainloop is null" << '\n';
    }
}

// #ifdef GSTREAMERMM_DISABLE_DEPRECATED

#include <gst/gsttypefind.h>

static void TypeFindElement_signal_have_type_callback(
        struct _GstTypeFindElement* self, guint p0, GstCaps* p1, void* data) {
    using SlotType = sigc::slot< void, guint, const Glib::RefPtr<Gst::Caps>& >;
    auto obj = dynamic_cast<Gst::Element*>(Glib::ObjectBase::_get_current_wrapper((GObject*)self));
    if (obj) {
        try {
            if (const auto slot = Glib::SignalProxyNormal::data_to_slot(data))
                (*static_cast<SlotType*>(slot))(p0, Glib::wrap(p1, true));
        } catch(...) {
            Glib::exception_handlers_invoke();
        }
    }
}

static const Glib::SignalProxyInfo TypeFindElement_signal_have_type_info = {
    "have-type",
    (GCallback) &TypeFindElement_signal_have_type_callback,
    (GCallback) &TypeFindElement_signal_have_type_callback
};
// #endif

int main(int argc, char *argv[]) {
    // Initialize Gstreamermm:
    Gst::init(argc, argv);

    // Check input arguements:
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << '\n';
        return 1;
    }

    const std::string filename = argv[1];

    // Create pipeline:
    auto pipeline = Gst::Pipeline::create("my-pipeline");

    // Create elements:
    auto element_source = Gst::ElementFactory::create_element("filesrc");
    element_source->set_property("location", filename);
    auto element_sink = Gst::ElementFactory::create_element("fakesink");
    auto element_typefind = Gst::ElementFactory::create_element("typefind");

    Glib::SignalProxy<void, guint, const Glib::RefPtr<Gst::Caps>& >
        proxy(element_typefind.operator->(), &TypeFindElement_signal_have_type_info);
    proxy.connect(sigc::ptr_fun(&on_typefind_have_type));

    // We must add the elements to the pipeline before linking them:
    try
    {
      pipeline->add(element_source)->add(element_typefind)->add(element_sink);
      //pipeline->add(element_source)->add(element_id3demux)->add(element_typefind)->add(element_sink);
    }
    catch (std::runtime_error& ex)
    {
      std::cerr << "Exception while adding: " << ex.what() << std::endl;
      return 1;
    }

    // Link the elements together:
    try
    {
      //element_source->link(element_id3demux)->link(element_typefind)->link(element_sink);
      element_source->link(element_typefind)->link(element_sink);
    }
    catch(const std::runtime_error& error)
    {
      std::cerr << "Exception while linking: " << error.what() << std::endl;
      return 1;
    }

    // Now set the while pipeline to playing and start the main loop:
    std::cout << "Setting pipeline to PLAYING." << '\n';
    mainloop = Glib::MainLoop::create();
    pipeline->set_state(Gst::STATE_PLAYING);
    std::cout << "Pipeline is playing." << std::endl;
    if (!have_type) {
        mainloop->run();
    }

    // Clean up nicely.
    std::cout << "Returned. Stopping pipeline." << '\n';
    pipeline->set_state(Gst::STATE_NULL);

    return 0;
}
