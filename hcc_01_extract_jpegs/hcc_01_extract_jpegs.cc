// © 2024 : github.com/NMoroney
// MIT License 
//

#include <iostream>

#include <opencv2/opencv.hpp>


namespace n8m {

  std::string to_zero_padded(const std::string& s, const int char_wide) {
    std::string padded;
    for (int i = s.size(); i < char_wide; ++i) {
      padded += "0";
    }
    return padded + s;
  }

  void read_video_frames(const char* name) {

    using std::cout;
    cout << "\n" << "read video frames :\n";
    
    auto capture = cv::VideoCapture(name);

    double fps = capture.get(cv::CAP_PROP_FPS);
    cout << "fps : " << fps << "\n";

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {
        ++frame_count;
        std::string fc { to_zero_padded(std::to_string(frame_count), pad_size) },
                    f  { "frame_" + fc + ".jpg" };
        cv::imwrite(f, frame);
        cout << frame_count << " " << std::flush;
     }
    }
    cout << "\n";
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::read_video_frames(argv[1]); }

  return 0;

}

