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

  void frame_aspect_ratio(const char* name) {

    using std::cout;
    cout << "\n" << "frame aspect ratio :\n";
    
    auto capture = cv::VideoCapture(name);

    auto length = capture.get(cv::CAP_PROP_FRAME_COUNT);
    cout << "length : " << length << "\n";

    auto wide = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    auto high = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "wide : " << wide << "\n"
         << "high : " << high << "\n";
    auto hw { high / wide }, wh { wide / high };

    cout << "hw : " << hw << "\n"
         << "wh : " << wh << "\n";

    int out_wide { (int)wide }, out_high { (int)high };
    cv::Size out_size(out_wide, out_high);
    cout << "out wide : " << out_wide << "\n"
         << "out high : " << out_high << "\n";

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {

        if (frame_count == 0) {
          cout << "frame cols : " << frame.cols << "\n"
               << "      rows : " << frame.rows << "\n";

          cv::Mat scaled;
          cv::resize(frame, scaled, out_size, cv::INTER_LINEAR);
          imwrite("temp_scaled_01.jpg", scaled);

          imwrite("temp_frame_01.jpg", frame);
        }
        ++frame_count;

      }
    }
    cout << "\n";
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::frame_aspect_ratio(argv[1]); }

  return 0;

}

