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

  void ocv_sample_video_frames(const char* name) {

    using std::cout;
    cout << "\n" << "ocv sample video frames :\n";
    
    auto capture = cv::VideoCapture(name);

    float scale_factor { 0.15 };

    int out_wide { (int)(scale_factor * capture.get(cv::CAP_PROP_FRAME_WIDTH))  },
        out_high { (int)(scale_factor * capture.get(cv::CAP_PROP_FRAME_HEIGHT)) };
    cv::Size out_size { out_wide, out_high };

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 }, 
        j { 1 }, nth_frame { 10 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {
        ++frame_count;

        if ((frame_count % nth_frame) == 0) {
          std::string fc { to_zero_padded(std::to_string(j), pad_size) },
                      f  { "frame_" + fc + ".jpg" };

          cv::Mat scaled;
          cv::resize(frame, scaled, out_size, cv::INTER_LINEAR);

          cv::imwrite(f, scaled);
          cout << j << " " << std::flush;
          ++j;
        } 
      }
    }
    cout << "\n";

    cout << "ffmpeg -framerate 4 -pattern_type glob -i 'frame*.jpg' ham_cam_jam-03.gif\n";
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::ocv_sample_video_frames(argv[1]); }

  return 0;

}

