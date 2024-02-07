// © 2024 : github.com/NMoroney
// MIT License 
//

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

#include <armadillo>


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

  void hcc_kmeans_daylight(const char* name) {
    using T = double;
    using std::cout;
    cout << "\n" << "hcc kmeans daylight :\n";
    
    auto capture = cv::VideoCapture(name);

    auto length = capture.get(cv::CAP_PROP_FRAME_COUNT);

    arma::mat rgbs(3, length);

    cv::Mat frame;
    bool is_reading { true };
    int frame_count { 0 }, pad_size { 7 };
    while (is_reading) {
      capture.read(frame);
      if (frame.empty()) { 
        is_reading = false; 
      } else {

        auto ave = average_rgb<T>(frame);

        rgbs(0, frame_count) = ave.r;
        rgbs(1, frame_count) = ave.g;
        rgbs(2, frame_count) = ave.b;

        ++frame_count;

        cout << frame_count << " " << std::flush;
     }
    }
    cout << "\n";

    int k { 2 }, number_iterations { 10 };
    bool is_verbose { false };
    arma::mat means;
    bool result = kmeans(means, rgbs, k, arma::random_subset, number_iterations, is_verbose);

    if (!result) {
      cout << "clustering failed\n";
    } else {
      means.print("means:");
    }

    cout << "cluster 0 : "
         << means(0,0) << " " << means(1,0) << " " << means(2,0) << "\n"
         << "cluster 1 : "
         << means(0,1) << " " << means(1,1) << " " << means(2,1) << "\n";

    auto distance = [] (const arma::mat& means, const int i,
                        const arma::mat& rgbs,  const int j) {
      auto dx { means(0, i) - rgbs(0, j) },
           dy { means(1, i) - rgbs(1, j) },
           dz { means(2, i) - rgbs(2, j) };
      return sqrt((dx * dx) + (dy * dy) + (dz * dz));
    };

    std::vector<int> nearest;
    for (int i = 0; i < length; ++i) {
      auto d0 { distance(means, 0, rgbs, i) },
           d1 { distance(means, 1, rgbs, i) };
      int near = (d0 < d1 ? 0 : 1);
      nearest.push_back(near);
    }

    for (auto n : nearest) { cout << n << " "; }
    cout << "\n";

    std::vector<int> changes;
    int less_one { (int)nearest.size() - 1 };
    for (int i = 1; i < less_one; ++i) {
      if ((nearest[i] != nearest[i - 1]) ||
          (nearest[i] != nearest[i + 1])) {
        changes.push_back(i);
      }
    } 
    cout << "\n";

    hcc_extract_frames(name, changes, 0.25);

    for (auto i : changes) {
      int seconds { i * 100 },
          hours   { (int)((T)seconds / (T)3600) },
          minutes { (int)(((T)seconds - (T)(hours * 3600)) / (T)60) }; 
      cout << i << " - " << hours << ":" << minutes << "\n";
    }
  }

}

int main(int argc, char* argv[]) {

  if (argc == 2) { n8m::hcc_kmeans_daylight(argv[1]); }

  return 0;

}

