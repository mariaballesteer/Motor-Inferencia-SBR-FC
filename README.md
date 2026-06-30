# Motor de Inferencia SBR - Sistemas Basados en Reglas con Factores de Certeza

Proyecto desarrollado como parte de la asignatura **Sistemas Inteligentes (SSII)** del Grado en Ingeniería Informática.

**Autora:** María Ballester Martínez  
**Fecha de creación:** Diciembre de 2024

---

## Descripción

Este proyecto consiste en el desarrollo e implementación en C++ de un motor de inferencia basado en un algoritmo de **Encadenamiento Hacia Atrás (Backward Chaining)**. 

El sistema es capaz de evaluar reglas lógicas combinando premisas (antecedentes y consecuentes) y gestionando la incertidumbre mediante el cálculo y propagación de **Factores de Certeza (FC)**.

---

## Características

- Algoritmo de encadenamiento hacia atrás para la resolución de objetivos.
- Gestión avanzada de la incertidumbre mediante la lógica de Factores de Certeza.
- Combinación de factores de certeza ante múltiples reglas que apuntan a una misma hipótesis.
- Lectura dinámica de Bases de Conocimiento (`BC.txt`) y Bases de Hechos (`BH.txt`) desde archivos externos.
- Generación automatizada de archivos de resultados con las trazas y la red de inferencia obtenida.

---

## Tecnologías utilizadas

- C++ 
- Programación Modular y Estructuras de Datos Eficientes

---

## Estructura del proyecto

motor-inferencia-sbr/
├── doc/
│   ├── ManualUso.md
│   ├── Practica2-Cuestiones.pdf
│   └── Practica2-Informe.pdf
├── ejecutable/
│   └── SBR-FC.exe
├── FicherosPruebas/
│   ├── Prueba1/
│   |   ├── BC-1.txt
│   |   ├── BC-1.txt_BH-1.txt
│   |   └── BH-1.txt
│   ├── Prueba2/
│   |   ├── BC-2.txt
│   |   ├── BC-2.txt_BH-2_1.txt
│   |   ├── BC-2.txt_BH-2_2.txt
│   |   ├── BH-2_1.txt
│   |   └── BH-2_2.txt
│   ├── Prueba3/
│   |   ├── BC-3.txt
│   |   ├── BC-3.txt_BH-3.txt
│   |   └── BH-3.txt
│   └── PruebaA/
│   |   ├── BC-A.txt
│   |   ├── BC-A.txt_BH-A.txt
│   |   └── BH-A.txt
├── src/
│   └── SBR-FC.cpp
└── README.md