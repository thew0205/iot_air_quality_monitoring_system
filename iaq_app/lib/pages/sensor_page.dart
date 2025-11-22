import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/pages/model.dart';
import 'package:iaq_app/pages/widget.dart';
import 'package:iaq_app/provider/provider.dart';

class SensorPage extends ConsumerWidget {
  const SensorPage({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    ref.watch(mqttClientProvider);

    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      mainAxisSize: MainAxisSize.max,
      children: <Widget>[
        Expanded(
          child: GridView.builder(
            itemCount: DataClass.para.length,
            gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
              crossAxisSpacing: 10,
              mainAxisSpacing: 10,
              maxCrossAxisExtent: 250,
            ),
            itemBuilder: (context, index) {
              return CardWidget(data: DataClass.para[index]);
            },
          ),
        ),
      ],
    );
  }
}
