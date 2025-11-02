import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:iaq_app/provider/provider.dart';

class MyDrawer extends ConsumerWidget {
  const MyDrawer({super.key});

  @override
  Widget build(BuildContext context, ref) {
    final themeMode = ref.watch(themeProvider);

    return Drawer(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.end,
        mainAxisSize: MainAxisSize.max,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              TextButton(onPressed: () async {}, child: Text("Logout")),
              IconButton(
                onPressed: () {
                  ref.read(themeProvider.notifier).changeTheme();
                },
                icon: Icon(
                  themeMode != ThemeMode.light ? Icons.sunny : Icons.dark_mode,
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }
}
