import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_datetime_picker_plus/flutter_datetime_picker_plus.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/pages/model.dart';
import 'package:iaq_app/pages/widget.dart';
import 'package:iaq_app/provider/provider.dart';
import 'package:intl/intl.dart';
import 'package:syncfusion_flutter_charts/charts.dart';

class SensorChartPage extends ConsumerStatefulWidget {
  const SensorChartPage({super.key, required this.paramId});
  final String paramId;

  @override
  ConsumerState<SensorChartPage> createState() => _SensorChartPageState();
}

class _SensorChartPageState extends ConsumerState<SensorChartPage> {
  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final sensorData = ref.watch(sensorDateProvider);

    return Scaffold(
      appBar: AppBar(
        title: Text(
          DataClass.getDAtaFromId(widget.paramId).text,
          style: Theme.of(context).textTheme.titleLarge?.copyWith(
            fontWeight: FontWeight.w700,
            letterSpacing: -0.5,
          ),
        ),
        centerTitle: true,
        backgroundColor: Colors.transparent,
        elevation: 0,
        surfaceTintColor: Colors.transparent,
        leading: Builder(
          builder: (context) => IconButton(
            icon: Icon(
              Icons.arrow_back_ios_new_rounded,
              size: 20,
              color: Theme.of(context).colorScheme.onSurface,
            ),
            onPressed: () => Navigator.of(context).maybePop(),
          ),
        ),
      ),
      body: SafeArea(
        child: sensorData.when<Widget>(
          error: (error, stackTrace) {
            return Center(child: Text("Error loading sensor data: $error"));
          },
          loading: () {
            return const Center(child: CircularProgressIndicator());
          },
          data: (data) {
            return Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                DateTimePickerWidget(),
                const SizedBox(height: 20),
                Expanded(
                  child: IAQChart(paramId: widget.paramId, data: data),
                ),
                const SizedBox(height: 20),
              ],
            );
          },
        ),
      ),
    );
  }
}

class IAQChart extends ConsumerWidget {
  IAQChart({super.key, required this.paramId, required this.data});

  final String paramId;
  final List<TemplateData> data;

  final _zoomPanBehavior = ZoomPanBehavior(
    enablePinching: true,
    enablePanning: true,
    enableDoubleTapZooming: true,
    zoomMode: ZoomMode.x,
    enableMouseWheelZooming: true,
    // enableDirectionalZooming: true,
    // enableSelectionZooming: true
  );
  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final currentStartDate = ref.watch(startDateTimeProvider);
    final currentEndDate = ref.watch(endDateTimeProvider);
    ref.watch(sensorDateProvider);
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 16),

      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.surfaceContainerHigh,
        borderRadius: BorderRadius.circular(24),
        border: Border.all(
          color: Theme.of(context).colorScheme.outlineVariant.withOpacity(0.4),
          width: 1,
        ),
      ),
      padding: const EdgeInsets.fromLTRB(12, 16, 12, 8),

      child: SfCartesianChart(
        plotAreaBackgroundColor: Colors.transparent,

        zoomPanBehavior: _zoomPanBehavior,
        backgroundColor: Colors.transparent,
        primaryXAxis: CategoryAxis(
          labelStyle: Theme.of(context).textTheme.labelSmall?.copyWith(
            color: Theme.of(context).colorScheme.onSurfaceVariant,
          ),
          axisLine: AxisLine(
            color: Theme.of(context).colorScheme.outlineVariant,
          ),
          majorGridLines: MajorGridLines(
            color: Theme.of(
              context,
            ).colorScheme.outlineVariant.withOpacity(0.3),
          ),
        ),
        primaryYAxis: NumericAxis(
          labelStyle: Theme.of(context).textTheme.labelSmall?.copyWith(
            color: Theme.of(context).colorScheme.onSurfaceVariant,
          ),
          axisLine: AxisLine(
            color: Theme.of(context).colorScheme.outlineVariant,
          ),
          majorGridLines: MajorGridLines(
            color: Theme.of(
              context,
            ).colorScheme.outlineVariant.withOpacity(0.3),
          ),
        ),
        title: ChartTitle(
          text:
              '${DataClass.getDAtaFromId(paramId).text} Overview\n${DateFormat('MMM d, HH:mm').format(currentStartDate)} to ${DateFormat('MMM d, HH:mm').format(currentEndDate)}',
          alignment: ChartAlignment.center,
          textStyle: Theme.of(context).textTheme.labelLarge?.copyWith(
            color: Theme.of(context).colorScheme.onSurfaceVariant,
            fontWeight: FontWeight.w700,
            height: 1.4,
          ),
        ),
        // Chart title
        legend: Legend(isVisible: true),
        // Enable tooltip
        tooltipBehavior: TooltipBehavior(
          enable: true,
          color: Theme.of(context).colorScheme.inverseSurface,
          textStyle: TextStyle(
            color: Theme.of(context).colorScheme.onInverseSurface,
            fontSize: 12,
            fontWeight: FontWeight.w600,
          ),
        ),
        series: <CartesianSeries<TemplateData, String>>[
          LineSeries<TemplateData, String>(
            dataSource: data,
            xValueMapper: (TemplateData data, _) =>
                DateFormat('HH:mm:ss').format(data.timestamp),
            yValueMapper: (TemplateData data, _) => data.getData(paramId),
            name: DataClass.getDAtaFromId(paramId).text,
            // Enable data label
            dataLabelSettings: DataLabelSettings(isVisible: true),
          ),
        ],
      ),
    );
  }
}

