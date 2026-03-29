import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:iaq_app/pages/model.dart';
import 'package:iaq_app/provider/provider.dart';
import 'package:iaq_app/utils/aqi_calculator.dart';

class CardWidget extends ConsumerWidget {
  const CardWidget({super.key, required this.data});

  final DataClass data;
  @override
  Widget build(BuildContext context, WidgetRef ref) {
    // final templateData = ref.watch(templateDataClassProvider);
    final mqttData = ref.watch(mqttDataProvider);
    final value = mqttData.getData(data.id);
    final aqiInfo = AQICalculator.getAQI(data.id, value);

    return Container(
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(20),
        gradient: LinearGradient(
          colors: [
            Theme.of(context).colorScheme.primaryContainer.withOpacity(0.8),
            Theme.of(context).colorScheme.primaryContainer.withOpacity(0.4),
          ],
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
        ),
        boxShadow: [
          BoxShadow(
            color: Theme.of(context).shadowColor.withOpacity(0.1),
            blurRadius: 15,
            offset: const Offset(0, 8),
          ),
        ],
        border: Border.all(
          color: Theme.of(context).colorScheme.outline.withOpacity(0.1),
          width: 1,
        ),
      ),
      child: Material(
        color: Colors.transparent,
        borderRadius: BorderRadius.circular(20),
        child: InkWell(
          borderRadius: BorderRadius.circular(20),
          onTap: () {
            context.go('/sensor/${data.id}');
          },
          child: Padding(
            padding: const EdgeInsets.symmetric(vertical: 16, horizontal: 16),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [
                Row(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Container(
                      padding: const EdgeInsets.all(8),
                      decoration: BoxDecoration(
                        color: Theme.of(
                          context,
                        ).colorScheme.surface.withOpacity(0.5),
                        borderRadius: BorderRadius.circular(12),
                      ),
                      child: Image.asset(data.icon, height: 32, width: 32),
                    ),
                    const SizedBox(width: 8),
                    Expanded(
                      child: Text(
                        data.text,
                        maxLines: 2,
                        overflow: TextOverflow.ellipsis,
                        style: Theme.of(context).textTheme.titleSmall?.copyWith(
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                    ),
                    if (aqiInfo.level != "N/A")
                      Container(
                        padding: const EdgeInsets.symmetric(
                          horizontal: 6,
                          vertical: 3,
                        ),
                        decoration: BoxDecoration(
                          color: aqiInfo.color,
                          borderRadius: BorderRadius.circular(8),
                        ),
                        child: Text(
                          "AQI ${aqiInfo.index}",
                          style: Theme.of(context).textTheme.labelSmall
                              ?.copyWith(
                                color: aqiInfo.onColor,
                                fontWeight: FontWeight.w800,
                                letterSpacing: 0.1,
                              ),
                        ),
                      ),
                  ],
                ),
                Expanded(
                  child: Center(
                    child: Column(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Text(
                          value.toStringAsFixed(2),
                          textAlign: TextAlign.center,
                          style: Theme.of(context).textTheme.displaySmall
                              ?.copyWith(
                                fontWeight: FontWeight.w800,
                                letterSpacing: -1,
                                color: aqiInfo.level != "N/A"
                                    ? aqiInfo.color
                                    : null,
                              ),
                        ),
                        const SizedBox(height: 4),
                        Text(
                          data.unit.toUpperCase(),
                          style: Theme.of(context).textTheme.bodySmall
                              ?.copyWith(
                                color: Theme.of(
                                  context,
                                ).colorScheme.onSurfaceVariant,
                                fontWeight: FontWeight.w500,
                              ),
                        ),
                      ],
                    ),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
