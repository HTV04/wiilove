--[[----------------------------------------------------------------------------
WiiLÖVE boot.lua

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

local result

-- Redirect package paths
package.path = "save/?.lua; save/?/init.lua; data/?.lua; data/?/init.lua"
package.cpath = "" -- Disable C modules

-- Standard callback handlers
love.handlers = setmetatable({
	-- WIP: These handlers may change in the future
	gamepadpressed = function(j,b) -- Unused for now
		if love.gamepadpressed then return love.gamepadpressed(j,b) end
	end,
	gamepadreleased = function(j,b) -- Unused for now
		if love.gamepadreleased then return love.gamepadreleased(j,b) end
	end,
	gamepadaxis = function(j,a,v) -- Unused for now
		if love.gamepadaxis then return love.gamepadaxis(j,a,v) end
	end,
	wiimoteconnected = function(w)
		if love.wiimoteconnected then return love.wiimoteconnected(w) end
	end,
	wiimotedisconnected = function(w)
		if love.wiimotedisconnected then return love.wiimotedisconnected(w) end
	end,
	joystickadded = function(j) -- Unused for now
		if love.joystickadded then return love.joystickadded(j) end
	end,
	joystickremoved = function(j) -- Unused for now
		if love.joystickremoved then return love.joystickremoved(j) end
	end,
	keypressed = function(b,s,r) -- Unused for now
		if love.keypressed then return love.keypressed(b,s,r) end
	end,
	keyreleased = function(b,s) -- Unused for now
		if love.keyreleased then return love.keyreleased(b,s) end
	end,
	textinput = function(t) -- Unused for now
		if love.textinput then return love.textinput(t) end
	end,
	textedited = function(t,s,l) -- Unused for now
		if love.textedited then return love.textedited(t,s,l) end
	end,
}, {
	__index = function(self, name)
		error("Unknown event: " .. name)
	end,
})

function love.run()
	local dt = 0

	if love.load then love.load() end

	-- We don't want the first frame's dt to include time taken by love.load
	if love.timer then love.timer.step() end

	-- Main loop
	while true do
		-- Process events.
		if love.event then
			love.event.pump()

			for name, a,b,c,d,e,f in love.event.poll do
				if name == "homepressed" then
					if not love.homepressed or not love.homepressed(a) then
						return 0
					end
				else
					love.handlers[name](a,b,c,d,e,f)
				end
			end
		end

		-- Update dt, as we'll be passing it to update
		if love.timer then dt = love.timer.step() end

		-- Update
		if love.update then love.update(dt) end

		-- Draw
		love.graphics.origin()
		love.graphics.clear()

		if love.draw then love.draw() end

		love.graphics.present()
	end
end

function love.errhand(err)
	local msg = "Error\n\n" ..
	            tostring(err) ..
				"\n\n\n" ..
				string.gsub(string.gsub(debug.traceback(), "\t", ""), "stack traceback:", "Traceback\n") ..
				"\n\n\nPress HOME to return to loader\n"
	local msgTable = {}

	-- Temporary workaround for newlines
	for line in string.gmatch(msg, "([^\n]*)\n") do
		table.insert(msgTable, line)
	end

	-- Stop all Wiimote vibrations
	if love.system.getConsole ~= "GameCube" then
		for _, wiimote in ipairs(love.wiimote.getWiimotes()) do
			wiimote:setRumble(false)
		end
	end

	--love.audio.stop()

	love.graphics.reset()
	love.graphics.setNewFont(14)

	while true do
		love.event.pump()

		for name, a,b,c,d,e,f in love.event.poll do
			if name == "homepressed" then
				return 1
			end
		end

		love.graphics.clear(89, 157, 220)

		for i, line in ipairs(msgTable) do
			love.graphics.print(line, 70, 60 + i * 18)
		end

		love.graphics.present()
	end
end

-- Load main.lua to intialize functions
xpcall(love.filesystem.load("main.lua"), love.errhand)

_, result = xpcall(love.run, love.errhand)

return result
