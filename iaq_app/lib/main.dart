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
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitDown,
      DeviceOrientation.portraitUp,
    ]);
  }

  @override
  Widget build(BuildContext context) {
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

// import 'package:flutter/material.dart';
// import 'package:syncfusion_flutter_charts/charts.dart';
// import 'package:syncfusion_flutter_charts/sparkcharts.dart';

// void main() {
//   return runApp(_ChartApp());
// }

// class _ChartApp extends StatelessWidget {
//   @override
//   Widget build(BuildContext context) {
//     return MaterialApp(
//       theme: ThemeData(primarySwatch: Colors.blue, useMaterial3: false),
//       home: _MyHomePage(),
//     );
//   }
// }

// class _MyHomePage extends StatefulWidget {
//   // ignore: prefer_const_constructors_in_immutables
//   _MyHomePage({Key? key}) : super(key: key);

//   @override
//   _MyHomePageState createState() => _MyHomePageState();
// }

// class _MyHomePageState extends State<_MyHomePage> {
//   List<_SalesData> data = [
//     _SalesData('Jan', 35),
//     _SalesData('Feb', 28),
//     _SalesData('Mar', 34),
//     _SalesData('Apr', 32),
//     _SalesData('May', 40),
//     _SalesData('Jan', 35),
//     _SalesData('Feb', 28),
//     _SalesData('Mar', 34),
//     _SalesData('Apr', 32),
//     _SalesData('May', 40),
//     _SalesData('Jan', 35),
//     _SalesData('Feb', 28),
//     _SalesData('Mar', 34),
//     _SalesData('Apr', 32),
//     _SalesData('May', 40),
//     _SalesData('Jan', 35),
//     _SalesData('Feb', 28),
//     _SalesData('Mar', 34),
//     _SalesData('Apr', 32),
//     _SalesData('May', 40),
//   ];
//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: AppBar(title: const Text('Syncfusion Flutter chart')),
//       body: Column(
//         children: [
//           //Initialize the chart widget
//           SfCartesianChart(
//             zoomPanBehavior: ZoomPanBehavior(
//               enablePinching: true,
//               enablePanning: true,
//               zoomMode: ZoomMode.x,
//             ),
//             primaryXAxis: CategoryAxis(),
//             // Chart title
//             title: ChartTitle(text: 'Half yearly sales analysis'),
//             // Enable legend
//             legend: Legend(isVisible: true),
//             // Enable tooltip
//             tooltipBehavior: TooltipBehavior(enable: true),
//             series: <CartesianSeries<_SalesData, String>>[
//               LineSeries<_SalesData, String>(
//                 dataSource: data,
//                 xValueMapper: (_SalesData sales, _) => sales.year,
//                 yValueMapper: (_SalesData sales, _) => sales.sales,
//                 name: 'Sales',
//                 // Enable data label
//                 dataLabelSettings: DataLabelSettings(isVisible: true),
//               ),
//             ],
//           ),
//           Expanded(
//             child: Padding(
//               padding: const EdgeInsets.all(8.0),
//               //Initialize the spark charts widget
//               child: SfSparkLineChart.custom(
//                 //Enable the trackball
//                 trackball: SparkChartTrackball(
//                   activationMode: SparkChartActivationMode.tap,
//                 ),
//                 //Enable marker
//                 marker: SparkChartMarker(
//                   displayMode: SparkChartMarkerDisplayMode.all,
//                 ),
//                 //Enable data label
//                 labelDisplayMode: SparkChartLabelDisplayMode.all,
//                 xValueMapper: (int index) => data[index].year,
//                 yValueMapper: (int index) => data[index].sales,
//                 dataCount: 5,
//               ),
//             ),
//           ),
//         ],
//       ),
//     );
//   }
// }

// class _SalesData {
//   _SalesData(this.year, this.sales);

//   final String year;
//   final double sales;
// }
