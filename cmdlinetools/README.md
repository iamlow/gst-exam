# GStreamer Command Line Tools Examples
## 테스트 환경
- Jetson TX1/TX2
## Reference
- [Jetson TX1 Accelerated Gstreamer User Guide](http://developer2.download.nvidia.com/embedded/L4T/r28_Release_v1.0/Docs/Jetson_TX1_Accelerated_GStreamer_User_Guide.pdf)

## 플러그인 확인
```sh
gst-inspect-1.0
```
## Play
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
