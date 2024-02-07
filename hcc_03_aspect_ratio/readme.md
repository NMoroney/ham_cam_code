
## Ham Cam : Aspect Ratio

If you are familiar with the Ham Cam daily videos, the aspect ratio of the saved frames in example one differs from the aspect ratio of the video as played in a viewer, such as QuickTime player.

Using OpenCV to get the corresponding capture property :

```cpp
    auto wide = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    auto high = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "wide : " << wide << "\n"
         << "high : " << high << "\n";
```

yields the following values :

```
wide : 818.526
high : 480
```

In comparison, getting the frame dimensions from a single frame :

```cpp
          cout << "frame cols : " << frame.cols << "\n"
               << "      rows : " << frame.rows << "\n";
```

yields these values :

```
frame cols : 640
      rows : 480
```

So if you really want the exported frames to match the video player displayed aspect ratio, the frames must be re-scaled.


