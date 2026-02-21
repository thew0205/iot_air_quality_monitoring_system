import 'dart:math';

import 'package:freezed_annotation/freezed_annotation.dart';

part 'model.freezed.dart';
part 'model.g.dart';

enum TimeFrame { second, minutes, hour, day }

final rand = Random();
int i = 0;

@freezed
abstract class DataDateTime with _$DataDateTime {
  const factory DataDateTime({
    required double datum,
    required DateTime dateTime,
  }) = _DataDateTime;

  factory DataDateTime.fromJson(Map<String, dynamic> json) =>
      _$DataDateTimeFromJson(json);
}

@freezed
abstract class DataClass with _$DataClass {
  const factory DataClass({
    required String id,
    required String text,
    required String unit,
    required String icon,
  }) = _DataClass;

  static DataClass getDAtaFromId(String id) =>
      para.firstWhere((element) => id == element.id);

  static const para = [
    DataClass(
      id: 'pm1',
      text: 'PM1',
      unit: 'ppm',
      icon: "assets/icons/pm2_5.png",
    ),
    DataClass(
      id: 'pm25',
      text: 'PM2.5',
      unit: 'ppm',
      icon: "assets/icons/pm2_5.png",
    ),
    DataClass(
      id: 'pm10',
      text: 'PM10',
      unit: 'ppm',
      icon: "assets/icons/pm10.png",
    ),
    DataClass(
      id: 'co2',
      text: 'CO2',
      unit: 'ppm',
      icon: "assets/icons/co2.png",
    ),
    DataClass(
      id: 'voc',
      text: 'VOC',
      unit: 'ppm',
      icon: "assets/icons/co2.png",
    ),
    DataClass(
      id: 'temp',
      text: 'Temperature',
      unit: 'ppm',
      icon: "assets/icons/temp.png",
    ),
    DataClass(
      id: 'hum',
      text: 'Relative Humidity',
      unit: 'ppm',
      icon: "assets/icons/rHum.png",
    ),
    DataClass(
      id: 'ch2o',
      text: 'CH2O',
      unit: 'ppm',
      icon: "assets/icons/no2.png",
    ),
    DataClass(id: 'co', text: 'CO', unit: 'ppm', icon: "assets/icons/co.png"),
    DataClass(id: 'o3', text: 'O3', unit: 'ppm', icon: "assets/icons/no2.png"),
    DataClass(
      id: 'no2',
      text: 'NO2',
      unit: 'ppm',
      icon: "assets/icons/no2.png",
    ),
  ];
}

@freezed
abstract class DataResponseGemini with _$DataResponseGemini {
  const factory DataResponseGemini({
    required double value,
    required double minValue,
    required double maxValue,
    required String status,
    required String message,
  }) = _DataResponseGemini;

  factory DataResponseGemini.fromJson(Map<String, dynamic> json) =>
      _$DataResponseGeminiFromJson(json);
}

@freezed
abstract class TemplateData with _$TemplateData {
  const factory TemplateData({
    required double pm1,
    required double pm25,
    required double pm10,
    required double co2,
    required double voc,
    required double temp,
    required double hum,
    required double ch2o,
    required double co,
    required double o3,
    required double no2,

    required double h2s,
    // required double sno2,
  }) = _TemplateData;

  factory TemplateData.fromJson(Map<String, dynamic> json) =>
      _$TemplateDataFromJson(json);

  const TemplateData._();

  double getData(String id) => toJson()[id];
}
