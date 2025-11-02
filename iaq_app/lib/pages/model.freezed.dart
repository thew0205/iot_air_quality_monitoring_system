// GENERATED CODE - DO NOT MODIFY BY HAND
// coverage:ignore-file
// ignore_for_file: type=lint
// ignore_for_file: unused_element, deprecated_member_use, deprecated_member_use_from_same_package, use_function_type_syntax_for_parameters, unnecessary_const, avoid_init_to_null, invalid_override_different_default_values_named, prefer_expression_function_bodies, annotate_overrides, invalid_annotation_target, unnecessary_question_mark

part of 'model.dart';

// **************************************************************************
// FreezedGenerator
// **************************************************************************

// dart format off
T _$identity<T>(T value) => value;

/// @nodoc
mixin _$DataDateTime {

 double get datum; DateTime get dateTime;
/// Create a copy of DataDateTime
/// with the given fields replaced by the non-null parameter values.
@JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
$DataDateTimeCopyWith<DataDateTime> get copyWith => _$DataDateTimeCopyWithImpl<DataDateTime>(this as DataDateTime, _$identity);

  /// Serializes this DataDateTime to a JSON map.
  Map<String, dynamic> toJson();


@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is DataDateTime&&(identical(other.datum, datum) || other.datum == datum)&&(identical(other.dateTime, dateTime) || other.dateTime == dateTime));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,datum,dateTime);

@override
String toString() {
  return 'DataDateTime(datum: $datum, dateTime: $dateTime)';
}


}

/// @nodoc
abstract mixin class $DataDateTimeCopyWith<$Res>  {
  factory $DataDateTimeCopyWith(DataDateTime value, $Res Function(DataDateTime) _then) = _$DataDateTimeCopyWithImpl;
@useResult
$Res call({
 double datum, DateTime dateTime
});




}
/// @nodoc
class _$DataDateTimeCopyWithImpl<$Res>
    implements $DataDateTimeCopyWith<$Res> {
  _$DataDateTimeCopyWithImpl(this._self, this._then);

  final DataDateTime _self;
  final $Res Function(DataDateTime) _then;

/// Create a copy of DataDateTime
/// with the given fields replaced by the non-null parameter values.
@pragma('vm:prefer-inline') @override $Res call({Object? datum = null,Object? dateTime = null,}) {
  return _then(_self.copyWith(
datum: null == datum ? _self.datum : datum // ignore: cast_nullable_to_non_nullable
as double,dateTime: null == dateTime ? _self.dateTime : dateTime // ignore: cast_nullable_to_non_nullable
as DateTime,
  ));
}

}


