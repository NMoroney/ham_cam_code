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

  void hcc_time_columns(const char* name) {

    using std::cout;
    cout << "\n" << "hcc time gradients :\n";
    
    auto capture = cv::VideoCapture(name);

    auto length = capture.get(cv::CAP_PROP_FRAME_COUNT);
    cout << "length : " << length << "\n";

    auto wide = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    auto high = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "wide : " << wide << "\n"
         << "high : " << high << "\n";
    auto scale { high / wide };

    cout << "scale : " << scale << "\n";

    int out_wide { (int)length }, out_high { (int)(length * scale) };
    cv::Size out_size(out_wide, out_high);
    cout << "out wide : " << out_wide << "\n"
         << "out high : " << out_high << "\n";

    cv::Mat frame;
    cv::Mat merged(out_high, out_wide, CV_8UC3);
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {

        if (frame_count == 0) {
          cout << "frame cols : " << frame.cols << "\n"
               << "      rows : " << frame.rows << "\n";
        }
        ++frame_count;

        cv::Mat scaled;
        cv::resize(frame, scaled, out_size, cv::INTER_LINEAR);
        for (int y = 0; y < out_high; ++y) {
          int x = frame_count;
          cv::Vec3b& m_bgr = merged.at<cv::Vec3b>(y, x); 
          cv::Vec3b  s_bgr = scaled.at<cv::Vec3b>(y, x); 
          m_bgr[0] = s_bgr[0];
          m_bgr[1] = s_bgr[1];
          m_bgr[2] = s_bgr[2];
        }
      }
    }
    cout << "\n";

    imwrite("ham_cam_code-time_gradient-2024-01-27.jpg", merged);
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::hcc_time_columns(argv[1]); }

  return 0;

}

