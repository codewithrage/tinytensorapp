# TinyML Dynamic Tensor & Quantization Implementation

Bu proje, bellek kısıtlı gömülü sistemlerde (Arduino, ESP32, STM32 vb.) yapay zeka modellerini verimli bir şekilde çalıştırmak için tasarlanmış dinamik bir **Tensor** yapısını ve **8-bit Quantization (Niceleme)** algoritmasını içermektedir.

## 🚀 Proje Amacı
Standart derin öğrenme modelleri ağırlıkları 32-bit Float formatında saklar. Ancak 32KB - 512KB SRAM kapasitesine sahip mikrodenetleyicilerde bu veriler hızlıca belleği doldurur. Bu projede:
- `Union` yapısı kullanılarak bellek tasarrufu sağlanmıştır.
- Floating point veriler `INT8` formatına sıkıştırılarak bellek kullanımı %75 oranında azaltılmıştır.
- C dilinin düşük seviyeli imkanları kullanılarak "Primitive Tensor" mimarisi oluşturulmuştur.

## 🛠 Teknik Detaylar
- **Dil:** C (C99 Standardı)
- **Veri Yapısı:** `Union` tabanlı dinamik tip yönetimi.
- **Quantization:** Asymmetric Linear Quantization ($f = (q - Z) * S$).
- **Hedef Platform:** TinyML destekli tüm mikrodenetleyiciler.

## 📁 Dosya Yapısı
- `tensor.h`: Tensor veri yapısı ve tip tanımlamaları.
- `main.c`: Quantization algoritması ve demo uygulaması.
- `docs/`: Sunum ve teknik dokümantasyon (isteğe bağlı).

## 💻 Kurulum ve Çalıştırma
Herhangi bir C derleyicisi (GCC, Clang) ile çalıştırılabilir:

```bash
gcc main.c -o tiny_ml_demo -lm
./tiny_ml_demo
