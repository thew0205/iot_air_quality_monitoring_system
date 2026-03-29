// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'model.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

_DataDateTime _$DataDateTimeFromJson(Map<String, dynamic> json) =>
    _DataDateTime(
      datum: (json['datum'] as num).toDouble(),
      dateTime: DateTime.parse(json['dateTime'] as String),
    );

Map<String, dynamic> _$DataDateTimeToJson(_DataDateTime instance) =>
    <String, dynamic>{
      'datum': instance.datum,
      'dateTime': instance.dateTime.toIso8601String(),
    };

_DataResponseGemini _$DataResponseGeminiFromJson(Map<String, dynamic> json) =>
    _DataResponseGemini(
      value: (json['value'] as num).toDouble(),
      minValue: (json['minValue'] as num).toDouble(),
      maxValue: (json['maxValue'] as num).toDouble(),
      status: json['status'] as String,
      message: json['message'] as String,
    );

Map<String, dynamic> _$DataResponseGeminiToJson(_DataResponseGemini instance) =>
    <String, dynamic>{
      'value': instance.value,
      'minValue': instance.minValue,
      'maxValue': instance.maxValue,
      'status': instance.status,
      'message': instance.message,
    };

_TemplateData _$TemplateDataFromJson(Map<String, dynamic> json) =>
    _TemplateData(
      pm1: (json['pm1'] as num).toDouble(),
      pm25: (json['pm25'] as num).toDouble(),
      pm10: (json['pm10'] as num).toDouble(),
      co2: (json['co2'] as num).toDouble(),
      voc: (json['voc'] as num).toDouble(),
      temp: (json['temp'] as num).toDouble(),
      hum: (json['hum'] as num).toDouble(),
      ch2o: (json['ch2o'] as num).toDouble(),
      co: (json['co'] as num).toDouble(),
      o3: (json['o3'] as num).toDouble(),
      no2: (json['no2'] as num).toDouble(),
      h2s: (json['h2s'] as num).toDouble(),
      timestamp: const DateTimeConverter().fromJson(
        json['timestamp'] as String,
      ),
    );

Map<String, dynamic> _$TemplateDataToJson(_TemplateData instance) =>
    <String, dynamic>{
      'pm1': instance.pm1,
      'pm25': instance.pm25,
      'pm10': instance.pm10,
      'co2': instance.co2,
      'voc': instance.voc,
      'temp': instance.temp,
      'hum': instance.hum,
      'ch2o': instance.ch2o,
      'co': instance.co,
      'o3': instance.o3,
      'no2': instance.no2,
      'h2s': instance.h2s,
      'timestamp': const DateTimeConverter().toJson(instance.timestamp),
    };
