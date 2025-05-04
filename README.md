# Trivium Stream Cipher 

This project is a standalone implementation of the **Trivium** stream cipher — a lightweight, hardware-oriented cryptographic algorithm selected as part of the eSTREAM portfolio (Profile II).

##  Overview

Trivium is designed for simplicity and speed in constrained environments like IoT devices and embedded systems. This project implements Trivium from scratch and evaluates its output using NIST statistical randomness tests.

##  Features

-  Full implementation of the Trivium key stream generator
-  80-bit key and 80-bit IV initialization
-  Evaluation of the output's randomness using:
- Frequency Test
- Cumulative Sums Test
- Non-Overlapping Template Matching
- Linear Complexity Test
- (Tests like Random Excursions were not performed)

##  Results Summary

- **No. of bit streams tested**: 10  
- **All tests passed**: P-values > 0.01  
- Notable results:
  - Cumulative Sums Test: *Lowest p-value* (≈ 0.083)
  - Linear Complexity and Template Tests: *High p-values*

###  Inference

The Trivium implementation produced output that demonstrates **strong randomness** properties across various statistical dimensions. It is suitable for cryptographic applications requiring secure pseudorandom generation.



