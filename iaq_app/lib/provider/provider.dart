import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/pages/model.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import 'package:shared_preferences/shared_preferences.dart';

part 'provider.g.dart';

// final themeModeProvider = StateProvider((ref) => ThemeMode.light);

@riverpod
class ThemeNotifier extends _$ThemeNotifier {
  @override
  ThemeMode build() => ThemeMode.light;

  void changeTheme() =>
      state = state == ThemeMode.light ? ThemeMode.dark : ThemeMode.light;
}

@riverpod
class TemplateDataClass extends _$TemplateDataClass {
  @override
  List<TemplateData> build() {
    return templateDateList;
  }
}

// final timeFrameProvider = StateProvider((ref) => TimeFrame.second);

@riverpod
class TimeFrameNotifier extends _$TimeFrameNotifier {
  @override
  TimeFrame build() => TimeFrame.second;

  void changeTheme(TimeFrame timeFrame) => state = timeFrame;
}

// @riverpod
// List<DataDateTime> filterTemplateData(Ref ref, {required String id}) {
//   final currentDateTime = DateTime.now();
//   final templateData = ref.watch(templateDataClassProvider);
//   final timeFrame = ref.watch(timeFrameProvider);

//   return switch (timeFrame) {
//     TimeFrame.second =>
//       templateData
//           .where((element) {
//             return currentDateTime.difference(element.dateTime).inSeconds <
//                 60 * 5;
//           })
//           .map((e) => DataDateTime(datum: e.getData(id), dateTime: e.dateTime))
//           .toList(),
//     TimeFrame.minutes =>
//       templateData
//           .where((element) {
//             return element.dateTime.second == 0 &&
//                 currentDateTime.difference(element.dateTime).inMinutes < 60;
//           })
//           .map((e) => DataDateTime(datum: e.getData(id), dateTime: e.dateTime))
//           .toList(),
//     TimeFrame.hour =>
//       templateData
//           .where((element) {
//             return element.dateTime.second == 0 &&
//                 element.dateTime.minute == 0 &&
//                 currentDateTime.difference(element.dateTime).inHours < 24;
//           })
//           .map((e) => DataDateTime(datum: e.getData(id), dateTime: e.dateTime))
//           .toList(),
//     TimeFrame.day =>
//       templateData
//           .where((element) {
//             return element.dateTime.second == 0 &&
//                 element.dateTime.minute == 0 &&
//                 element.dateTime.hour == 12;
//           })
//           .map((e) => DataDateTime(datum: e.getData(id), dateTime: e.dateTime))
//           .toList(),
//   };
// }

@riverpod
class MyInt extends _$MyInt {
  @override
  int build() {
    return 0;
  }

  void add() {
    state += 1;
    final now = DateTime.now();
    final data = {
      now.millisecondsSinceEpoch.toString(): {
        "int": state,
        "float": state.toStringAsPrecision(2),
        "string": "state $state",
      },
    };
  }
}

@riverpod
SharedPreferencesWithCache sharedPreference(Ref ref) => throw "error";

@riverpod
class MqttData extends _$MqttData {
  @override
  TemplateData build() {
    
    return TemplateData(
      co2: 0,
      pm2_5: 0,
      pm10: 0,
      no2: 0,
      co: 0,
      rHum: 0,
      temp: 0,
      // dateTime: DateTime.now(),
    );
  }

  void setData(TemplateData data) {
    state = data;
  }

  void setDataFromMap(Map<String, dynamic> map) {
    state = TemplateData.fromJson(map);
  }
}

Future<Uint8List> loadAssetContent(String path) async {
  String data = await rootBundle.loadString(path);
  return Uint8List.fromList(utf8.encode(data));
}

const url = 'aj8uipcillvb-ats.iot.eu-west-2.amazonaws.com';

// @riverpod
// Future<MqttServerClient> mqttClient(Ref ref) async {
//   final client = MqttServerClient.withPort(url, 'dart_test', 8883);
//   ref.keepAlive();

//   client.logging(on: true);

//   Uint8List rootCaBytes = await loadAssetContent('assets/AmazonRootCA1.pem');

//   Uint8List certificateByte = await loadAssetContent('assets/certificate.crt');

//   Uint8List privateByte = await loadAssetContent('assets/private.key');

//   client.secure = true;

//   final context = SecurityContext.defaultContext;
//   context.setClientAuthoritiesBytes(rootCaBytes);
//   context.useCertificateChainBytes(certificateByte);
//   context.usePrivateKeyBytes(privateByte);
//   client.securityContext = context;

//   client.setProtocolV311();

//   client.keepAlivePeriod = 20;

//   client.connectTimeoutPeriod = 2000;

//   client.onDisconnected = onDisconnected;

//   client.onConnected = onConnected;

//   client.onSubscribed = onSubscribed;

//   client.pongCallback = pong;

//   client.pingCallback = ping;

//   final connMess = MqttConnectMessage()
//       .withClientIdentifier('dart_test')
//       .withWillTopic('willtopic')
//       .withWillMessage('My Will message')
//       .startClean()
//       .withWillQos(MqttQos.atMostOnce);
//   print('EXAMPLE::MQTT client connecting....');
//   client.connectionMessage = connMess;

//   try {
//     await client.connect();
//   } on NoConnectionException catch (e) {
//     print('EXAMPLE::client exception - $e');
//     client.disconnect();
//   } on SocketException catch (e) {
//     print('EXAMPLE::socket exception - $e');
//     client.disconnect();
//   }