class DateTimePickerWidget extends ConsumerWidget {
  const DateTimePickerWidget({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final currentStartDate = ref.watch(startDateTimeProvider);
    final currentEndDate = ref.watch(endDateTimeProvider);
    final cs = Theme.of(context).colorScheme;
    final tt = Theme.of(context).textTheme;

    return Container(
      padding: const EdgeInsets.symmetric(vertical: 16, horizontal: 20),
      margin: const EdgeInsets.symmetric(horizontal: 16),
      decoration: BoxDecoration(
        color: cs.surfaceContainerHigh,
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: cs.outlineVariant.withOpacity(0.4), width: 1),
      ),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Row(
            children: [
              Icon(
                Icons.access_time_filled_rounded,
                size: 18,
                color: cs.primary,
              ),
              const SizedBox(width: 8),
              Text(
                'Time Range',
                style: tt.titleMedium?.copyWith(
                  fontWeight: FontWeight.w700,
                  letterSpacing: 0.2,
                ),
              ),
            ],
          ),
          const SizedBox(height: 12),
          Container(
            padding: const EdgeInsets.symmetric(vertical: 10, horizontal: 12),
            decoration: BoxDecoration(
              color: cs.surfaceContainerHighest,
              borderRadius: BorderRadius.circular(12),
            ),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text(
                  DateFormat('MMM d, HH:mm').format(currentStartDate),
                  style: tt.labelLarge?.copyWith(fontWeight: FontWeight.w600),
                ),
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 12),
                  child: Icon(
                    Icons.arrow_forward_rounded,
                    size: 16,
                    color: cs.onSurfaceVariant,
                  ),
                ),
                Text(
                  DateFormat('MMM d, HH:mm').format(currentEndDate),
                  style: tt.labelLarge?.copyWith(fontWeight: FontWeight.w600),
                ),
              ],
            ),
          ),
          const SizedBox(height: 16),
          Row(
            children: [
              Expanded(child: StartDateTimePickerWidget()),
              const SizedBox(width: 12),
              Expanded(child: EndDateTimePickerWidget()),
            ],
          ),
        ],
      ),
    );
  }
}

class StartDateTimePickerWidget extends ConsumerWidget {
  const StartDateTimePickerWidget({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return FilledButton.tonalIcon(
      style: FilledButton.styleFrom(
        minimumSize: const Size(double.infinity, 44),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      ),
      icon: const Icon(Icons.play_circle_filled_rounded, size: 18),
      label: const Text(
        'Start Time',
        style: TextStyle(fontWeight: FontWeight.w600),
      ),
      onPressed: () {
        DatePicker.showDateTimePicker(
          context,
          showTitleActions: true,
          minTime: DateTime(2026, 02, 26),
          maxTime: DateTime.now(),
          onConfirm: (date) {
            ref.read(startDateTimeProvider.notifier).setDateTime(date);
          },
          currentTime: DateTime.now(),
        );
      },
    );
  }
}

class EndDateTimePickerWidget extends ConsumerWidget {
  const EndDateTimePickerWidget({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return FilledButton.tonalIcon(
      style: FilledButton.styleFrom(
        minimumSize: const Size(double.infinity, 44),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      ),
      icon: const Icon(Icons.stop_circle_rounded, size: 18),
      label: const Text(
        'End Time',
        style: TextStyle(fontWeight: FontWeight.w600),
      ),
      onPressed: () {
        DatePicker.showDateTimePicker(
          context,
          showTitleActions: true,
          minTime: DateTime(2026, 02, 26),
          maxTime: DateTime.now(),
          onConfirm: (date) {
            ref.read(endDateTimeProvider.notifier).setDateTime(date);
          },
          currentTime: DateTime.now(),
        );
      },
    );
  }
}
// class SensorChartPage extends ConsumerWidget {
//   const SensorChartPage({super.key, required this.paramId});
//   final String paramId;

//   @override
//   Widget build(BuildContext context, WidgetRef ref) {}
// }

class SensorPage extends ConsumerWidget {
  const SensorPage({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    ref.watch(mqttClientProvider);

    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      mainAxisSize: MainAxisSize.max,
      children: <Widget>[
        // Text(
        //   text,
        //   style: Theme.of(context).textTheme.headlineMedium,
        // ),
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
