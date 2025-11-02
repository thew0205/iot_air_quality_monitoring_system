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
      co2: (json['co2'] as num).toDouble(),
      pm2_5: (json['pm2_5'] as num).toDouble(),
      pm10: (json['pm10'] as num).toDouble(),
      no2: (json['no2'] as num).toDouble(),
      co: (json['co'] as num).toDouble(),
      temp: (json['temp'] as num).toDouble(),
      rHum: (json['rHum'] as num).toDouble(),
    );

Map<String, dynamic> _$TemplateDataToJson(_TemplateData instance) =>
    <String, dynamic>{
      'co2': instance.co2,
      'pm2_5': instance.pm2_5,
      'pm10': instance.pm10,
      'no2': instance.no2,
      'co': instance.co,
      'temp': instance.temp,
      'rHum': instance.rHum,
    };
