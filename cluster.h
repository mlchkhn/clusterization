#include <vector>
#include <cmath>

struct Point {
    Point(int id, double x, double y, int cluster_id) : id(id), x(x), y(y), cluster_id(cluster_id) {}
    int id;
    double x;
    double y;
    int cluster_id = 0;
};

class Cluster{ // конкретный кластер
public:
    std::vector<Point> cluster_points; // точки этого кластера
    double curX , curY; // координаты текущего центроида
    double lastX, lastY; // координаты предыдущего центроида
    size_t Size() { return cluster_points.size(); } // получаем размер вектора
    inline void Add(const Point& pt){ cluster_points.push_back(pt); } // Добавляем пиксель к кластеру
    void SetCenter();
    friend void Bind(int k, Cluster * cluster_array, const std::vector<Point>& points);
    friend void InitialCenter(int k, Cluster * cluster_array , const std::vector<Point>& vpt);
    friend int Start(int k, Cluster * cluster_array, const std::vector<Point>& points);
};

// uniform distribution over a vector
void
InitialCenter(int k, Cluster * cluster_array, const std::vector<Point>& vpt)
{
    int size = vpt.size();
    int step = size/k;
    int steper = 0;

    for (int i = 0; i < k; i++, steper += step) {
        cluster_array[i].curX = vpt[steper].x;
        cluster_array[i].curY = vpt[steper].y;
    }
}

// Recalculate means (centers) for observations assigned to each cluster
void
Cluster::SetCenter()
{
    double sumX = 0, sumY = 0;
    int size = Size();
    for (int i = 0; i < size; sumX += cluster_points[i].x, ++i); // the centers of mass by x
    for (int i = 0; i < size; sumY += cluster_points[i].y, ++i); // the centers of mass by y
    lastX = curX;
    lastY = curY;
    curX = sumX / size;
    curY = sumY / size;
}

// binding points to centers
void
Bind(int k, Cluster * cluster_array, const std::vector<Point>& points)
{
    for (int j = 0; j < k; ++j)
        cluster_array[j].cluster_points.clear(); // Чистим кластер перед использованием
    int size = points.size();
    for (int i = 0; i < size; ++i) { // Запускаем цикл по всем пикселям множества
        double min = pow((double)cluster_array[0].curX - points[i].x, 2)
                    + pow((double)cluster_array[0].curY - points[i].y, 2);
        Cluster * cl = &cluster_array[0];
        for (int j = 1; j < k; j++) {
            double tmp = pow((double)cluster_array[j].curX - points[i].x, 2)
                        + pow((double)cluster_array[j].curY - points[i].y, 2);
            if (tmp < min) { // Ищем близлежащий кластер
                min = tmp;
                cl = &cluster_array[j];
            }
        }
        cl->Add(points[i]); // Добавляем в близ лежащий кластер текущий пиксель
    }
}

int
Start(int k, Cluster * cluster_array, const std::vector<Point>& points)
{
    InitialCenter(k, cluster_array, points);
    int iterations = 0;
    for( ; iterations < 100; ++iterations) { // Запускаем основной цикл
        int chk = 0;
        Bind(k, cluster_array, points); // Связываем точки с кластерами
        for (int j = 0; j < k; ++j) // Высчитываем новые координаты центров
            cluster_array[j].SetCenter();
        for (int p = 0; p < k; ++p) { // Проверяем не совпадают ли они с предыдущими цент-ми
            if (cluster_array[p].curX == cluster_array[p].lastX && cluster_array[p].curY == cluster_array[p].lastY)
                ++chk;
        }
        if (chk == k) break; // Если да выходим с цикла
    }
    return iterations;
}