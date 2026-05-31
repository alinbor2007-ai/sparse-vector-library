/**
 * @file sparse_vector.h
 * @brief Разреженный вектор для C
 */

#ifndef SPARSE_VECTOR_H
#define SPARSE_VECTOR_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Структура для хранения ненулевого элемента
 */
typedef struct {
    int index;    /**< Индекс элемента */
    double value; /**< Значение элемента */
} SparseEntry;

/**
 * @brief Разреженный вектор
 */
typedef struct {
    SparseEntry* entries; /**< Массив ненулевых элементов */
    int count;            /**< Количество ненулевых элементов */
    int size;             /**< Общая длина вектора */
} SparseVector;

/**
 * @brief Создает разреженный вектор из плотного массива
 * @param dense Обычный массив
 * @param size Размер массива
 * @return SparseVector Созданный вектор
 */
SparseVector sparse_vector_create(double* dense, int size) {
    SparseVector vec;
    vec.size = size;
    vec.count = 0;
    
    for (int i = 0; i < size; i++) {
        if (dense[i] != 0) {
            vec.count++;
        }
    }
    
    vec.entries = (SparseEntry*)malloc(vec.count * sizeof(SparseEntry));
    
    int pos = 0;
    for (int i = 0; i < size; i++) {
        if (dense[i] != 0) {
            vec.entries[pos].index = i;
            vec.entries[pos].value = dense[i];
            pos++;
        }
    }
    
    return vec;
}

/**
 * @brief Вычисляет скалярное произведение двух векторов
 * @param a Первый вектор
 * @param b Второй вектор
 * @return double Результат
 */
double sparse_vector_dot(SparseVector* a, SparseVector* b) {
    if (a->size != b->size) {
        return 0;
    }
    
    double result = 0;
    
    for (int i = 0; i < a->count; i++) {
        for (int j = 0; j < b->count; j++) {
            if (a->entries[i].index == b->entries[j].index) {
                result += a->entries[i].value * b->entries[j].value;
                break;
            }
        }
    }
    
    return result;
}

/**
 * @brief Преобразует разреженный вектор в плотный
 * @param vec Разреженный вектор
 * @return double* Плотный массив (нужно освободить free)
 */
double* sparse_vector_to_dense(SparseVector* vec) {
    double* dense = (double*)malloc(vec->size * sizeof(double));
    
    for (int i = 0; i < vec->size; i++) {
        dense[i] = 0;
    }
    
    for (int i = 0; i < vec->count; i++) {
        dense[vec->entries[i].index] = vec->entries[i].value;
    }
    
    return dense;
}

/**
 * @brief Складывает два разреженных вектора
 * @param a Первый вектор
 * @param b Второй вектор
 * @return SparseVector Новый вектор-сумма
 */
SparseVector sparse_vector_add(SparseVector* a, SparseVector* b) {
    if (a->size != b->size) {
        SparseVector empty = {NULL, 0, 0};
        return empty;
    }
    
    double* temp = (double*)calloc(a->size, sizeof(double));
    
    for (int i = 0; i < a->count; i++) {
        temp[a->entries[i].index] += a->entries[i].value;
    }
    
    for (int i = 0; i < b->count; i++) {
        temp[b->entries[i].index] += b->entries[i].value;
    }
    
    SparseVector result = sparse_vector_create(temp, a->size);
    free(temp);
    
    return result;
}

/**
 * @brief Освобождает память вектора
 * @param vec Вектор для освобождения
 */
void sparse_vector_free(SparseVector* vec) {
    if (vec->entries) {
        free(vec->entries);
        vec->entries = NULL;
    }
    vec->count = 0;
    vec->size = 0;
}

#endif
