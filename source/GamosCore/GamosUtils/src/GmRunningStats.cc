#include <iostream>
#include <cmath>

class RunningStats {
public:
    RunningStats() : n(0), mean(0.0), M2(0.0) {}

    // Añade un nuevo valor y actualiza las estadísticas
    void add(double x) {
        n++;
        double delta = x - mean;
        mean += delta / n;
        double delta2 = x - mean;
        M2 += delta * delta2;
    }

    int count() const {
        return n;
    }

    double get_mean() const {
        return (n > 0) ? mean : 0.0;
    }

    double variance() const {
        return (n > 1) ? M2 / (n - 1) : 0.0;  // varianza muestral
    }

    double std_dev() const {
        return std::sqrt(variance());
    }

private:
    int n;       // número de valores leídos
    double mean; // media de los valores
    double M2;   // sumatoria de las diferencias al cuadrado
};

int main() {
    RunningStats stats;

    // Ejemplo de lectura de datos
    double value;
    std::cout << "Introduce valores (introduce un valor no numérico para terminar):" << std::endl;
    while (std::cin >> value) {
        stats.add(value);
    }

    std::cout << "Número de valores: " << stats.count() << std::endl;
    std::cout << "Media: " << stats.get_mean() << std::endl;
    std::cout << "Desviación estándar: " << stats.std_dev() << std::endl;

    return 0;
}
