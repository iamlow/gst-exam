#include <gstreamermm.h>
#include <iostream>
#include <glibmm/main.h>

using namespace Gst;
using Glib::RefPtr;

RefPtr<Element> video_parser, audio_parser;
RefPtr<Glib::MainLoop> main_loop;

bool on_bus_message(const RefPtr<Gst::Bus>&, const RefPtr<Message>& message) {
    switch(message->get_message_type()) {
    case MESSAGE_EOS:
        std::cout << "\nEnd of stream" << '\n';
        main_loop->quit();
        return false;
    case MESSAGE_ERROR: {
        Glib::Error error;
        std::string debug_message;
        RefPtr<MessageError>::cast_static(message)->parse(error, debug_message);
        std::cerr << "Error: " << error.what() << std::endl << debug_message << '\n';
        main_loop->quit();
        return false;
    }
    default:
        break;
    }

    return true;
}

void on_demux_pad_added(const RefPtr<Pad>& newPad) {
    std::cout << "Dynamic pad created. Linking demuxer/decoder." << '\n';
    RefPtr<Pad> sinkpad = video_parser->get_static_pad("sink");
    PadLinkReturn ret = newPad->link(sinkpad);

    if (ret != PAD_LINK_OK && ret != PAD_LINK_WAS_LINKED) {
        std::cerr << "Linking of pads " << newPad->get_name() << " and "
                << sinkpad->get_name() << " failed." << '\n';
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <Ogg/Vorbis filename>"
                << " <mp3 filename> <mkv output filename> "<< '\n';
        return 1;
    }

    init(argc, argv);

    auto pipeline = Pipeline::create("play-pipeline");
    main_loop = Glib::MainLoop::create();
    RefPtr<Bus> bus = pipeline->get_bus();
    bus->add_watch(sigc::ptr_fun(&on_bus_message));

    auto video_source = ElementFactory::create_element("filesrc");
    auto audio_source = ElementFactory::create_element("filesrc");
    auto filesink = ElementFactory::create_element("filesink");

    auto ogg_demuxer = ElementFactory::create_element("oggdemux");
    auto audio_parser = ElementFactory::create_element("mad");

    auto audiosink = ElementFactory::create_element("autoaudiosink");
    auto videosink = ElementFactory::create_element("autovideosink");

    auto muxer = ElementFactory::create_element("matroskamux");
    auto video_parse = ElementFactory::create_element("theoraparse");

    if (!video_source || !ogg_demuxer || !video_parser || !videosink
            || !pipeline || !audio_parser || !audio_source) {
        std::cout << "One element could not be created." << std::endl;
        return 1;
    }

    video_source->set_property<Glib::ustring>("location", argv[1]);
    audio_source->set_property<Glib::ustring>("location", argv[2]);
    filesink->set_property<Glib::ustring>("location", argv[3]);

    pipeline->add(video_source)->add(ogg_demuxer)->add(video_parser)
            ->add(audio_source)->add(audio_parser)->add(muxer)->add(filesink);

    ogg_demuxer->signal_pad_added().connect(sigc::ptr_fun(&on_demux_pad_added));

    video_source->link(ogg_demuxer);
    video_parser->link(muxer);
    audio_source->link(audio_parser)->link(muxer);
    muxer->link(filesink);

    pipeline->set_state(STATE_PLAYING);
    main_loop->run();
    pipeline->set_state(STATE_NULL);

    return 0;
}