/// Adds pattern-matching-related methods to [DataDateTime].
extension DataDateTimePatterns on DataDateTime {
/// A variant of `map` that fallback to returning `orElse`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeMap<TResult extends Object?>(TResult Function( _DataDateTime value)?  $default,{required TResult orElse(),}){
final _that = this;
switch (_that) {
case _DataDateTime() when $default != null:
return $default(_that);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// Callbacks receives the raw object, upcasted.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case final Subclass2 value:
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult map<TResult extends Object?>(TResult Function( _DataDateTime value)  $default,){
final _that = this;
switch (_that) {
case _DataDateTime():
return $default(_that);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `map` that fallback to returning `null`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? mapOrNull<TResult extends Object?>(TResult? Function( _DataDateTime value)?  $default,){
final _that = this;
switch (_that) {
case _DataDateTime() when $default != null:
return $default(_that);case _:
  return null;

}
}
/// A variant of `when` that fallback to an `orElse` callback.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeWhen<TResult extends Object?>(TResult Function( double datum,  DateTime dateTime)?  $default,{required TResult orElse(),}) {final _that = this;
switch (_that) {
case _DataDateTime() when $default != null:
return $default(_that.datum,_that.dateTime);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// As opposed to `map`, this offers destructuring.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case Subclass2(:final field2):
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult when<TResult extends Object?>(TResult Function( double datum,  DateTime dateTime)  $default,) {final _that = this;
switch (_that) {
case _DataDateTime():
return $default(_that.datum,_that.dateTime);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `when` that fallback to returning `null`
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? whenOrNull<TResult extends Object?>(TResult? Function( double datum,  DateTime dateTime)?  $default,) {final _that = this;
switch (_that) {
case _DataDateTime() when $default != null:
return $default(_that.datum,_that.dateTime);case _:
  return null;

}
}

}

/// @nodoc
@JsonSerializable()

class _DataDateTime implements DataDateTime {
  const _DataDateTime({required this.datum, required this.dateTime});
  factory _DataDateTime.fromJson(Map<String, dynamic> json) => _$DataDateTimeFromJson(json);

@override final  double datum;
@override final  DateTime dateTime;

/// Create a copy of DataDateTime
/// with the given fields replaced by the non-null parameter values.
@override @JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
_$DataDateTimeCopyWith<_DataDateTime> get copyWith => __$DataDateTimeCopyWithImpl<_DataDateTime>(this, _$identity);

@override
Map<String, dynamic> toJson() {
  return _$DataDateTimeToJson(this, );
}

@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is _DataDateTime&&(identical(other.datum, datum) || other.datum == datum)&&(identical(other.dateTime, dateTime) || other.dateTime == dateTime));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,datum,dateTime);

@override
String toString() {
  return 'DataDateTime(datum: $datum, dateTime: $dateTime)';
}


}

/// @nodoc
abstract mixin class _$DataDateTimeCopyWith<$Res> implements $DataDateTimeCopyWith<$Res> {
  factory _$DataDateTimeCopyWith(_DataDateTime value, $Res Function(_DataDateTime) _then) = __$DataDateTimeCopyWithImpl;
@override @useResult
$Res call({
 double datum, DateTime dateTime
});




}
/// @nodoc
class __$DataDateTimeCopyWithImpl<$Res>
    implements _$DataDateTimeCopyWith<$Res> {
  __$DataDateTimeCopyWithImpl(this._self, this._then);

  final _DataDateTime _self;
  final $Res Function(_DataDateTime) _then;

/// Create a copy of DataDateTime
/// with the given fields replaced by the non-null parameter values.
@override @pragma('vm:prefer-inline') $Res call({Object? datum = null,Object? dateTime = null,}) {
  return _then(_DataDateTime(
datum: null == datum ? _self.datum : datum // ignore: cast_nullable_to_non_nullable
as double,dateTime: null == dateTime ? _self.dateTime : dateTime // ignore: cast_nullable_to_non_nullable
as DateTime,
  ));
}


}

/// @nodoc
mixin _$DataClass {

 String get id; String get text; String get unit; String get icon;
/// Create a copy of DataClass
/// with the given fields replaced by the non-null parameter values.
@JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
$DataClassCopyWith<DataClass> get copyWith => _$DataClassCopyWithImpl<DataClass>(this as DataClass, _$identity);



@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is DataClass&&(identical(other.id, id) || other.id == id)&&(identical(other.text, text) || other.text == text)&&(identical(other.unit, unit) || other.unit == unit)&&(identical(other.icon, icon) || other.icon == icon));
}


@override
int get hashCode => Object.hash(runtimeType,id,text,unit,icon);

@override
String toString() {
  return 'DataClass(id: $id, text: $text, unit: $unit, icon: $icon)';
}


}

/// @nodoc
abstract mixin class $DataClassCopyWith<$Res>  {
  factory $DataClassCopyWith(DataClass value, $Res Function(DataClass) _then) = _$DataClassCopyWithImpl;
@useResult
$Res call({
 String id, String text, String unit, String icon
});




}
/// @nodoc
class _$DataClassCopyWithImpl<$Res>
    implements $DataClassCopyWith<$Res> {
  _$DataClassCopyWithImpl(this._self, this._then);

  final DataClass _self;
  final $Res Function(DataClass) _then;

/// Create a copy of DataClass
/// with the given fields replaced by the non-null parameter values.
@pragma('vm:prefer-inline') @override $Res call({Object? id = null,Object? text = null,Object? unit = null,Object? icon = null,}) {
  return _then(_self.copyWith(
id: null == id ? _self.id : id // ignore: cast_nullable_to_non_nullable
as String,text: null == text ? _self.text : text // ignore: cast_nullable_to_non_nullable
as String,unit: null == unit ? _self.unit : unit // ignore: cast_nullable_to_non_nullable
as String,icon: null == icon ? _self.icon : icon // ignore: cast_nullable_to_non_nullable
as String,
  ));
}

}


/// Adds pattern-matching-related methods to [DataClass].
extension DataClassPatterns on DataClass {
/// A variant of `map` that fallback to returning `orElse`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeMap<TResult extends Object?>(TResult Function( _DataClass value)?  $default,{required TResult orElse(),}){
final _that = this;
switch (_that) {
case _DataClass() when $default != null:
return $default(_that);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// Callbacks receives the raw object, upcasted.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case final Subclass2 value:
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult map<TResult extends Object?>(TResult Function( _DataClass value)  $default,){
final _that = this;
switch (_that) {
case _DataClass():
return $default(_that);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `map` that fallback to returning `null`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? mapOrNull<TResult extends Object?>(TResult? Function( _DataClass value)?  $default,){
final _that = this;
switch (_that) {
case _DataClass() when $default != null:
return $default(_that);case _:
  return null;

}
}
/// A variant of `when` that fallback to an `orElse` callback.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeWhen<TResult extends Object?>(TResult Function( String id,  String text,  String unit,  String icon)?  $default,{required TResult orElse(),}) {final _that = this;
switch (_that) {
case _DataClass() when $default != null:
return $default(_that.id,_that.text,_that.unit,_that.icon);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// As opposed to `map`, this offers destructuring.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case Subclass2(:final field2):
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult when<TResult extends Object?>(TResult Function( String id,  String text,  String unit,  String icon)  $default,) {final _that = this;
switch (_that) {
case _DataClass():
return $default(_that.id,_that.text,_that.unit,_that.icon);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `when` that fallback to returning `null`
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? whenOrNull<TResult extends Object?>(TResult? Function( String id,  String text,  String unit,  String icon)?  $default,) {final _that = this;
switch (_that) {
case _DataClass() when $default != null:
return $default(_that.id,_that.text,_that.unit,_that.icon);case _:
  return null;

}
}

}

/// @nodoc


class _DataClass implements DataClass {
  const _DataClass({required this.id, required this.text, required this.unit, required this.icon});
  

@override final  String id;
@override final  String text;
@override final  String unit;
@override final  String icon;

/// Create a copy of DataClass
/// with the given fields replaced by the non-null parameter values.
@override @JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
_$DataClassCopyWith<_DataClass> get copyWith => __$DataClassCopyWithImpl<_DataClass>(this, _$identity);



@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is _DataClass&&(identical(other.id, id) || other.id == id)&&(identical(other.text, text) || other.text == text)&&(identical(other.unit, unit) || other.unit == unit)&&(identical(other.icon, icon) || other.icon == icon));
}


@override
int get hashCode => Object.hash(runtimeType,id,text,unit,icon);

@override
String toString() {
  return 'DataClass(id: $id, text: $text, unit: $unit, icon: $icon)';
}


}

/// @nodoc
abstract mixin class _$DataClassCopyWith<$Res> implements $DataClassCopyWith<$Res> {
  factory _$DataClassCopyWith(_DataClass value, $Res Function(_DataClass) _then) = __$DataClassCopyWithImpl;
@override @useResult
$Res call({
 String id, String text, String unit, String icon
});




}
/// @nodoc
class __$DataClassCopyWithImpl<$Res>
    implements _$DataClassCopyWith<$Res> {
  __$DataClassCopyWithImpl(this._self, this._then);

  final _DataClass _self;
  final $Res Function(_DataClass) _then;

/// Create a copy of DataClass
/// with the given fields replaced by the non-null parameter values.
@override @pragma('vm:prefer-inline') $Res call({Object? id = null,Object? text = null,Object? unit = null,Object? icon = null,}) {
  return _then(_DataClass(
id: null == id ? _self.id : id // ignore: cast_nullable_to_non_nullable
as String,text: null == text ? _self.text : text // ignore: cast_nullable_to_non_nullable
as String,unit: null == unit ? _self.unit : unit // ignore: cast_nullable_to_non_nullable
as String,icon: null == icon ? _self.icon : icon // ignore: cast_nullable_to_non_nullable
as String,
  ));
}


}


/// @nodoc
mixin _$DataResponseGemini {

 double get value; double get minValue; double get maxValue; String get status; String get message;
/// Create a copy of DataResponseGemini
/// with the given fields replaced by the non-null parameter values.
@JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
$DataResponseGeminiCopyWith<DataResponseGemini> get copyWith => _$DataResponseGeminiCopyWithImpl<DataResponseGemini>(this as DataResponseGemini, _$identity);

  /// Serializes this DataResponseGemini to a JSON map.
  Map<String, dynamic> toJson();


@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is DataResponseGemini&&(identical(other.value, value) || other.value == value)&&(identical(other.minValue, minValue) || other.minValue == minValue)&&(identical(other.maxValue, maxValue) || other.maxValue == maxValue)&&(identical(other.status, status) || other.status == status)&&(identical(other.message, message) || other.message == message));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,value,minValue,maxValue,status,message);

@override
String toString() {
  return 'DataResponseGemini(value: $value, minValue: $minValue, maxValue: $maxValue, status: $status, message: $message)';
}


}

/// @nodoc
abstract mixin class $DataResponseGeminiCopyWith<$Res>  {
  factory $DataResponseGeminiCopyWith(DataResponseGemini value, $Res Function(DataResponseGemini) _then) = _$DataResponseGeminiCopyWithImpl;
@useResult
$Res call({
 double value, double minValue, double maxValue, String status, String message
});




}
/// @nodoc
class _$DataResponseGeminiCopyWithImpl<$Res>
    implements $DataResponseGeminiCopyWith<$Res> {
  _$DataResponseGeminiCopyWithImpl(this._self, this._then);

  final DataResponseGemini _self;
  final $Res Function(DataResponseGemini) _then;

/// Create a copy of DataResponseGemini
/// with the given fields replaced by the non-null parameter values.
@pragma('vm:prefer-inline') @override $Res call({Object? value = null,Object? minValue = null,Object? maxValue = null,Object? status = null,Object? message = null,}) {
  return _then(_self.copyWith(
value: null == value ? _self.value : value // ignore: cast_nullable_to_non_nullable
as double,minValue: null == minValue ? _self.minValue : minValue // ignore: cast_nullable_to_non_nullable
as double,maxValue: null == maxValue ? _self.maxValue : maxValue // ignore: cast_nullable_to_non_nullable
as double,status: null == status ? _self.status : status // ignore: cast_nullable_to_non_nullable
as String,message: null == message ? _self.message : message // ignore: cast_nullable_to_non_nullable
as String,
  ));
}

}


/// Adds pattern-matching-related methods to [DataResponseGemini].
extension DataResponseGeminiPatterns on DataResponseGemini {
/// A variant of `map` that fallback to returning `orElse`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeMap<TResult extends Object?>(TResult Function( _DataResponseGemini value)?  $default,{required TResult orElse(),}){
final _that = this;
switch (_that) {
case _DataResponseGemini() when $default != null:
return $default(_that);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// Callbacks receives the raw object, upcasted.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case final Subclass2 value:
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult map<TResult extends Object?>(TResult Function( _DataResponseGemini value)  $default,){
final _that = this;
switch (_that) {
case _DataResponseGemini():
return $default(_that);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `map` that fallback to returning `null`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? mapOrNull<TResult extends Object?>(TResult? Function( _DataResponseGemini value)?  $default,){
final _that = this;
switch (_that) {
case _DataResponseGemini() when $default != null:
return $default(_that);case _:
  return null;

}
}
/// A variant of `when` that fallback to an `orElse` callback.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeWhen<TResult extends Object?>(TResult Function( double value,  double minValue,  double maxValue,  String status,  String message)?  $default,{required TResult orElse(),}) {final _that = this;
switch (_that) {
case _DataResponseGemini() when $default != null:
return $default(_that.value,_that.minValue,_that.maxValue,_that.status,_that.message);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// As opposed to `map`, this offers destructuring.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case Subclass2(:final field2):
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult when<TResult extends Object?>(TResult Function( double value,  double minValue,  double maxValue,  String status,  String message)  $default,) {final _that = this;
switch (_that) {
case _DataResponseGemini():
return $default(_that.value,_that.minValue,_that.maxValue,_that.status,_that.message);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `when` that fallback to returning `null`
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? whenOrNull<TResult extends Object?>(TResult? Function( double value,  double minValue,  double maxValue,  String status,  String message)?  $default,) {final _that = this;
switch (_that) {
case _DataResponseGemini() when $default != null:
return $default(_that.value,_that.minValue,_that.maxValue,_that.status,_that.message);case _:
  return null;

}
}

}

/// @nodoc
@JsonSerializable()

class _DataResponseGemini implements DataResponseGemini {
  const _DataResponseGemini({required this.value, required this.minValue, required this.maxValue, required this.status, required this.message});
  factory _DataResponseGemini.fromJson(Map<String, dynamic> json) => _$DataResponseGeminiFromJson(json);

@override final  double value;
@override final  double minValue;
@override final  double maxValue;
@override final  String status;
@override final  String message;

/// Create a copy of DataResponseGemini
/// with the given fields replaced by the non-null parameter values.
@override @JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
_$DataResponseGeminiCopyWith<_DataResponseGemini> get copyWith => __$DataResponseGeminiCopyWithImpl<_DataResponseGemini>(this, _$identity);

@override
Map<String, dynamic> toJson() {
  return _$DataResponseGeminiToJson(this, );
}

@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is _DataResponseGemini&&(identical(other.value, value) || other.value == value)&&(identical(other.minValue, minValue) || other.minValue == minValue)&&(identical(other.maxValue, maxValue) || other.maxValue == maxValue)&&(identical(other.status, status) || other.status == status)&&(identical(other.message, message) || other.message == message));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,value,minValue,maxValue,status,message);

@override
String toString() {
  return 'DataResponseGemini(value: $value, minValue: $minValue, maxValue: $maxValue, status: $status, message: $message)';
}


}

/// @nodoc
abstract mixin class _$DataResponseGeminiCopyWith<$Res> implements $DataResponseGeminiCopyWith<$Res> {
  factory _$DataResponseGeminiCopyWith(_DataResponseGemini value, $Res Function(_DataResponseGemini) _then) = __$DataResponseGeminiCopyWithImpl;
@override @useResult
$Res call({
 double value, double minValue, double maxValue, String status, String message
});




}
/// @nodoc
class __$DataResponseGeminiCopyWithImpl<$Res>
    implements _$DataResponseGeminiCopyWith<$Res> {
  __$DataResponseGeminiCopyWithImpl(this._self, this._then);

  final _DataResponseGemini _self;
  final $Res Function(_DataResponseGemini) _then;

/// Create a copy of DataResponseGemini
/// with the given fields replaced by the non-null parameter values.
@override @pragma('vm:prefer-inline') $Res call({Object? value = null,Object? minValue = null,Object? maxValue = null,Object? status = null,Object? message = null,}) {
  return _then(_DataResponseGemini(
value: null == value ? _self.value : value // ignore: cast_nullable_to_non_nullable
as double,minValue: null == minValue ? _self.minValue : minValue // ignore: cast_nullable_to_non_nullable
as double,maxValue: null == maxValue ? _self.maxValue : maxValue // ignore: cast_nullable_to_non_nullable
as double,status: null == status ? _self.status : status // ignore: cast_nullable_to_non_nullable
as String,message: null == message ? _self.message : message // ignore: cast_nullable_to_non_nullable
as String,
  ));
}


}


/// @nodoc
mixin _$TemplateData {

 double get co2; double get pm2_5; double get pm10; double get no2; double get co; double get temp; double get rHum;
/// Create a copy of TemplateData
/// with the given fields replaced by the non-null parameter values.
@JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
$TemplateDataCopyWith<TemplateData> get copyWith => _$TemplateDataCopyWithImpl<TemplateData>(this as TemplateData, _$identity);

  /// Serializes this TemplateData to a JSON map.
  Map<String, dynamic> toJson();


@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is TemplateData&&(identical(other.co2, co2) || other.co2 == co2)&&(identical(other.pm2_5, pm2_5) || other.pm2_5 == pm2_5)&&(identical(other.pm10, pm10) || other.pm10 == pm10)&&(identical(other.no2, no2) || other.no2 == no2)&&(identical(other.co, co) || other.co == co)&&(identical(other.temp, temp) || other.temp == temp)&&(identical(other.rHum, rHum) || other.rHum == rHum));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,co2,pm2_5,pm10,no2,co,temp,rHum);

@override
String toString() {
  return 'TemplateData(co2: $co2, pm2_5: $pm2_5, pm10: $pm10, no2: $no2, co: $co, temp: $temp, rHum: $rHum)';
}


}

/// @nodoc
abstract mixin class $TemplateDataCopyWith<$Res>  {
  factory $TemplateDataCopyWith(TemplateData value, $Res Function(TemplateData) _then) = _$TemplateDataCopyWithImpl;
@useResult
$Res call({
 double co2, double pm2_5, double pm10, double no2, double co, double temp, double rHum
});




}
/// @nodoc
class _$TemplateDataCopyWithImpl<$Res>
    implements $TemplateDataCopyWith<$Res> {
  _$TemplateDataCopyWithImpl(this._self, this._then);

  final TemplateData _self;
  final $Res Function(TemplateData) _then;

/// Create a copy of TemplateData
/// with the given fields replaced by the non-null parameter values.
@pragma('vm:prefer-inline') @override $Res call({Object? co2 = null,Object? pm2_5 = null,Object? pm10 = null,Object? no2 = null,Object? co = null,Object? temp = null,Object? rHum = null,}) {
  return _then(_self.copyWith(
co2: null == co2 ? _self.co2 : co2 // ignore: cast_nullable_to_non_nullable
as double,pm2_5: null == pm2_5 ? _self.pm2_5 : pm2_5 // ignore: cast_nullable_to_non_nullable
as double,pm10: null == pm10 ? _self.pm10 : pm10 // ignore: cast_nullable_to_non_nullable
as double,no2: null == no2 ? _self.no2 : no2 // ignore: cast_nullable_to_non_nullable
as double,co: null == co ? _self.co : co // ignore: cast_nullable_to_non_nullable
as double,temp: null == temp ? _self.temp : temp // ignore: cast_nullable_to_non_nullable
as double,rHum: null == rHum ? _self.rHum : rHum // ignore: cast_nullable_to_non_nullable
as double,
  ));
}

}


/// Adds pattern-matching-related methods to [TemplateData].
extension TemplateDataPatterns on TemplateData {
/// A variant of `map` that fallback to returning `orElse`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeMap<TResult extends Object?>(TResult Function( _TemplateData value)?  $default,{required TResult orElse(),}){
final _that = this;
switch (_that) {
case _TemplateData() when $default != null:
return $default(_that);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// Callbacks receives the raw object, upcasted.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case final Subclass2 value:
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult map<TResult extends Object?>(TResult Function( _TemplateData value)  $default,){
final _that = this;
switch (_that) {
case _TemplateData():
return $default(_that);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `map` that fallback to returning `null`.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case final Subclass value:
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? mapOrNull<TResult extends Object?>(TResult? Function( _TemplateData value)?  $default,){
final _that = this;
switch (_that) {
case _TemplateData() when $default != null:
return $default(_that);case _:
  return null;

}
}
/// A variant of `when` that fallback to an `orElse` callback.
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return orElse();
/// }
/// ```

@optionalTypeArgs TResult maybeWhen<TResult extends Object?>(TResult Function( double co2,  double pm2_5,  double pm10,  double no2,  double co,  double temp,  double rHum)?  $default,{required TResult orElse(),}) {final _that = this;
switch (_that) {
case _TemplateData() when $default != null:
return $default(_that.co2,_that.pm2_5,_that.pm10,_that.no2,_that.co,_that.temp,_that.rHum);case _:
  return orElse();

}
}
/// A `switch`-like method, using callbacks.
///
/// As opposed to `map`, this offers destructuring.
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case Subclass2(:final field2):
///     return ...;
/// }
/// ```

@optionalTypeArgs TResult when<TResult extends Object?>(TResult Function( double co2,  double pm2_5,  double pm10,  double no2,  double co,  double temp,  double rHum)  $default,) {final _that = this;
switch (_that) {
case _TemplateData():
return $default(_that.co2,_that.pm2_5,_that.pm10,_that.no2,_that.co,_that.temp,_that.rHum);case _:
  throw StateError('Unexpected subclass');

}
}
/// A variant of `when` that fallback to returning `null`
///
/// It is equivalent to doing:
/// ```dart
/// switch (sealedClass) {
///   case Subclass(:final field):
///     return ...;
///   case _:
///     return null;
/// }
/// ```

@optionalTypeArgs TResult? whenOrNull<TResult extends Object?>(TResult? Function( double co2,  double pm2_5,  double pm10,  double no2,  double co,  double temp,  double rHum)?  $default,) {final _that = this;
switch (_that) {
case _TemplateData() when $default != null:
return $default(_that.co2,_that.pm2_5,_that.pm10,_that.no2,_that.co,_that.temp,_that.rHum);case _:
  return null;

}
}

}

/// @nodoc
@JsonSerializable()

class _TemplateData extends TemplateData {
  const _TemplateData({required this.co2, required this.pm2_5, required this.pm10, required this.no2, required this.co, required this.temp, required this.rHum}): super._();
  factory _TemplateData.fromJson(Map<String, dynamic> json) => _$TemplateDataFromJson(json);

@override final  double co2;
@override final  double pm2_5;
@override final  double pm10;
@override final  double no2;
@override final  double co;
@override final  double temp;
@override final  double rHum;

/// Create a copy of TemplateData
/// with the given fields replaced by the non-null parameter values.
@override @JsonKey(includeFromJson: false, includeToJson: false)
@pragma('vm:prefer-inline')
_$TemplateDataCopyWith<_TemplateData> get copyWith => __$TemplateDataCopyWithImpl<_TemplateData>(this, _$identity);

@override
Map<String, dynamic> toJson() {
  return _$TemplateDataToJson(this, );
}

@override
bool operator ==(Object other) {
  return identical(this, other) || (other.runtimeType == runtimeType&&other is _TemplateData&&(identical(other.co2, co2) || other.co2 == co2)&&(identical(other.pm2_5, pm2_5) || other.pm2_5 == pm2_5)&&(identical(other.pm10, pm10) || other.pm10 == pm10)&&(identical(other.no2, no2) || other.no2 == no2)&&(identical(other.co, co) || other.co == co)&&(identical(other.temp, temp) || other.temp == temp)&&(identical(other.rHum, rHum) || other.rHum == rHum));
}

@JsonKey(includeFromJson: false, includeToJson: false)
@override
int get hashCode => Object.hash(runtimeType,co2,pm2_5,pm10,no2,co,temp,rHum);

@override
String toString() {
  return 'TemplateData(co2: $co2, pm2_5: $pm2_5, pm10: $pm10, no2: $no2, co: $co, temp: $temp, rHum: $rHum)';
}


}

/// @nodoc
abstract mixin class _$TemplateDataCopyWith<$Res> implements $TemplateDataCopyWith<$Res> {
  factory _$TemplateDataCopyWith(_TemplateData value, $Res Function(_TemplateData) _then) = __$TemplateDataCopyWithImpl;
@override @useResult
$Res call({
 double co2, double pm2_5, double pm10, double no2, double co, double temp, double rHum
});




}
/// @nodoc
class __$TemplateDataCopyWithImpl<$Res>
    implements _$TemplateDataCopyWith<$Res> {
  __$TemplateDataCopyWithImpl(this._self, this._then);

  final _TemplateData _self;
  final $Res Function(_TemplateData) _then;

/// Create a copy of TemplateData
/// with the given fields replaced by the non-null parameter values.
@override @pragma('vm:prefer-inline') $Res call({Object? co2 = null,Object? pm2_5 = null,Object? pm10 = null,Object? no2 = null,Object? co = null,Object? temp = null,Object? rHum = null,}) {
  return _then(_TemplateData(
co2: null == co2 ? _self.co2 : co2 // ignore: cast_nullable_to_non_nullable
as double,pm2_5: null == pm2_5 ? _self.pm2_5 : pm2_5 // ignore: cast_nullable_to_non_nullable
as double,pm10: null == pm10 ? _self.pm10 : pm10 // ignore: cast_nullable_to_non_nullable
as double,no2: null == no2 ? _self.no2 : no2 // ignore: cast_nullable_to_non_nullable
as double,co: null == co ? _self.co : co // ignore: cast_nullable_to_non_nullable
as double,temp: null == temp ? _self.temp : temp // ignore: cast_nullable_to_non_nullable
as double,rHum: null == rHum ? _self.rHum : rHum // ignore: cast_nullable_to_non_nullable
as double,
  ));
}


}

// dart format on
