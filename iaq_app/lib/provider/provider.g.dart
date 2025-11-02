// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'provider.dart';

// **************************************************************************
// RiverpodGenerator
// **************************************************************************

// GENERATED CODE - DO NOT MODIFY BY HAND
// ignore_for_file: type=lint, type=warning

@ProviderFor(ThemeNotifier)
const themeProvider = ThemeNotifierProvider._();

final class ThemeNotifierProvider
    extends $NotifierProvider<ThemeNotifier, ThemeMode> {
  const ThemeNotifierProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'themeProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$themeNotifierHash();

  @$internal
  @override
  ThemeNotifier create() => ThemeNotifier();

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(ThemeMode value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<ThemeMode>(value),
    );
  }
}

String _$themeNotifierHash() => r'9e814af768e189b93040bc23d8b9644b7800cf93';

abstract class _$ThemeNotifier extends $Notifier<ThemeMode> {
  ThemeMode build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref = this.ref as $Ref<ThemeMode, ThemeMode>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<ThemeMode, ThemeMode>,
              ThemeMode,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}

@ProviderFor(TemplateDataClass)
const templateDataClassProvider = TemplateDataClassProvider._();

final class TemplateDataClassProvider
    extends $NotifierProvider<TemplateDataClass, List<TemplateData>> {
  const TemplateDataClassProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'templateDataClassProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$templateDataClassHash();

  @$internal
  @override
  TemplateDataClass create() => TemplateDataClass();

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(List<TemplateData> value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<List<TemplateData>>(value),
    );
  }
}

String _$templateDataClassHash() => r'd12bbb7744dbe7bd16ad65067548baa9bc0dcd8f';

abstract class _$TemplateDataClass extends $Notifier<List<TemplateData>> {
  List<TemplateData> build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref = this.ref as $Ref<List<TemplateData>, List<TemplateData>>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<List<TemplateData>, List<TemplateData>>,
              List<TemplateData>,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}

@ProviderFor(TimeFrameNotifier)
const timeFrameProvider = TimeFrameNotifierProvider._();

final class TimeFrameNotifierProvider
    extends $NotifierProvider<TimeFrameNotifier, TimeFrame> {
  const TimeFrameNotifierProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'timeFrameProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$timeFrameNotifierHash();

  @$internal
  @override
  TimeFrameNotifier create() => TimeFrameNotifier();

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(TimeFrame value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<TimeFrame>(value),
    );
  }
}

String _$timeFrameNotifierHash() => r'ae63fb5b0c9c285ea6c6382bd986d3370bb0e45c';

abstract class _$TimeFrameNotifier extends $Notifier<TimeFrame> {
  TimeFrame build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref = this.ref as $Ref<TimeFrame, TimeFrame>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<TimeFrame, TimeFrame>,
              TimeFrame,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}

@ProviderFor(MyInt)
const myIntProvider = MyIntProvider._();

final class MyIntProvider extends $NotifierProvider<MyInt, int> {
  const MyIntProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'myIntProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$myIntHash();

  @$internal
  @override
  MyInt create() => MyInt();

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(int value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<int>(value),
    );
  }
}

String _$myIntHash() => r'f251ada8e040aebcaabd5137a01ce0c4c0e6b688';

abstract class _$MyInt extends $Notifier<int> {
  int build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref = this.ref as $Ref<int, int>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<int, int>,
              int,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}

@ProviderFor(sharedPreference)
const sharedPreferenceProvider = SharedPreferenceProvider._();

final class SharedPreferenceProvider
    extends
        $FunctionalProvider<
          SharedPreferencesWithCache,
          SharedPreferencesWithCache,
          SharedPreferencesWithCache
        >
    with $Provider<SharedPreferencesWithCache> {
  const SharedPreferenceProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'sharedPreferenceProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$sharedPreferenceHash();

  @$internal
  @override
  $ProviderElement<SharedPreferencesWithCache> $createElement(
    $ProviderPointer pointer,
  ) => $ProviderElement(pointer);

  @override
  SharedPreferencesWithCache create(Ref ref) {
    return sharedPreference(ref);
  }

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(SharedPreferencesWithCache value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<SharedPreferencesWithCache>(value),
    );
  }
}

String _$sharedPreferenceHash() => r'98271899029c2322aaca66d5db7d1ceccf640f4a';

@ProviderFor(MqttData)
const mqttDataProvider = MqttDataProvider._();

final class MqttDataProvider extends $NotifierProvider<MqttData, TemplateData> {
  const MqttDataProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'mqttDataProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$mqttDataHash();

  @$internal
  @override
  MqttData create() => MqttData();

  /// {@macro riverpod.override_with_value}
  Override overrideWithValue(TemplateData value) {
    return $ProviderOverride(
      origin: this,
      providerOverride: $SyncValueProvider<TemplateData>(value),
    );
  }
}

String _$mqttDataHash() => r'a45aee16ba68b1ce040407340f6e3943220b7afc';

abstract class _$MqttData extends $Notifier<TemplateData> {
  TemplateData build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref = this.ref as $Ref<TemplateData, TemplateData>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<TemplateData, TemplateData>,
              TemplateData,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}

@ProviderFor(MqttClientNotifier)
const mqttClientProvider = MqttClientNotifierProvider._();

final class MqttClientNotifierProvider
    extends $AsyncNotifierProvider<MqttClientNotifier, MqttServerClient> {
  const MqttClientNotifierProvider._()
    : super(
        from: null,
        argument: null,
        retry: null,
        name: r'mqttClientProvider',
        isAutoDispose: true,
        dependencies: null,
        $allTransitiveDependencies: null,
      );

  @override
  String debugGetCreateSourceHash() => _$mqttClientNotifierHash();

  @$internal
  @override
  MqttClientNotifier create() => MqttClientNotifier();
}

String _$mqttClientNotifierHash() =>
    r'7f81421eb6e9885a46747c8b2faf2bb4e8c030a2';

abstract class _$MqttClientNotifier extends $AsyncNotifier<MqttServerClient> {
  FutureOr<MqttServerClient> build();
  @$mustCallSuper
  @override
  void runBuild() {
    final created = build();
    final ref =
        this.ref as $Ref<AsyncValue<MqttServerClient>, MqttServerClient>;
    final element =
        ref.element
            as $ClassProviderElement<
              AnyNotifier<AsyncValue<MqttServerClient>, MqttServerClient>,
              AsyncValue<MqttServerClient>,
              Object?,
              Object?
            >;
    element.handleValue(ref, created);
  }
}
