import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/pages/model.dart';
import 'package:iaq_app/provider/provider.dart';
import 'package:iaq_app/provider/router.dart';

class CardWidget extends ConsumerWidget {
  const CardWidget({super.key, required this.data});

  final DataClass data;
  @override
  Widget build(BuildContext context, WidgetRef ref) {
    // final templateData = ref.watch(templateDataClassProvider);
    final mqttData = ref.watch(mqttDataProvider);
    return Material(
      elevation: 5,
      child: Container(
        margin: EdgeInsets.all(5),
        padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 8),
        decoration: BoxDecoration(
          color: Theme.of(context).primaryColor.withOpacity(0.2),
          borderRadius: BorderRadius.circular(5),
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          mainAxisSize: MainAxisSize.max,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                const SizedBox(width: 10),
                Image.asset(data.icon, height: 40),
                const SizedBox(width: 10),
                Expanded(
                  child: Text(
                    data.text,
                    overflow: TextOverflow.ellipsis,
                    style: Theme.of(context).textTheme.bodyLarge?.copyWith(),
                  ),
                ),
              ],
            ),
            const SizedBox(height: 10),
            Text(
              mqttData.getData(data.id).toStringAsFixed(2),
              textAlign: TextAlign.center,
              style: Theme.of(context).textTheme.headlineLarge?.copyWith(
                fontSize: 25,
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 20),
            Text(
              "The ${data.text} is quite high",
              style: Theme.of(context).textTheme.bodySmall?.copyWith(),
            ),
          ],
        ),
      ),
    );
  }
}
