#include <gstreamermm.h>
#include <iostream>
#include <glibmm/main.h>

using namespace Gst;
using Glib::RefPtr;

class AllMediaPlayer {
private:
    RefPtr<Glib::MainLoop> main_loop;
    RefPtr<Pipeline> pipeline;

    RefPtr<Element> source;
    RefPtr<Element> decoder;

    bool on_bus_message(const RefPtr<Bus>&, const RefPtr<Message>& message) {
        switch (message->get_message_type()) {
        case MESSAGE_EOS:
            std::cout << "\nEnd of stream" << '\n';
            main_loop->quit();
            return false;
        case MESSAGE_ERROR:
            std::cerr << "Error."
                << RefPtr<MessageError>::cast_static(message)->parse_debug() << '\n';
            main_loop->quit();
            return false;
        default:
            break;
        }

        return true;
    }

    void on_decoder_pad_added(const RefPtr<Pad>& pad) {
        auto caps_format = pad->get_current_caps()->to_string().substr(0, 5);
        RefPtr<Bin> parent = parent.cast_dynamic(pad->get_parent()->get_parent());

        if (!parent) {
            std::cerr << "cannot get parent bin" << '\n';
            return;
        }

        Glib::ustring factory_name;

        if (caps_format == "video") {
            factory_name = "autovideosink";
        } else if (caps_format == "audio") {
            factory_name = "autoaudiosink";
        } else {
            std::cerr << "unsupported media type: " << pad->get_current_caps()->to_string() << '\n';
            return;
        }

        RefPtr<Element> element = ElementFactory::create_element(factory_name);
        if (!element) {
            std::cerr << "cannot create element " << factory_name << '\n';
            return;
        }

        try {
            parent->add(element);
            element->set_state(STATE_PLAYING);
            pad->link(element->get_static_pad("sink"));
        } catch (const std::runtime_error& err) {
            std::cerr << "cannot add element to a bin: " << err.what() << '\n';
        }
    }

    void init() {
        source = ElementFactory::create_element("filesrc");
        decoder = ElementFactory::create_element("decodebin");
        if (!decoder || !source) {
            throw std::runtime_error("One element could not be created.");
        }

        pipeline->add(source)->add(decoder);
        decoder->signal_pad_added().connect(
                sigc::mem_fun(*this, &AllMediaPlayer::on_decoder_pad_added));
        source->link(decoder);
    }

public:
    AllMediaPlayer() {
        main_loop = Glib::MainLoop::create();
        pipeline = Pipeline::create();
        pipeline->get_bus()->add_watch(
                sigc::mem_fun(*this, &AllMediaPlayer::on_bus_message));
    }

    void play_until_eos(const std::string& filename) {
        init();

        source->set_property("location", filename);
        pipeline->set_state(STATE_PLAYING);
        main_loop->run();
        pipeline->set_state(STATE_NULL);
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <multimedia filename>" << '\n';
        return 1;
    }

    init(argc, argv);
    AllMediaPlayer player;

    try {
        player.play_until_eos(argv[1]);
    } catch (const std::runtime_error& err) {
        std::cerr << "runtime error: " << err.what() << '\n';
    }
    return 0;
}
