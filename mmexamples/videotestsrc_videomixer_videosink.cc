/*
    script
    gst-launch-1.0 -e -v videomixer name=mix \
        sink_0::xpos=0 sink_0::ypos=0 \
        sink_1::xpos=100 sink_1::ypos=100 \
        ! autovideosink \
        videotestsrc ! video/x-raw,width=100,height=100 ! mix.sink_0 \
        videotestsrc pattern=4 ! video/x-raw,width=200,height=200 ! mix.sink_1

    gst-launch-1.0 -e -v glvideomixer name=mix \
        sink_0::xpos=0 sink_0::ypos=0 \
        sink_1::xpos=100 sink_1::ypos=100 \
        ! glimagesink \
        videotestsrc ! video/x-raw,width=100,height=100 ! mix.sink_0 \
        videotestsrc pattern=4 ! video/x-raw,width=200,height=200 ! mix.sink_1
 */

#include <gstreamermm.h>
#include <iostream>
#include <glibmm/main.h>

class VideotestsrcVideomixerVideosinkBin {
private:
    Glib::RefPtr<Glib::MainLoop> main_loop;
    Glib::RefPtr<Gst::Pipeline> pipeline;
    Glib::RefPtr<Gst::Element> source1;
    Glib::RefPtr<Gst::Element> source2;
    Glib::RefPtr<Gst::Element> mixer;
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

    void init() {
        source1 = Gst::ElementFactory::create_element("videotestsrc");
        source2 = Gst::ElementFactory::create_element("videotestsrc");
#if 0
        mixer = Gst::ElementFactory::create_element("videomixer");
        sink = Gst::ElementFactory::create_element("autovideosink");
#else
        mixer = Gst::ElementFactory::create_element("glvideomixer");
        sink = Gst::ElementFactory::create_element("glimagesink");
#endif
        if (!sink || !mixer || !source2 || !source1) {
            throw std::runtime_error("One element could not be created.");
        }

        source1->set_property("pattern", 0);
        source2->set_property("pattern", 4);

        Glib::RefPtr<Gst::Caps> caps1 = Gst::Caps::create_from_string(
                "video/x-raw, width=(int)100, height=(int)100");
        Glib::RefPtr<Gst::Caps> caps2 = Gst::Caps::create_from_string(
                "video/x-raw, width=(int)200, height=(int)200");

        auto mixer_sink_0_pad_tpl = mixer->get_pad_template("sink_%u");
        auto mixer_sink_0_pad = mixer->request_pad(mixer_sink_0_pad_tpl);
        mixer_sink_0_pad->set_property("xpos", 0);
        mixer_sink_0_pad->set_property("ypos", 0);

        auto mixer_sink_1_pad_tpl = mixer->get_pad_template("sink_%u");
        auto mixer_sink_1_pad = mixer->request_pad(mixer_sink_1_pad_tpl);
        mixer_sink_1_pad->set_property("xpos", 100);
        mixer_sink_1_pad->set_property("ypos", 100);

        pipeline->add(source1)->add(source2)->add(mixer)->add(sink);
        // sink->signal_pad_added().connect(
        //         sigc::mem_fun(*this, &AllMediaPlayer::on_sink_pad_added));
        source1->link(mixer, caps1);
        source2->link(mixer, caps2);
        // source1->link_pads("src", mixer, "sink_0");
        // source2->link_pads("src", mixer, "sink_1");
        mixer->link(sink);
    }
public:
    VideotestsrcVideomixerVideosinkBin () {
        main_loop = Glib::MainLoop::create();
        pipeline = Gst::Pipeline::create();
        pipeline->get_bus()->add_watch(
                sigc::mem_fun(*this, &VideotestsrcVideomixerVideosinkBin::on_bus_message));
    }

    virtual ~VideotestsrcVideomixerVideosinkBin () {}

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
    VideotestsrcVideomixerVideosinkBin bin;

    try {
        bin.play_until_eos();
    } catch (const std::runtime_error& err) {
        std::cerr << "runtime error: " << err.what() << '\n';
    }
    return 0;
}
