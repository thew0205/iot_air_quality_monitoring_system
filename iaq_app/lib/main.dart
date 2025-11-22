import 'package:flex_color_scheme/flex_color_scheme.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/logger_riverpod.dart';
import 'package:iaq_app/provider/provider.dart';
import 'package:iaq_app/provider/router.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'platform_specific_code.dart';

void main() async {
  usePathUrlStrategy();
  WidgetsFlutterBinding.ensureInitialized();

  final prefs = await SharedPreferencesWithCache.create(
    cacheOptions: const SharedPreferencesWithCacheOptions(
      // This cache will only accept the key 'counter'.
      allowList: <String>{'uid', 'email'},
    ),
  );
  runApp(
    ProviderScope(
      observers: [const LoggerRiverpod()],
      overrides: [sharedPreferenceProvider.overrideWithValue(prefs)],
      child: const MyApp(),
    ),
  );
}

final FlexSchemeColor schemeLight = FlexSchemeColor.from(primary: Colors.cyan);
final FlexSchemeColor schemeDark = FlexSchemeColor.from(
  primary: const Color.fromARGB(255, 0, 87, 98),
);

class MyApp extends ConsumerStatefulWidget {
  const MyApp({super.key});

  @override
  ConsumerState<ConsumerStatefulWidget> createState() => _MyAppState();
}

class _MyAppState extends ConsumerState<MyApp> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitDown,
      DeviceOrientation.portraitUp,
    ]);
    return MaterialApp.router(
      debugShowCheckedModeBanner: false,
      routerConfig: ref.watch(goRouteProvider),
      title: 'Power Leaf',
      themeMode: ref.watch(themeProvider),
      darkTheme: FlexThemeData.dark(scheme: FlexScheme.flutterDash),

      //  ThemeData(
      //   colorScheme: ColorScheme.fromSeed(
      //       brightness: Brightness.dark, seedColor: Colors.cyan),
      //   useMaterial3: true,
      // ),
      theme: FlexThemeData.light(scheme: FlexScheme.flutterDash),
      // ThemeData(
      //   colorScheme: ColorScheme.fromSeed(seedColor: Colors.cyan),
      //   useMaterial3: true,
      // ),
    );
  }
}
