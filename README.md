# WiiLÖVE
An *awesome* framework for making 2D [Nintendo Wii](https://en.wikipedia.org/wiki/Wii) homebrew games in [Lua](https://www.lua.org/manual/5.1/). Its API is based on [LÖVE](https://love2d.org/), but its functions are mostly written from scratch and optimized for the Wii.

```lua
function love.draw()
	love.graphics.print("WiiLÖVE is awesome!", 320, 240)
end
```

# Why is it awesome?
Wii homebrew has a lot of potential, but unfortunately it has a learning curve. Not only do you need to learn C/C++, but you also need to set up a toolchain and a proper development environment. You also have to learn how to use the Wii homebrew libraries and their many different functions.

WiiLÖVE aims to make this much easier, at least for 2D game development, by making it so that you don't have to do any of this. All you have to do is learn Lua, an easy-to-learn and flexible programming language, write your game with WiiLÖVE's versatile API, and run it from your SD card with WiiLÖVE's binary, both during development and for your finished product. It will automatically compile and run your game for you, no complex setup needed.

WiiLÖVE also has a built-in error handler, making game development much less messy and easier to debug. It also allows your game to cleanly exit back to its loader (for example, the Homebrew Channel) if it crashes.

# Setup
Download the latest release and extract it to a folder on your SD card under the name of your game.

Create a `data` folder in your game folder and start writing your game code in a `main.lua` file!

TODO: Rewrite with more instructions

# Docs
TODO: Write docs and add link

# Examples
Check out some open-source projects that use WiiLÖVE!
* [Wiirdle](https://github.com/HTV04/wiirdle): Wordle clone for the Wii made to demonstrate WiiLÖVE's features
