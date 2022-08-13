--[[----------------------------------------------------------------------------
WiiLÖVE api.lua

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

local love = love

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
		local _
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:pause()
		end
	end
	function love.audio.play(...)
		local _
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:play()
		end
	end
	function love.audio.resume(...)
		local _
		local sourceList = {...}

		for _, source in ipairs(sourceList) do
			source:resume()
		end
	end
	function love.audio.stop(...)
		local _
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
function love.filesystem.load(filename)
	return loadstring(love.filesystem.read(filename), filename)
end

-- love.graphics
do
	local newFont = love.graphics.newFont
	local setFont = love.graphics.setFont

	function love.graphics.setNewFont(...)
		local font = newFont(...)

		setFont(font)

		return font
	end
end

-- Delete global usertypes
_Source = nil
_Font = nil
_Quad = nil
_Texture = nil
