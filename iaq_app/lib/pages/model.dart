import 'dart:math';

import 'package:freezed_annotation/freezed_annotation.dart';

part 'model.freezed.dart';
part 'model.g.dart';

enum TimeFrame {
  second,
  minutes,
  hour,
  day,
}

final rand = Random();
int i = 0;

  
@freezed
abstract class DataDateTime with _$DataDateTime {
  const factory DataDateTime(
      {required double datum, required DateTime dateTime}) = _DataDateTime;

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

  // factory DataClass.fromJson(Map<String, dynamic> json) =>
  //     _$DataClassFromJson(json);
  static DataClass getDAtaFromId(String id) => para.firstWhere(
        (element) => id == element.id,
      );

  static const para = [
    DataClass(
      id: 'co2',
      text: 'CO2',
      unit: 'ppm',
      icon: "assets/icons/co2.png",
    ),
    DataClass(
      id: 'pm2_5',
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
      id: 'no2',
      text: 'NO2',
      unit: 'ppm',
      icon: "assets/icons/no2.png",
    ),
    DataClass(
      id: 'co',
      text: 'CO',
      unit: 'ppm',
      icon: "assets/icons/co.png",
    ),
    DataClass(
      id: 'temp',
      text: 'Temperature',
      unit: 'ppm',
      icon: "assets/icons/temp.png",
    ),
    DataClass(
      id: 'rHum',
      text: 'Relative Humidity',
      unit: 'ppm',
      icon: "assets/icons/rHum.png",
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
    required double co2,
    required double pm2_5,
    required double pm10,
    required double no2,
    required double co,
    required double temp,
    required double rHum,
    // required DateTime dateTime,
  }) = _TemplateData;

  factory TemplateData.fromJson(Map<String, dynamic> json) =>
      _$TemplateDataFromJson(json);
  // Map<String, dynamic> toMap() {
  //   return <String, dynamic>{
  //     'co2': co2,
  //     'pm2_5': pm2_5,
  //     'pm10': pm10,
  //     'no2': no2,
  //     'co': co,
  //     'temp': temp,
  //     'r_hum': relHum,
  //     'dateTime': dateTime.microsecondsSinceEpoch
  //   };
  // }
  const TemplateData._();

  factory TemplateData.generate() {
    final timeNow = DateTime.now();
    final time = timeNow.copyWith(
      second: 0,
      microsecond: 0,
      millisecond: 0,
    );
    i += 10;
    return TemplateData(
      co2: rand.nextDouble(),
      no2: rand.nextDouble(),
      pm10: rand.nextDouble(),
      pm2_5: rand.nextDouble(),
      co: rand.nextDouble(),
      temp: rand.nextDouble(),
      rHum: rand.nextDouble(),
      // dateTime: time.subtract(
        // Duration(seconds: i),
      // ),
    );
  }
  // factory TemplateData.fromMap(Map<String, dynamic> map) {
  //   return TemplateData(
  //       co2: map['co2'] as double,
  //       pm2_5: map['pm2_5'] as double,
  //       pm10: map['pm10'] as double,
  //       no2: map['no2'] as double,
  //       co: map['co'] as double,
  //       temp: map['temp'] as double,
  //       relHum: map['r_hum'] as double,
  //       dateTime: DateTime.fromMicrosecondsSinceEpoch(map['dateTime'] as int));
  // }

  double getData(String id) => toJson()[id];

  // factory TemplateData.fromJson(String source) =>
  // TemplateData.fromMap(json.decode(source) as Map<String, dynamic>);
}

// final templateDate = <String, double>{
//   'co2': rand.nextDouble(),
//   'pm2_5': rand.nextDouble(),
//   'pm10': rand.nextDouble(),
//   'no2': rand.nextDouble(),
//   'co': rand.nextDouble(),
// };

final templateDateList = [
  for (int i = 0; i < 60 * 60 * 24; i++) TemplateData.generate(),
];
