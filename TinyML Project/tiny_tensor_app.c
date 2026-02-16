#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- Yapı Tanımları --- */
typedef enum {
    TENSOR_FLOAT32 = 0,
    TENSOR_INT8    = 2 
} TensorType;

typedef struct {
    float scale;
    int32_t zero_point;
} QuantizationParams;

typedef struct {
    TensorType type;
    uint32_t length;
    QuantizationParams q_params; 
    union {
        float* f32;
        int8_t* i8;
        void* raw;
    } data;
} Tensor;

/* --- Fonksiyonlar --- */
void quantize_f32_to_i8(float* input, int8_t* output, uint32_t len, QuantizationParams* params) {
    float min_val = input[0], max_val = input[0];
    uint32_t i; // i değişkenini BURADA BİR KEZ tanımladık

    // 1. Min/Max Bulma
    for(i = 0; i < len; i++) {
        if(input[i] < min_val) min_val = input[i];
        if(input[i] > max_val) max_val = input[i];
    }

    params->scale = (max_val - min_val) / 255.0f;
    params->zero_point = (int32_t)roundf(-min_val / params->scale) - 128;

    // 2. Dönüştürme (Burada tekrar uint32_t i yazmıyoruz!)
    for(i = 0; i < len; i++) {
        int32_t q = (int32_t)roundf(input[i] / params->scale) + params->zero_point;
        if(q > 127) q = 127;
        if(q < -128) q = -128;
        output[i] = (int8_t)q;
    }
}

float dequantize_i8_to_f32(int8_t input, QuantizationParams params) {
    return ((float)input - params.zero_point) * params.scale;
}

/* --- Ana Uygulama --- */
int main() {
    uint32_t size = 5;
    float raw_weights[] = {1.2f, -0.5f, 0.0f, 0.88f, -1.1f};
    uint32_t i; // Main içindeki döngüler için i tanımı burada
    
    printf("### TinyML Dynamic Tensor Demo ###\n\n");

    Tensor modelWeightTensor;
    modelWeightTensor.type = TENSOR_INT8;
    modelWeightTensor.length = size;
    modelWeightTensor.data.i8 = (int8_t*)malloc(size * sizeof(int8_t));

    if (modelWeightTensor.data.i8 == NULL) return 1;

    quantize_f32_to_i8(raw_weights, modelWeightTensor.data.i8, size, &modelWeightTensor.q_params);

    printf("Index | Orijinal | Quantized | Dequantized\n");
    printf("------------------------------------------\n");
    
    // Döngü içinde 'int i=0' yerine yukarıda tanımladığımız 'i'yi kullanıyoruz
    for(i = 0; i < size; i++) {
        float deq = dequantize_i8_to_f32(modelWeightTensor.data.i8[i], modelWeightTensor.q_params);
        printf("  %u   |  %6.2f  |    %4d   |   %6.2f\n", 
                i, raw_weights[i], modelWeightTensor.data.i8[i], deq);
    }

    free(modelWeightTensor.data.i8);
    return 0;
}