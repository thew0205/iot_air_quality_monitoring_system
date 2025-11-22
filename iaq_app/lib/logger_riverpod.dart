import 'package:flutter_riverpod/flutter_riverpod.dart';

final class LoggerRiverpod extends ProviderObserver {
  const LoggerRiverpod();

  @override
  void didAddProvider(ProviderObserverContext provider, Object? value) {
    print('''
didAddProvider: {
"provider":"${provider.provider.name ?? provider.provider.runtimeType}",
"value":"${value.runtimeType}"}
''');
  }

  /// A provider emitted an error, be it by throwing during initialization
  /// or by having a [Future]/[Stream] emit an error
  @override
  void providerDidFail(
    ProviderObserverContext provider,
    Object error,
    StackTrace stackTrace,
  ) {
    print('''
providerDidFail: {
"provider":"${provider.provider.name ?? provider.provider.runtimeType}",
"error":"$error"}
''');
  }

  /// Called by providers when they emit a notification.
  ///
  /// - [newValue] will be `null` if the provider threw during initialization.
  /// - [previousValue] will be `null` if the previous build threw during initialization.
  @override
  void didUpdateProvider(
    ProviderObserverContext provider,
    Object? previousValue,
    Object? newValue,
  ) {
    print('''
didUpdateProvider: {
"provider":"${provider.provider.name ?? provider.provider.runtimeType}",
"previousValue":"${previousValue.runtimeType}",
"newValue":"${newValue.runtimeType}"}
''');
  }

  /// A provider was disposed
  @override
  void didDisposeProvider(ProviderObserverContext provider) {
    print('''
didDisposeProvider: {
"provider":"${provider.provider.name ?? provider.runtimeType}",}
''');
  }
}
