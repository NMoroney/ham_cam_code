// © 2024 : github.com/NMoroney
// MIT License 
//

#include <array>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>

#include <opencv2/opencv.hpp>

#include "anomaly_detection.hpp"    // https://github.com/ankane/AnomalyDetection.cpp


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


  std::vector<int> hcc_blueness_anomalies(const char* name) {

    using T = float;                       // anomaly_detection.hpp : vector of floats
    using Point2D = std::array<T, 2>;
    using Points  = std::vector<Point2D>;
    using std::cout;
    cout << "\n" << "hcc blueness anomalies :\n";
    
    auto capture = cv::VideoCapture(name);

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    std::vector<int> blues;
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {
        ++frame_count;

        auto ave = average_rgb<T>(frame);
        blues.push_back(ave.b);

        cout << frame_count << " " << std::flush;
     }
    }
    cout << "\n";

    std::ofstream ofs("temp_ave_b.tsv");
    ofs << "average blue\n";
    for (int i = 0; i < blues.size(); ++i) {
      ofs << blues[i] << "\n";
    }


    // compute +/- interval lengths (or start to end as plotted distances) of blue values
    //
    int s1 { (int)blues.size() - 1 };
    auto distance = [] (const int x1, const int y1, 
                        const int x2, const int y2) {
      T dx { (T)x1 - (T)x2 }, dy { (T)y1 - (T)y2 };
      return (T)sqrt((dx * dx) + (dy * dy));
    };

    int radius { 10 };
    std::vector<T> interval_lengths;
    for (int i = 0; i < blues.size(); ++i) {
      int i_lo { i - radius }, i_hi { i + radius };
      int y1, y2;
      if (i_lo <  0) { y1 = blues[0];     } else { y1 = blues[i_lo]; }
      if (i_hi > s1) { y2 = blues.back(); } else { y2 = blues[i_hi]; }
      T length = distance(i_lo, y1, i_hi, y2);
      interval_lengths.push_back(length);
    }

    std::ofstream ofs_ds("temp_interval_lengths.tsv");
    ofs_ds << "interval lengths\n";
    for (int i = 0; i < interval_lengths.size(); ++i) {
      ofs_ds << interval_lengths[i] << "\n";
    }

    T min_d { interval_lengths[0] }, max_d { min_d };
    for (int i = 1; i < interval_lengths.size(); ++i) {
      if (interval_lengths[i] < min_d) { min_d = interval_lengths[i]; }
      if (interval_lengths[i] > max_d) { max_d = interval_lengths[i]; }
    }
 

    // apply anomaly detection to the blue interval lengths
    //
    size_t period = 1;     // number of observations in a single period

    auto res = anomaly_detection::params().fit(interval_lengths, period);

    std::vector<T> anomalies(blues.size(), min_d);  

    std::vector<int> an;   // the anomaly index values

    for (auto anomaly : res.anomalies) {
      anomalies[anomaly] = max_d;
      an.push_back(anomaly);
    }

    std::ofstream ofs_anom("temp_anomalies.tsv");
    ofs_anom << "interval lengths\tanomalies\n";
    for (int i = 0; i < blues.size(); ++i) {
      ofs_anom << interval_lengths[i] << "\t" << anomalies[i] << "\n";
    }

    return an;
  }


  void hcc_extract_frames(const char* name,
                          const std::vector<int>& indices,
                          float scale_factor) {
    using std::cout;
    cout << "\n" << "hcc extract frames :\n";
    
    auto capture = cv::VideoCapture(name);
    std::set<int> frames;
    for (auto n : indices) {
      frames.insert(n);
    }

    int out_wide { (int)(scale_factor * capture.get(cv::CAP_PROP_FRAME_WIDTH))  },
        out_high { (int)(scale_factor * capture.get(cv::CAP_PROP_FRAME_HEIGHT)) };
    cv::Size out_size { out_wide, out_high };

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    std::vector<int> blues;
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {
        ++frame_count;
        if (frames.find(frame_count) != frames.end()) {
          cout << frame_count << " ";
          std::string fc { to_zero_padded(std::to_string(frame_count), pad_size) },
                      f  { "frame_" + fc + ".jpg" };

          cv::Mat scaled;
          cv::resize(frame, scaled, out_size, cv::INTER_LINEAR);

          cv::imwrite(f, scaled);
        }
      }
    }
    cout << "\n";

  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { 
    auto indices = n8m::hcc_blueness_anomalies(argv[1]);

    std::cout << "indices size : " << indices.size() << "\n";

    std::vector<int> ends { indices[0], indices[0] + 10,
                            indices.back() - 10, indices.back() };
    n8m::hcc_extract_frames(argv[1], ends, 1.0);
  }

  return 0;

}