//   if (client.connectionStatus!.state == MqttConnectionState.connected) {
//     print('EXAMPLE::MQTT client connected');
//   } else {
//     print(
//       'EXAMPLE::ERROR MQTT client connection failed - disconnecting, status is ${client.connectionStatus}',
//     );
//     client.disconnect();
//   }

//   print('EXAMPLE::Subscribing to the test/lol topic');
//   const topic = 'test/lol';
//   client.subscribe("/test/topic/#", MqttQos.atMostOnce);
//   client.subscribe("/test/topic", MqttQos.atMostOnce);

//   client.updates!.listen((List<MqttReceivedMessage<MqttMessage?>>? c) {
//     final recMess = c![0].payload as MqttPublishMessage;
//     final pt = MqttPublishPayload.bytesToStringAsString(
//       recMess.payload.message,
//     );

//     print(
//       'EXAMPLE::Change notification:: topic is <${c[0].topic}>, payload is <-- $pt -->',
//     );
//     ref
//         .read(mqttDataProvider.notifier)
//         .setDataFromMap(jsonDecode(pt) as Map<String, dynamic>);
//   });

//   client.published!.listen((MqttPublishMessage message) {
//     print(
//       'EXAMPLE::Published notification:: topic is ${message.variableHeader!.topicName}, with Qos ${message.header!.qos}',
//     );
//   });

//   const pubTopic = '/test/topic';
//   final builder = MqttClientPayloadBuilder();
//   builder.addString('Hello from mqtt_client');

//   return client;
// }

@riverpod
class MqttClientNotifier extends _$MqttClientNotifier {
  @override
  Future<MqttServerClient> build() async {
    final client = MqttServerClient.withPort(url, 'dart_test', 8883);
    client.logging(on: true);

    Uint8List rootCaBytes = await loadAssetContent('assets/AmazonRootCA1.pem');

    Uint8List certificateByte = await loadAssetContent(
      'assets/certificate.crt',
    );

    Uint8List privateByte = await loadAssetContent('assets/private.key');

    client.secure = true;

    final context = SecurityContext.defaultContext;
    context.setClientAuthoritiesBytes(rootCaBytes);
    context.useCertificateChainBytes(certificateByte);
    context.usePrivateKeyBytes(privateByte);
    client.securityContext = context;

    client.setProtocolV311();

    client.keepAlivePeriod = 20;

    client.connectTimeoutPeriod = 2000;

    client.onDisconnected = onDisconnected;

    client.onConnected = onConnected;

    client.onSubscribed = onSubscribed;

    client.pongCallback = pong;

    client.pingCallback = ping;

    final connMess = MqttConnectMessage()
        .withClientIdentifier('dart_test')
        .withWillTopic('willtopic')
        .withWillMessage('My Will message')
        .startClean()
        .withWillQos(MqttQos.atMostOnce);
    print('EXAMPLE::MQTT client connecting....');
    client.connectionMessage = connMess;

    try {
      await client.connect();
    } on NoConnectionException catch (e) {
      print('EXAMPLE::client exception - $e');
      client.disconnect();
    } on SocketException catch (e) {
      print('EXAMPLE::socket exception - $e');
      client.disconnect();
    }

    if (client.connectionStatus!.state == MqttConnectionState.connected) {
      print('EXAMPLE::MQTT client connected');
    } else {
      print(
        'EXAMPLE::ERROR MQTT client connection failed - disconnecting, status is ${client.connectionStatus}',
      );
      client.disconnect();
    }

    print('EXAMPLE::Subscribing to the test/lol topic');
    const topic = 'test/lol';
    client.subscribe("/test/topic/#", MqttQos.atMostOnce);
    client.subscribe("/test/topic", MqttQos.atMostOnce);

    client.updates!.listen((List<MqttReceivedMessage<MqttMessage?>>? c) {
      final recMess = c![0].payload as MqttPublishMessage;
      final pt = MqttPublishPayload.bytesToStringAsString(
        recMess.payload.message,
      );

      print(
        'EXAMPLE::Change notification:: topic is <${c[0].topic}>, payload is <-- $pt -->',
      );
      ref
          .read(mqttDataProvider.notifier)
          .setDataFromMap(jsonDecode(pt) as Map<String, dynamic>);
    });

    client.published!.listen((MqttPublishMessage message) {
      print(
        'EXAMPLE::Published notification:: topic is ${message.variableHeader!.topicName}, with Qos ${message.header!.qos}',
      );
    });

    const pubTopic = '/test/topic';
    final builder = MqttClientPayloadBuilder();
    builder.addString('Hello from mqtt_client');
    return client;
  }

  void onSubscribed(String topic) {
    print('EXAMPLE::Subscription confirmed for topic $topic');
  }

  void onDisconnected() {
    print('EXAMPLE::OnDisconnected client callback - Client disconnection');
    if (state.value!.connectionStatus!.disconnectionOrigin ==
        MqttDisconnectionOrigin.solicited) {
      print('EXAMPLE::OnDisconnected callback is solicited, this is correct');
    } else {
      print(
        'EXAMPLE::OnDisconnected callback is unsolicited or none, this is incorrect - exiting',
      );
      exit(-1);
    }
  }

  void onConnected() {
    print(
      'EXAMPLE::OnConnected client callback - Client connection was successful',
    );
  }

  void pong() {
    print('EXAMPLE::Ping response client callback invoked');
    print(
      'EXAMPLE::Latency of this ping/pong cycle is ${state.value!.lastCycleLatency} milliseconds',
    );
  }

  void ping() {
    print('EXAMPLE::Ping sent client callback invoked');
  }

  void publishMessage(String topic, String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    state.value!.publishMessage(topic, MqttQos.atMostOnce, builder.payload!);
  }
}
