#include <gstreamermm.h>
#include <iostream>
#include <glibmm/main.h>

class NvcamRtmpBin {
    Glib::RefPtr<Glib::MainLoop> main_loop;
    Glib::RefPtr<Gst::Pipeline> pipeline;
    Glib::RefPtr<Gst::Element> source;
    Glib::RefPtr<Gst::Element> sink;

    bool on_bus_message(const Glib::RefPtr<Gst::Bus>&, const Glib::RefPtr<Gst::Message>& message) {
        switch (message->get_message_type()) {
        case Gst::MESSAGE_EOS:
            std::cout << "\nEnd of stream" << '\n';
            main_loop->quit();
            return false;
        case Gst::MESSAGE_ERROR:
            std::cerr << "Error."
                << Glib::RefPtr<Gst::MessageError>::cast_static(message)->parse_debug() << '\n';
            main_loop->quit();
            return false;
        default:
            break;
        }

        return true;
    }

    // void on_sink_pad_added(const Glib::RefPtr<Gst::Pad>& pad) {
    //     auto caps_format = pad->get_current_caps()->to_string().substr(0, 5);
    //     Glib::RefPtr<Gst::Bin> parent = parent.cast_dynamic(pad->get_parent()->get_parent());
    //
    //     if (!parent) {
    //         std::cerr << "cannot get parent bin" << '\n';
    //         return;
    //     }
    //
    //     Glib::ustring factory_name;
    //
    //     if (caps_format == "video") {
    //         factory_name = "autovideosink";
    //     } else if (caps_format == "audio") {
    //         factory_name = "autoaudiosink";
    //     } else {
    //         std::cerr << "unsupported media type: " << pad->get_current_caps()->to_string() << '\n';
    //         return;
    //     }
    //
    //     Glib::RefPtr<Gst::Element> element = Gst::ElementFactory::create_element(factory_name);
    //     if (!element) {
    //         std::cerr << "cannot create element " << factory_name << '\n';
    //         return;
    //     }
    //
    //     try {
    //         parent->add(element);
    //         element->set_state(Gst::STATE_PLAYING);
    //         pad->link(element->get_static_pad("sink"));
    //     } catch (const std::runtime_error& err) {
    //         std::cerr << "cannot add element to a bin: " << err.what() << '\n';
    //     }
    // }

    void init() {
        source = Gst::ElementFactory::create_element("wrappercamerabinsrc");
        sink = Gst::ElementFactory::create_element("autovideosink");
        if (!sink || !source) {
            throw std::runtime_error("One element could not be created.");
        }

        pipeline->add(source)->add(sink);
        // sink->signal_pad_added().connect(
        //         sigc::mem_fun(*this, &AllMediaPlayer::on_sink_pad_added));
        source->link(sink);
    }

public:
    NvcamRtmpBin() {
        main_loop = Glib::MainLoop::create();
        pipeline = Gst::Pipeline::create();
        pipeline->get_bus()->add_watch(
                sigc::mem_fun(*this, &NvcamRtmpBin::on_bus_message));
    }

    void play_until_eos() {
        init();

        // source->set_property("location", filename);
        pipeline->set_state(Gst::STATE_PLAYING);
        main_loop->run();
        pipeline->set_state(Gst::STATE_NULL);
    }
};

int main(int argc, char *argv[]) {
    Gst::init(argc, argv);
    NvcamRtmpBin nvcam_rtmp_bin;

    try {
        nvcam_rtmp_bin.play_until_eos();
    } catch (const std::runtime_error& err) {
        std::cerr << "runtime error: " << err.what() << '\n';
    }
    return 0;
}
