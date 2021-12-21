// ASCII.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

#include <vector>


//// GENERAL SETTINGS ////
float scale = 2.5f; // Масштаб
string filename = "frontlever.bmp";


struct Pixel
{
    short R;
    short G;
    short B;
};


int main()
{
    FILE* f = fopen(filename.c_str(), "rb");

    if (!f) {
        throw "File Exception";
    }

    unsigned char info[54]{};
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl << endl;

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded] {};

    // Создаем массив пикселей
    vector<vector<Pixel>> pixels(height, vector<Pixel>(width));

    for (int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for (int j = 0; j < width * 3; j += 3)
        {
            int ser = int(data[j + 2] * 0.3 + data[j + 1] * 0.59 + data[j + 0] * 0.11);
            pixels[height - i - 1][j / 3].R = ser;
            pixels[height - i - 1][j / 3].G = ser;
            pixels[height - i - 1][j / 3].B = ser;
        }
    }

    // Конвертируем в ASCII
    char let[]{ " .,:'`;-\"^\\/*!_|irI<>+lv()t1j={}Jf?7Ly]cuT[xnYosVzF%34h2ewCaqkUZdAp05Pb86X9mg#H$SKWOEDNGR&BQM@" };
    int sWidth = int(8 / scale);
    int sHeight = int(16 / scale);

    for (int i = 0; i < height; i += sHeight)
    {
        for (int j = 0; j < width; j += sWidth)
        {
            // Подбираем подходящий символ так, чтобы результат получился примерно равен размеру оригинала
            int targetvalue(0);
            for (int sI(i); sI < ((i + sHeight < height) ? (i + sHeight) : (height - i)); ++sI)
                for (int sJ(j); sJ < ((j + sWidth < width) ? (j + sWidth) : (width - j)); ++sJ)
                    targetvalue += pixels[sI][sJ].R;

            targetvalue /= sWidth * sHeight;
            cout << let[int(targetvalue / 2.897)];
        }
        cout << endl;
    }

    fclose(f);
}