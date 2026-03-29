import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:iaq_app/pages/home_page.dart';
import 'package:iaq_app/pages/sensor_page.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';


part 'router.g.dart';

@riverpod
GoRouter goRoute(Ref ref) {
  return GoRouter(
    debugLogDiagnostics: true,
    initialLocation: '/',
    redirect: (context, state) {
      if (kDebugMode) {
        print("path: ${state.path}");
        print("fullPath: ${state.fullPath}");
        print("uri: ${state.uri}");
        print("matchedLocation: ${state.matchedLocation}");
      }

      return null;
    },
    routes: [
      GoRoute(
        name: "home",
        path: '/',
        builder: (context, state) => MyHomePage(key: state.pageKey),
        routes: [
          GoRoute(
            name: "sensor",
            path: '/sensor/:paramId',
            builder: (context, state) => SensorChartPage(key: state.pageKey, paramId: state.pathParameters['paramId']!),
          )
        ],
      ),
      // GoRoute(
      //   name: "login",
      //   path: '/login',
      //   builder: (context, state) => MyLoginPage(key: state.pageKey),
      // ),
      // GoRoute(
      //   name: "signup",
      //   path: '/signup',
      //   builder: (context, state) => MySignupPage(key: state.pageKey),
      // ),
    ],
  );
}
