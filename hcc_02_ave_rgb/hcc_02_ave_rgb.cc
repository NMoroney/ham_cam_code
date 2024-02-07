// © 2024 : github.com/NMoroney
// MIT License 
//

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>


namespace n8m {

  std::string to_zero_padded(const std::string& s, const int char_wide) {
    std::string padded;
    for (int i = s.size(); i < char_wide; ++i) {
      padded += "0";
    }
    return padded + s;
  }

  template <typename T>
  auto average_rgb(const cv::Mat& img) {
    T sum_r { 0 }, sum_g { 0 }, sum_b { 0 },
      n { (T)(img.rows * img.cols) };

    struct ave { int r; int g; int b; };

    for (int r = 0; r < img.rows; ++r) {
      for (int c = 0; c < img.cols; ++c) {
        auto bgr = img.at<cv::Vec3b>(r, c);
        sum_b += (T)bgr[0];
        sum_g += (T)bgr[1];
        sum_r += (T)bgr[2];
      }
    }

    return ave { (int)(sum_r / n), (int)(sum_g / n), (int)(sum_b / n) };
  }

  void ocv_read_video_frames(const char* name) {
    using T = double;
    using std::cout;
    cout << "\n" << "ocv read video frames :\n";
    
    auto capture = cv::VideoCapture(name);

    std::ofstream ofs_one("temp_ave_rgb.tsv");
    ofs_one << "frame count\tave red\tave green\tave blue\n";

    std::ofstream ofs_two("temp_xy_rgb.tsv");
    ofs_two << "frame_count\tave rgb\tred\tgreen\tblue\n";

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {
        ++frame_count;

        auto ave = average_rgb<T>(frame);

        ofs_one << frame_count << "\t"
                << ave.r << "\t" << ave.g << "\t" << ave.b << "\n";

        ofs_two << frame_count << "\t"
                << (T)(ave.r + ave.g + ave.b)/(T)3 << "\t"
                << ave.r << "\t" << ave.g << "\t" << ave.b << "\n";
        
        cout << frame_count << " " << std::flush;
     }
    }
    cout << "\n";
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::ocv_read_video_frames(argv[1]); }

  return 0;

}

