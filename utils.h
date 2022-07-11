#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <QtMath>
#include <QVector3D>

void vectorToArray(float *array, QVector3D &vector, int index) {
    array[3 * index] = vector.x();
    array[3 * index + 1] = vector.y();
    array[3 * index + 2] = vector.z();
}

float * toArray(QVector<QVector3D> &vector) {
    float *array = new float[vector.size() * 3];
    for (int i = 0; i < vector.length(); i++)
        vectorToArray(array, vector[i], i);
    return array;
}

#endif // UTILS_H
