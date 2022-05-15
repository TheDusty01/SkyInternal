# SkyInternal 
This is a very basic internal cheat client for CS:GO I've written with the help of [@Bind0s](https://github.com/Bind0s). Some parts are based off various other cheats found online.

I wouldn't recommend to use this cheat as it is probably not fully VAC proof nor does it have many features apart from a really basic wallhack. You could consider this repo more like of POC thing which you could use as a starting point.

## Motivation
We wanted to get a deeper understanding on how cheats work in general, from the rendering methods, the hooking to the injection itself while staying undetected. There are far more advanced cheats in existence but that wasn't the overall goal of this project.

We decided to publish it now since we both don't have time to work on this anymore.

### Missing/Planned features
- Custom injector/loader
- Custom offset dumper (retrieve offsets when injected)
- Better AC proof methods (e.g. direct sys calls for ``GetModuleHandle`` and typical hooked functions)
- More modules (Aimbot, Skin changer etc.)
- Controls/key config

## Setup
1. Dump the needed offsets with [frk1/hazedumper-rs](https://github.com/frk1/hazedumper-rs). Or use the already dumped offsets here: [frk1/hazedumper](https://github.com/frk1/hazedumper) (may not be up to date)
2. Replace the contents of [Offsets.h](SkyInternal/src/Sky/Game/Offsets.h) with new offsets
3. Build the solution using Visual Studio in Release mode

## Usage
1. Use an injector to inject the DLL (like [GH Injector](https://guidedhacking.com/resources/guided-hacking-dll-injector.4/) or write your own basic injector like we did, just google a bit)
2. Inject the ``SkyInternal.dll``
3. Open the cheat menu using the ``F8`` key
4. Uninject using the ``END`` key

**Note:** When you finished using the cheat you should restart your PC before playing with VAC again.

## License
SkyInternal is licensed under Apache License 2.0, see [LICENSE.txt](/LICENSE.txt) for more information.