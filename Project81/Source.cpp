#include <iostream>
#include <random>
#include <limits>

class GrayscaleImage {
public:
    GrayscaleImage(int width, int height, int fillOption, int fillValue1, int fillValue2)
        : width_(width), height_(height), fillOption_(fillOption), fillValue1_(fillValue1), fillValue2_(fillValue2) {
        allocateMemory();

        if (fillOption_ == 1) {
            fillRandom();
        }
        else if (fillOption_ == 2) {
            fillWithBool();
        }
        else {
            fillWithDefaultValue();
        }
    }

    ~GrayscaleImage() {
        deallocateMemory();
    }

    const int& operator()(int row, int col) const {
        return matrix_[row * width_ + col];
    }

    int& operator()(int row, int col) {
        return matrix_[row * width_ + col];
    }

    GrayscaleImage operator!() const {
        GrayscaleImage result(width_, height_, fillOption_, 0, 0);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                result(i, j) = invertValue(matrix_[i * width_ + j]);
            }
        }
        return result;
    }

    GrayscaleImage operator+(const GrayscaleImage& other) const {
        GrayscaleImage result(width_, height_, fillOption_, 0, 0);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                result(i, j) = matrix_[i * width_ + j] + other(i, j);
            }
        }
        return result;
    }

    bool operator==(const GrayscaleImage& other) const {
        if (width_ != other.width_ || height_ != other.height_) {
            return false;
        }

        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }

    void drawRectangle(int x1, int y1, int x2, int y2, const int& value) {
        for (int i = y1; i <= y2; ++i) {
            for (int j = x1; j <= x2; ++j) {
                (*this)(i, j) = value;
            }
        }
    }

    template <typename V>
    V computeSumInRectangle(int x1, int y1, int x2, int y2) const {
        V sum = V();
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                sum += static_cast<V>((*this)(i, j));
            }
        }
        return sum;
    }

    void invertRectangle(int x1, int y1, int x2, int y2) {
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                (*this)(i, j) = invertValue((*this)(i, j));
            }
        }
    }

private:
    void allocateMemory() {
        matrix_ = new int[width_ * height_];
    }

    void deallocateMemory() {
        delete[] matrix_;
    }

    void fillRandom() {
        std::random_device rd;
        std::mt19937 gen(rd());

        if (fillOption_ == 2) {
            std::uniform_int_distribution<int> dist(0, 1);
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    matrix_[i * width_ + j] = dist(gen);
                }
            }
        }
        else {
            std::uniform_int_distribution<int> dist(-100, 100);
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    matrix_[i * width_ + j] = dist(gen);
                }
            }
        }
    }

    void fillWithBool() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                matrix_[i * width_ + j] = dist(gen);
            }
        }
    }

    void fillWithDefaultValue() {
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                matrix_[i * width_ + j] = 0;
            }
        }
    }

    int invertValue(int value) const {
        return -value;
    }

    int width_;
    int height_;
    int* matrix_;
    int fillOption_;
    int fillValue1_;
    int fillValue2_;
};

int main() {
    setlocale(LC_ALL, "Russian");
    int width, height;
    std::cout << "Введите ширину изображения: ";
    std::cin >> width;
    std::cout << "Введите высоту изображения: ";
    std::cin >> height;

    int x1, y1;
    int fillValue1;
    std::cout << "Введите x1 (левый верхний угол): ";
    std::cin >> x1;
    std::cout << "Введите y1 (левый верхний угол): ";
    std::cin >> y1;
    std::cout << "Введите значение для заполнения прямоугольника: ";
    std::cin >> fillValue1;

    int x2, y2;
    int fillValue2;
    std::cout << "Введите x2 (правый нижний угол): ";
    std::cin >> x2;
    std::cout << "Введите y2 (правый нижний угол): ";
    std::cin >> y2;
    std::cout << "Введите значение для заполнения прямоугольника: ";
    std::cin >> fillValue2;
    std::cout << "Выберите способ заполнения оставшихся элементов (0 - нули, 1 - int, 2 - bool): ";
    int fillOption;
    std::cin >> fillOption;

    GrayscaleImage img(width, height, fillOption, fillValue1, fillValue2);

    img.drawRectangle(x1, y1, x2, y2, fillValue1);
    img(x2, y2) = fillValue2;

    std::cout << "Изображение после рисования прямоугольника:" << std::endl;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << img(i, j) << ' ';
        }
        std::cout << std::endl;
    }

    auto sumInRectangleInt = img.computeSumInRectangle<int>(x1, y1, x2, y2);

    std::cout << "Сумма чисел в прямоугольнике: " << sumInRectangleInt << std::endl;

    int invertOption;
    std::cout << "Хотите инвертировать прямоугольник? (1 - Да, 0 - Нет): ";
    std::cin >> invertOption;

    if (invertOption == 1) {
        img.invertRectangle(x1, y1, x2, y2);

        std::cout << "Изображение после инверсии прямоугольника:" << std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << img(i, j) << ' ';
            }
            std::cout << std::endl;
        }
    }

    return 0;
}