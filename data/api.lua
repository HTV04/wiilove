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
love.graphics.newTexture = _Texture.new

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
_Font = nil
_Texture = nil
