# Waffle the Windows API Filtering Layer

### Waffle is an advanced Windows API hooking framework.

### Download [the latest stable release](https://github.com/MakiseKurisu/WaffleNightly/releases) or [the latest nightly build](https://github.com/MakiseKurisu/WaffleNightly/archive/master.zip).

## Introduction

Working on Ring3, Waffle provides two ways to detour a function call: inline/hot-patch hooking, or VEH-based exception detour. Inline hooking is the classical way and provides the best performance, while exception detour works for virtually any function and supports two different exceptions. Also, Waffle is bulit with cross-platform in mind. Not only for x86 programs, Waffle also supports x64 and ARM (on jailbroken Windows RT) programs. The support for IA-64 is incomplete but Waffle is buildable under IA-64 toolchain.

At current stage Waffle only supports to hook the process that created by Waffle. Waffle handles code injection, plugin backward compatibility, and how to hook the function ---- it is written in a config file so you don't even need to write a single line of code for hooking. Evenmore, Waffle knows if your plugin calls a hooked function and automactially detours this function call to the unhooked version. It is amazing to see API hooking can be this easy.

Currently Waffle is in the pre-alpha stage. I don't want to call it even alpha since the APIs haven't yet been well redesigned and documented. However, it is stable enough that I made 2 working plugins and 1 sample plugin.

### Mojibake

Mojibake helps you to run ANSI programs properly.

![Mojibake on Windows RT](http://ww1.sinaimg.cn/large/6ddd8e3dgw1efdlk7mtclj211y0lctfd.jpg)

### PomiCo!

PomiCo! is a fan-made Simplified Chinese translation patch for [ComiPo!](http://www.comipo.com/en/index.html)

![PomiCo! on Windows 7](http://ww3.sinaimg.cn/large/6ddd8e3dgw1eflwb9c4gzj20sh0ivad8.jpg)

### nullstub

null

## Support

### Waffle

Seriously? It is in the pre-alpha stage!

### Mojibake

[Locale Emulator](https://github.com/xupefei/Locale-Emulator) is a better solution. Under active development.

My origional purpose for Waffle and Mojibake is to run LiveMaker-based games. After I made my first preview release and gave a solution, Locale Emulator added this feature in 1.1.1.0. So I no longer really have to write my own one.

The only disadvantage of Locale Emulator is it supports x86 only. [NTLEAS](https://ntlea.codeplex.com/) supports x86 and x64, while not as compatible as Locale Emulator.

### PomiCo!

Well the truth is I cannot speak Japanese. I used Google Translation and my Chinese knowledge to made this patch. This is horrible. I definitely won't add one more translation except update with latest ComiPo! resource, especially since I am no longer interested in this software.

### nullstub

null
