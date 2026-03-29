
import 'package:flutter/services.dart';

import 'dart:async';
import 'dart:io';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'dart:convert';

const url = 'aj8uipcillvb-ats.iot.eu-west-2.amazonaws.com';

final client = MqttServerClient.withPort(url, 'dart_test', 8883);

var pongCount = 0;
var pingCount = 0;



var receivedMessage= '';



Future<Uint8List> loadAssetContent(String path) async {
  String data = await rootBundle.loadString(path);
  return Uint8List.fromList(utf8.encode(data));
}

Future<void> connect() async {
  
  client.logging(on: true);

  Uint8List rootCaBytes = await loadAssetContent('assets/AmazonRootCA1.pem');

  Uint8List certificateByte = await loadAssetContent('assets/certificate.crt');

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
  print('EXAMPLE::Mosquitto client connecting....');
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
    print('EXAMPLE::Mosquitto client connected');
  } else {
    print(
      'EXAMPLE::ERROR Mosquitto client connection failed - disconnecting, status is ${client.connectionStatus}',
    );
    client.disconnect();
    exit(-1);
  }

  print('EXAMPLE::Subscribing to the test/lol topic');
  const topic = 'test/lol';
  client.subscribe("test/topic/#", MqttQos.atMostOnce);
  client.subscribe("test/topic", MqttQos.atMostOnce);

  client.updates!.listen((List<MqttReceivedMessage<MqttMessage?>>? c) {
    final recMess = c![0].payload as MqttPublishMessage;
    final pt = MqttPublishPayload.bytesToStringAsString(
      recMess.payload.message,
    );

    print(
      'EXAMPLE::Change notification:: topic is <${c[0].topic}>, payload is <-- $pt -->',
    );
    receivedMessage += '\n$pt';
    print(receivedMessage);
  });

  client.published!.listen((MqttPublishMessage message) {
    print(
      'EXAMPLE::Published notification:: topic is ${message.variableHeader!.topicName}, with Qos ${message.header!.qos}',
    );
  });
}

void onSubscribed(String topic) {
  print('EXAMPLE::Subscription confirmed for topic $topic');
}

void onDisconnected() {
  print('EXAMPLE::OnDisconnected client callback - Client disconnection');
  if (client.connectionStatus!.disconnectionOrigin ==
      MqttDisconnectionOrigin.solicited) {
    print('EXAMPLE::OnDisconnected callback is solicited, this is correct');
  } else {
    print(
      'EXAMPLE::OnDisconnected callback is unsolicited or none, this is incorrect - exiting',
    );
    exit(-1);
  }
  if (pongCount == 3) {
    print('EXAMPLE:: Pong count is correct');
  } else {
    print('EXAMPLE:: Pong count is incorrect, expected 3. actual $pongCount');
  }
  if (pingCount == 3) {
    print('EXAMPLE:: Ping count is correct');
  } else {
    print('EXAMPLE:: Ping count is incorrect, expected 3. actual $pingCount');
  }
}

void onConnected() {
  print(
    'EXAMPLE::OnConnected client callback - Client connection was successful',
  );
}

void pong() {
  print('EXAMPLE::Ping response client callback invoked');
  pongCount++;
  print(
    'EXAMPLE::Latency of this ping/pong cycle is ${client.lastCycleLatency} milliseconds',
  );
}

void ping() {
  print('EXAMPLE::Ping sent client callback invoked');
  pingCount++;
}
