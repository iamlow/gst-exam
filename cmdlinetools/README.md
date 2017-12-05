# GStreamer Command Line Tools Examples
## 테스트 환경
- Jetson TX1/TX2
- 영상: 1080p30f 3Mbps H.264 encoded video
## Reference
- [Jetson TX1 Accelerated Gstreamer User Guide](http://developer2.download.nvidia.com/embedded/L4T/r28_Release_v1.0/Docs/Jetson_TX1_Accelerated_GStreamer_User_Guide.pdf)

## 플러그인 확인
```sh
gst-inspect-1.0
```
## Play (모니터 출력/비디오만 처리)
- 파일읽기 -> 디먹싱 -> 파싱 -> 디코더 -> 비디오 렌터링(GPU)
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nveglglessink -e
    ```
    > sink: nveglglessink EGL/GLES videosink element -> using GPU

- 디코드빈을 사용하여 간단하게 렌터링
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! decodebin ! nveglglessink -e
    ```
- GPU 사용하지 않고 비디오 렌더링(NO GPU)
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvoverlaysink -e
    ```
    > sink: nvoverlaysink OpenMAX IL videosink element -> GPU 0%
- 자동으로 sink 알맞게 설정
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! decodebin ! autovideosink -e
    ```
## Transcoding
- H.264 decode -> H.264 encode (video only, audio by pass)
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw(memory:NVMM), format=(string)I420' ! omxh264enc ! qtmux name=mux ! filesink location="video_out.mp4" demux.audio_0 ! queue ! aacparse ! mux.audio_0 -e
    ```
    > 1080p30f 3200k video 2x H.264 decode + 2x H.264 encode result
    ```sh
    RAM 2350/3983MB (lfb 17x4MB) cpu [40%,45%,32%,42%]@1734 EMC 55%@1600 APE 25 NVDEC 716 MSENC 716 GR3D 0%@76
    ```
## Scale (H/W)
- 해상도 변경
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=1280, height=720' ! nveglglessink -e
    ```
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=1280, height=720, format=(string)NV12' ! nveglglessink -e
    ```
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=1280, height=720, format=(string)I420' ! nveglglessink -e
    ```
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=1280, height=720, format=(string)RGBA' ! nveglglessink -e
    ```
- NVMM (CPU 점유율이 낮음)
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1280, height=720' ! nveglglessink -e
    ```
## Videomixer (S/W using CPU)
- 2 videos compositing(overlay)
    ```sh
    gst-launch-1.0 -v videotestsrc name=tstsrc0 is-live=true ! video/x-raw,width=1280,height=720,framerate=30/1 ! videomixer name=mix ! videoconvert ! nveglglessink sync=false videotestsrc name=tstsrc1 pattern=ball background-color=0 ! mix.
    ```
    ```sh
    gst-launch-1.0 -e videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=0 sink_1::xpos=100 sink_1::ypos=50 ! nveglglessink videotestsrc ! video/x-raw,width=600,height=200 ! mix.sink_0 videotestsrc pattern=0 ! video/x-raw,width=100,height=100 ! mix.sink_1
    ```
- 3 videos compositing(overlay)
    ```sh
    gst-launch-1.0 -e videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=50 sink_1::ypos=50 sink_2::xpos=200 sink_2::ypos=50 ! nveglglessink videotestsrc ! video/x-raw,width=1280,height=720 ! mix.sink_0 videotestsrc ! video/x-raw,width=100,height=100 ! mix.sink_1 videotestsrc ! video/x-raw,width=100,height=100 ! mix.sink_2
    ```
    > videomixer는 CPU를 사용하는 S/W 합성이라 TX1에서 성능이슈 발생
    > CPU 100% 소모하며 초딩 30f 영상합성 불가
    > GPU를 사용하는 glvideomixer를 사용해야 처리 가능

## glvideomixer (using GPU)
> gstreamer1.0-plugins-bad 필요
    ```sh
    sudo apt-get install  gstreamer1.0-plugins-bad
    ```
    - 3 videos compositing(overlay)
    ```sh
    $ gst-launch-1.0 -e glvideomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=0 sink_1::ypos=240 sink_2::xpos=1280 sink_2::ypos=240 ! autovideosink filesrc location="video1.mp4" ! qtdemux name=demux_0 demux_0.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw' ! tee ! queue ! mix.sink_0 filesrc location="video2.mp4" ! qtdemux name=demux_1 demux_1.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=1280, height=720' ! tee ! queue ! mix.sink_1 filesrc location="video3.mp4" ! qtdemux name=demux_2 demux_2.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw, width=640, height=360' ! tee ! queue ! mix.sink_2 audiomixer name=amix ! audioconvert ! voaacenc ! mux.audio demux_1.audio_0 ! queue ! avdec_aac ! audioconvert ! amix.sink_0 demux_2.audio_0 ! queue ! avdec_aac ! audioconvert ! amix.sink_1 demux_0.audio_0 ! queue ! avdec_aac ! audioconvert ! amix.sink_2
    ```
## rtmpsink
> gstreamer1.0-plugins-bad 필요
```sh
sudo apt-get install  gstreamer1.0-plugins-bad
```
