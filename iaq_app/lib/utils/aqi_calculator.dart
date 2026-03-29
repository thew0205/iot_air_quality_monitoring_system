import 'package:flutter/material.dart';
import 'dart:math';

class AQIInfo {
  final int index;
  final String level;
  final Color color;
  final Color onColor;

  const AQIInfo({
    required this.index,
    required this.level,
    required this.color,
    required this.onColor,
  });
}

class AQICalculator {
  // AQI standard breakpoint lists [Breakpoint range, AQI range]
  // Format: [Clow, Chigh, Ilow, Ihigh]

  static int _calculateIndex(double c, List<List<double>> breakpoints) {
    for (final bp in breakpoints) {
      if (c >= bp[0] && c <= bp[1]) {
        return ((bp[3] - bp[2]) / (bp[1] - bp[0]) * (c - bp[0]) + bp[2])
            .round();
      }
    }
    // If value exceeds max breakpoint, extrapolate or cap
    final last = breakpoints.last;
    if (c > last[1]) {
      return ((last[3] - last[2]) / (last[1] - last[0]) * (c - last[0]) +
              last[2])
          .round();
    }
    return 0; // Default or below 0
  }

  static AQIInfo getAQI(String id, double value) {
    int index = 0;

    // Approximate breakpoints based on standard AQI/indoor guidelines.
    // Assuming mostly standard ppm or ug/m3 units depending on the sensor provided.

    switch (id.toLowerCase()) {
      case 'pm25':
      case 'pm2.5':
        // PM2.5 in ug/m3
        index = _calculateIndex(value, [
          [0.0, 12.0, 0, 50],
          [12.1, 35.4, 51, 100],
          [35.5, 55.4, 101, 150],
          [55.5, 150.4, 151, 200],
          [150.5, 250.4, 201, 300],
          [250.5, 500.4, 301, 500],
        ]);
        break;
      case 'pm10':
        // PM10 in ug/m3
        index = _calculateIndex(value, [
          [0, 54, 0, 50],
          [55, 154, 51, 100],
          [155, 254, 101, 150],
          [255, 354, 151, 200],
          [355, 424, 201, 300],
          [425, 604, 301, 500],
        ]);
        break;
      case 'pm1':
        // PM1 has no official EPA scale; map similarly to PM2.5 but stricter.
        index = _calculateIndex(value, [
          [0.0, 10.0, 0, 50],
          [10.1, 25.0, 51, 100],
          [25.1, 40.0, 101, 150],
          [40.1, 100.0, 151, 200],
          [100.1, 200.0, 201, 300],
          [200.1, 400.0, 301, 500],
        ]);
        break;
      case 'co':
        // CO in ppm
        index = _calculateIndex(value, [
          [0.0, 4.4, 0, 50],
          [4.5, 9.4, 51, 100],
          [9.5, 12.4, 101, 150],
          [12.5, 15.4, 151, 200],
          [15.5, 30.4, 201, 300],
          [30.5, 50.4, 301, 500],
        ]);
        break;
      case 'no2':
        // NO2 in ppm
        index = _calculateIndex(value, [
          [0.0, 0.053, 0, 50],
          [0.054, 0.100, 51, 100],
          [0.101, 0.360, 101, 150],
          [0.361, 0.649, 151, 200],
          [0.650, 1.249, 201, 300],
          [1.250, 2.049, 301, 500],
        ]);
        break;
      case 'o3':
        // O3 in ppm
        index = _calculateIndex(value, [
          [0.0, 0.054, 0, 50],
          [0.055, 0.070, 51, 100],
          [0.071, 0.085, 101, 150],
          [0.086, 0.105, 151, 200],
          [0.106, 0.200, 201, 300],
          [0.201, 0.400, 301, 500],
        ]);
        break;
      case 'co2':
        // CO2 in ppm (Proxy index based on indoor air quality)
        // 400-1000 good, 1000-2000 moderate/warning, 2000-5000 poor
        index = _calculateIndex(value, [
          [0, 1000, 0, 50],
          [1001, 2000, 51, 100],
          [2001, 3000, 101, 150],
          [3001, 5000, 151, 200],
          [5001, 10000, 201, 300],
        ]);
        break;
      case 'voc':
        // VOC in ppm (Proxy index)
        index = _calculateIndex(value, [
          [0.0, 0.5, 0, 50],
          [0.51, 1.0, 51, 100],
          [1.01, 3.0, 101, 150],
          [3.01, 5.0, 151, 200],
          [5.01, 10.0, 201, 300],
        ]);
        break;
      case 'ch2o':
        // Formaldehyde in ppm (Proxy index)
        index = _calculateIndex(value, [
          [0.0, 0.08, 0, 50],
          [0.081, 0.1, 51, 100],
          [0.101, 0.3, 101, 150],
          [0.301, 0.5, 151, 200],
          [0.501, 1.0, 201, 300],
        ]);
        break;
      case 'temp':
      case 'hum':
      default:
        // For non-gas parameters (Temperature, Humidity), we don't calculate an AQI standard.
        // Returning a neutral -1 to signal it should not display severe/good warnings strictly.
        index = -1;
        break;
    }

    // Limit index
    index = max(index, -1);

    // Categorize
    if (index == -1) {
      return const AQIInfo(
        index: 0,
        level: "N/A",
        color: Colors.grey,
        onColor: Colors.white,
      );
    } else if (index <= 50) {
      return AQIInfo(
        index: index,
        level: "Good",
        color: Colors.green.shade400,
        onColor: Colors.white,
      );
    } else if (index <= 100) {
      return AQIInfo(
        index: index,
        level: "Moderate",
        color: Colors.yellow.shade600,
        onColor: Colors.black87,
      );
    } else if (index <= 150) {
      return AQIInfo(
        index: index,
        level: "Unhealthy (SG)",
        color: Colors.orange.shade400,
        onColor: Colors.white,
      );
    } else if (index <= 200) {
      return AQIInfo(
        index: index,
        level: "Unhealthy",
        color: Colors.red.shade500,
        onColor: Colors.white,
      );
    } else if (index <= 300) {
      return AQIInfo(
        index: index,
        level: "Very Unhealthy",
        color: Colors.purple.shade400,
        onColor: Colors.white,
      );
    } else {
      return AQIInfo(
        index: index,
        level: "Hazardous",
        color: Colors.brown.shade700,
        onColor: Colors.white,
      );
    }
  }
}
