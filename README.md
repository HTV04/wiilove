# WiiLÖVE
[![GitHub release (latest SemVer including pre-releases)](https://img.shields.io/github/v/release/HTV04/wiilove?include_prereleases&style=flat-square)](https://github.com/HTV04/wiilove/releases) [![GitHub issues](https://img.shields.io/github/issues/HTV04/wiilove?style=flat-square)](https://github.com/HTV04/wiilove/issues) [![GitHub pull requests](https://img.shields.io/github/issues-pr/HTV04/wiilove?style=flat-square)](https://github.com/HTV04/wiilove/pulls) [![GitHub](https://img.shields.io/github/license/HTV04/wiilove?style=flat-square)](https://github.com/HTV04/wiilove/blob/main/LICENSE) [![Discord](https://img.shields.io/discord/852658576577003550?style=flat-square)](https://discord.gg/tQGzN2Wu48)

An *awesome* framework for making 2D [Nintendo Wii](https://en.wikipedia.org/wiki/Wii) homebrew games in [Lua](https://www.lua.org/). Its API is based on [LÖVE](https://love2d.org/), but its functions are mostly written from scratch and optimized for the Wii.

```lua
function love.draw()
	love.graphics.print("WiiLÖVE is awesome!", 320, 240)
end
```

# Why is it awesome?
The Wii has a lot of potential for homebrew development, but unfortunately getting started requires a lot of work. Not only do you need to learn C/C++, but you also need to set up a toolchain and a proper development environment. You also have to learn how to use many different Wii homebrew libraries and functions. What if there was an easier way to start developing a Wii homebrew game?

WiiLÖVE aims to make this much easier, at least for 2D game development. All you have to do is [learn Lua](https://www.lua.org/manual/5.1/), an easy-to-learn and flexible programming language, write your game with WiiLÖVE's versatile API, and run it from your SD card with WiiLÖVE's binary, both during development and for your finished product. It will automatically compile and run your game for you, no complex setup needed.

WiiLÖVE also has a built-in error handler, making game development much easier to debug. It also allows your game to cleanly exit back to its loader (for example, the Homebrew Channel) if it crashes.

# Setup
## Standard
* Download the [latest `wiilove.zip`](https://github.com/HTV04/wiilove/releases/latest) and extract it to the root of your SD card.
* Rename the `supergame` folder to something else (for example, the name of your game).
* Start writing your game using the `main.lua` template! Add and edit files as needed, such as new script files and the icon and metadata.
* Test your game by running it from the Homebrew Channel.

## Dolphin
* Download the [latest `wiilove-dolphin.zip`](https://github.com/HTV04/wiilove/releases/latest) and extract the contents of the `sd` folder to the root of your virtual SD card.
* Start writing your game using the `main.lua` template! Add and edit files as needed, such as new script files and the icon and metadata.
* Test your game by launching the `wiilove.dol` file in Dolphin.
* Once your project is complete, refer to the [standard setup](#standard) for structuring an app for the Homebrew Channel.

# Resources
Since there isn't a guide for WiiLÖVE yet, I recommend checking out Sheepolution's [*How to LÖVE*](https://www.sheepolution.com/learn/book/contents) guide, and then accounting for the differences in WiiLÖVE's API.

Docs are currently a WIP. Since WiiLÖVE is still in an alpha state, its functions are constantly updating, so docs are a low priority at the moment. In the meantime, see [`src/main.cpp`](src/main.cpp) and [`data/api.lua`](data/api.lua) for the names of all of WiiLÖVE's functions.

Check out some open-source projects that use WiiLÖVE!
* [Wiirdle](https://github.com/HTV04/wiirdle) - Wordle clone for the Wii made to demonstrate WiiLÖVE's features

# Building
Before following these instructions, you will need to [install devkitPro and devkitPPC](https://devkitpro.org/wiki/Getting_Started).

* Add the `htv-dkp-libs` repository. This will allow you to install WiiLÖVE's dependencies from pacman. Follow the instructions [here](https://github.com/HTV04/htv-dkp-libs).
* Ensure the following libraries are installed:
  * `libogc-mod`
  * `libfat-mod-ogc`
  * `wii-luajit`
  * `wii-audiogc`
  * `wii-grrlib-mod`
  * `ppc-freetype`
* Run `make` (or `make dist` to create release ZIPs).
* Compiled binaries will be available in `bin` (and release ZIPs will be available in `dist`).

## Build options
Append these to `make` to enable them:

* **WIP:** `WIILOVE_MODE=debug`: Enables features useful for debugging.
  * Currrently does nothing other than change the internal mode to "debug," but will do more in the future.
* **EXPERIMENTAL:** `WIILOVE_MODE=final`: Increases performance by not checking for certain errors, and not falling back to the error handler.
  * Only use this for finalized games or if the extra performance is absolutely necessary. It may cause hard crashes if there are mistakes or errors.
    * **DO NOT** use this mode for games that can use external code (for example, via a mod loader) however, because it may cause hard crashes if the external code has errors or is malicious. The safety features are definitely recommended for external code, especially because they can allow games to handle errors gracefully.

* `WIILOVE_BUILD=debug`: Make a debug build, disabling optimizations and enabling debug symbols.
  * Will slow down WiiLÖVE's performance, but will make debugging easier.
* `WIILOVE_BUILD=unity`: Make a unity build (all source files compiled as one).
  * May use more memory during compilation, but may also improve compile times and code generation.
  * Recommended for release builds, but not recommended for debugging because it makes it harder to find the source of errors, among other issues with combining all source files.

* `WIILOVE_LUA=minify`: Minify internal Lua source files, reducing the size of the compiled binary.
  * Requires [luamin](https://github.com/mathiasbynens/luamin#using-the-luamin-binary).
  * Recommended for release builds, but not recommended for debugging because some information is lost during minification.

Build options used for official releases: `WIILOVE_BUILD=unity WIILOVE_LUA=minify`.

# License
WiiLÖVE is licensed under the [GNU Lesser General Public License v3.0](LICENSE). Therefore, modifications to WiiLÖVE must be open-source and licensed under the same license. However, projects and files that interact with WiiLÖVE externally (for example, Lua scripts that WiiLÖVE runs) are not required to be open-source and can use any license.

Exceptions to this license are the files in the [`game`](game) folder, which are meant to be a project template. These files are unlicensed and free-to-use for any purpose.
