#include <iostream>
#include <fstream>
#include "cluster.h"
#include <chrono>

// печатает количество итераций для конкретного k, записывает в OUTPUT результаты
void overall_k(const std::vector<Point>& points, int k, std::ofstream& out) {
    auto *ptr = new Cluster[k];
    int iter = Start(k, ptr, points);
    std::cout << "итоговое к-во кластеров: " << k << ", к-во итераций: " << iter << std::endl;
    int m = 0;
    for (int i = 0; i < k; ++i) {
        int s = ptr[i].cluster_points.size();
        for (int j = 0; j < s; ++j) {
            double x_ = ptr[i].cluster_points[j].x;
            double y_ = ptr[i].cluster_points[j].y;
            int cluster_id_ = i;

            out << ++m << " " << x_ << " "
                << y_ << " " << cluster_id_ << std::endl;
        }
    }
    delete[] ptr;
}

// метод локтей, перебирает k в некотором интервале, рассчитывает total within-cluster sum of square
void elbow_method(const std::vector<Point>& points, int k_min, int k_max) {

    std::vector<int> WSS(k_max, 0);

    for (int k = k_min; k < k_max + 1; ++k) {
        auto *ptr = new Cluster[k];
        int iter = Start(k, ptr, points);
        std::cout << "к-во кластеров: " << k << ", к-во итераций: " << iter << std::endl;
        double current_sse = 0;

        for (int i = 0; i < k; ++i) {
            int s = ptr[i].cluster_points.size();
            for (int j = 0; j < s; ++j) {
                double x_ = ptr[i].cluster_points[j].x;
                double y_ = ptr[i].cluster_points[j].y;
                current_sse += pow((double) (x_ - ptr[i].curX), 2)
                               + pow((double) (y_ - ptr[i].curY), 2);
            }
        }
        WSS[k - 1] = current_sse;
        delete[] ptr;
    }
    std::ofstream out_wss("WSS.txt");
    for (int i = 0; i < k_max; ++i) {
        out_wss << i + 1 << " " << WSS[i] << std::endl;
    }
}

// метод силуэтов, перебирает k в некотором интервале, рассчитывает total within-cluster sum of square
void silhouette_method(const std::vector<Point>& points, int k_min, int k_max) {

    // std::vector<double > SS(k_max - k_min + 1, 0);
    std::vector<double> SS;
    std::ofstream out_ss("SS(k).txt");

    for (int k = k_min; k < k_max + 1; ++k) {
        std::cout << k << std::endl;
        auto *ptr = new Cluster[k];
        Start(k, ptr, points);
        double ss = 0; // среднее s_i для данного набора кластеров

        for (int i = 0; i < k; ++i) {
            int I = ptr[i].cluster_points.size();
            // if (I == 0) { continue; }
            for (int j = 0; j < I; ++j) { // i-ый кластер и j-ая в нем точка
                double a = 0.; // similarity
                double b = 0.; // dissimilarity
                auto b_min = double(-10);

                for (int l = 0; l < k; ++l) { // l-ый кластер
                    int J = ptr[l].cluster_points.size();

                    // ОБЪЕДЕНИТЬ !!!!!

                    if (i == l) { // если точки с одного кластера
                        for (int m = 0; m < J; ++m) {
                            a += sqrt(pow((double) (ptr[i].cluster_points[j].x - ptr[l].cluster_points[m].x), 2)
                                      +
                                      pow((double) (ptr[i].cluster_points[j].y - ptr[l].cluster_points[m].y), 2));
                        }
                        a = I == 1 ? 0 : a / ((double)I - 1);

                    } else { // если точки с разных кластеров
                        for (int m = 0; m < J; ++m) { // l-ый кластер и m-ая в нем точка
                            b += sqrt(pow((double) (ptr[i].cluster_points[j].x - ptr[l].cluster_points[m].x), 2)
                                      +
                                      pow((double) (ptr[i].cluster_points[j].y - ptr[l].cluster_points[m].y), 2));
                        }
                        b = J == 0 ? 0 : b / ((double)J); // J тоже может быть нулевым! хотя на картинке этого не видели
                        b_min = b_min == -10 ? b : std::min(b_min, b);
                    }
                }
                double temp = (std::max(a, b_min));
                if (temp != 0) { ss += (b_min - a) / temp; } // мы в среднее добавляем s_i каждой точки
            }
        }
        ss /= points.size();
        SS.push_back(ss);
        out_ss << k << " " << ss << std::endl;
        delete[] ptr;
    }
}

int main() {
    std::string s = "second";
    std::ifstream in("INPUT_0.5.txt");
    std::ofstream out("OUTPUT.txt");
    std::vector<Point> points;
    double x, y;
    int id = 0;
    while (!in.eof()) {
        in >> x >> y;
        points.emplace_back(Point(id, x, y, 0));
        ++id;
    }

    auto start1 = std::chrono::steady_clock::now();
    //elbow_method(points, 1, 30);
    auto end1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
    std::cerr << std::endl << "elbow_method time: " << elapsed_seconds1.count() << "s\n\n";

    auto start2 = std::chrono::steady_clock::now();
    //silhouette_method(points, 2, 30);
    auto end2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
    std::cerr << std::endl << "silhouette_method time: " << elapsed_seconds2.count() << "s\n";

    overall_k(points, 4, out);

    return 0;
}
//