--[[----------------------------------------------------------------------------
WiiLÖVE Lua API

This file is part of WiiLÖVE.

Copyright (C) 2022  HTV04

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
<https://www.gnu.org/licenses/>.
------------------------------------------------------------------------------]]

-- Lua function cache
local ipairs = ipairs
local loadstring = loadstring
local setmetatable = setmetatable

-- Global usertype workaround
love.graphics.newFont = _Font.new
love.graphics.newQuad = _Quad.new
love.graphics.newTexture = _Texture.new

-- love.audio
do
	local newSource = _Source.new

	local sources = {}
	setmetatable(sources, {__mode = "k"})

	love.audio = {}

	-- Custom newSource function that registers the Source
	function love.audio.newSource(...)
		local source = newSource(...)

		sources[source] = true

		return source
	end

	function love.audio.pause(...)
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:pause()
		end
	end
	function love.audio.play(...)
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:play()
		end
	end
	function love.audio.resume(...)
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:resume()
		end
	end
	function love.audio.stop(...)
		local sourceList = {...}

		if #sourceList == 0 then
			for source in pairs(sources) do
				source:stop()
			end
		else
			for _, source in ipairs(sourceList) do
				source:stop()
			end
		end
	end

	function love.audio.getActiveSourceCount()
		local count = 0

		for source in pairs(sources) do
			if source:isPlaying() then count = count + 1 end
		end

		return count
	end
end

-- love.filesystem
do
	local read = love.filesystem.read

	function love.filesystem.load(filename)
		return loadstring(read(filename), filename)
	end
end

-- love.graphics
do
	local clear = love.graphics.clear
	local setBackgroundColor = love.graphics.setBackgroundColor
	local setColor = love.graphics.setColor

	local ellipse = love.graphics.ellipse

	local print = love.graphics.print

	local draw = love.graphics.draw
	local drawQuad = love.graphics.drawQuad

	local newFont = love.graphics.newFont
	local setFont = love.graphics.setFont

	function love.graphics.clear(r, g, b, a)
		a = a or 255

		clear(r, g, b, a)
	end
	function love.graphics.setBackgroundColor(r, g, b, a)
		a = a or 255

		setBackgroundColor(r, g, b, a)
	end
	function love.graphics.setColor(r, g, b, a)
		a = a or 255

		setColor(r, g, b, a)
	end

	function love.graphics.circle(fill, x, y, radius)
		ellipse(fill, x, y, radius, radius)
	end

	function love.graphics.print(text, x, y, r, sx, sy, ox, oy)
		x = x or 0
		y = y or 0
		r = r or 0
		sx = sx or 1
		sy = sy or 1
		ox = ox or 0
		oy = oy or 0

		print(text, x, y, r, sx, sy, ox, oy)
	end

	function love.graphics.draw(texture, x, y, r, sx, sy, ox, oy)
		x = x or 0
		y = y or 0
		r = r or 0
		sx = sx or 1
		sy = sy or 1
		ox = ox or 0
		oy = oy or 0

		draw(texture, x, y, r, sx, sy, ox, oy)
	end
	function love.graphics.drawQuad(texture, textureQuad, x, y, r, sx, sy, ox, oy)
		x = x or 0
		y = y or 0
		r = r or 0
		sx = sx or 1
		sy = sy or 1
		ox = ox or 0
		oy = oy or 0

		drawQuad(texture, textureQuad, x, y, r, sx, sy, ox, oy)
	end

	function love.graphics.setNewFont(...)
		local font = newFont(...)

		setFont(font)

		return font
	end
end

do
	local random = love.math.random

	function love.math.random(min, max)
		if min and (not max) then
			max = min
			min = 1
		end

		random(min, max)
	end
end

-- Delete global usertypes
_Source = nil
_Font = nil
_Quad = nil
_Texture = nil

return love
