# GStreamer Command Line Tools Examples
## 테스트 환경
- Jetson TX1/TX2
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
- H.264 decode -> H.264 encode (비디오만 처리)
    ```sh
    gst-launch-1.0 filesrc location="video.mp4" ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! omxh264dec ! nvvidconv ! 'video/x-raw(memory:NVMM), format=(string)I420' ! omxh264enc ! qtmux name=mux ! filesink location="video_out.mp4" demux.audio_0 ! queue ! aacparse ! mux.audio_0 -e
    ```
    > 1080p30f 3200k video 2x H.264 decode + 2x H.264 encode result
    ```sh
    RAM 2350/3983MB (lfb 17x4MB) cpu [40%,45%,32%,42%]@1734 EMC 55%@1600 APE 25 NVDEC 716 MSENC 716 GR3D 0%@76
    ```
