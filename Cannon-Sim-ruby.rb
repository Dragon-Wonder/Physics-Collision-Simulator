#!/usr/bin/ruby

# Make some global variables for later use.
$Program_version = "1.0.0"
$Debug_Mode = 0 # Enables Debug mode which prints more messages to the console.
$Rad_Convert = Math::PI / 180 # Converts Degrees to radians

# main code
print "Cannon simulation\n"
print "Made by: GamerMan7799\n"
print "Version: #{$Program_version}\n"
print "Debug Mode on\n" if $Debug_Mode == 1

_temp = 0 #temp values for storing inputs

# values for creating the cannonball
_xplace = 0
_yplace = 1
_vinit = 0
_angleinit = 0
_radius = 1

print "\n\n"

# Start asking user for values

# Changing the starting position of the ball is currently disabled.
=begin
print "Please enter the starting x of the cannonball: "
_temp = gets
_xplace = _temp.to_i
print "Please enter the starting y of the cannonball: "
_temp = gets
_yplace = _temp.to_i
=end

print "Please enter the initial veloctiy of the ball (m/s): "
_temp = gets
_vinit = _temp.to_i
print "Please enter the angle of fire for the cannon (°): "
_temp = gets
_angleinit = _temp.to_i

#drag mode is currently not in this version (will be added in later one)
print"Would you like to enable drag? (1 for yes, 0 for no): "
_temp = gets
_enableddrag = _temp.to_i

print "Please enter a radius for the cannonball (meters): "
_temp = gets
_radius = _temp.to_f

print "\n"
Tick = ClsTick.new
Cannonball = ClsCannonball.new(_xplace, _yplace, _vinit, _angleinit, _radius, _enableddrag)

Cannonball.setdeltat(Tick.getdeltat)

while Cannonball.getPlace_Y > 0 do
	Cannonball.update
	Tick.wait
end #end while

#end of code.

BEGIN {
	# declare all the classes that I will need

	# Tick = Time Class
	class ClsTick
		@FPSgoal
		@SleepTime
		def initialize (value = 30)
			@FPSgoal = value
			@SleepTime = (1.0 / @FPSgoal)
		end #end initialize
		def getdeltat
			return @SleepTime
		end #end getdeltat
		def wait
			sleep(@SleepTime)
		end #end wait
	end #end tick class
	
	# Physical Properties Class for values required for drag calcs
	class ClsPhysicalProperties
		@radius
		@mass
		@area
		@density
		@volume
		def initialize (r = 1)
			@radius = r
			@density = 7850 #density of steel in kg/m^3
			
			@area = 2.0 * Math::PI * @radius ** 2
			@volume = (4/3) * Math::PI * radius ** 3
			@mass = @volume * @density			
		end #end initialize
		def radius
			return @radius
		end
		def mass
			return @mass
		end
		def area
			return @area
		end
	end #end physical properties class
	
	# XY Class
	# This is used to hold the x and y values in the same place_x
	# used for velocity / place / acc
	# I am doing it this way to make the code look nicer
	class ClsXY
		@x
		@y
		def initialize (initx = 10, inity = 10)
			@x = initx
			@y = inity
		end #end initialize
		def setXY(newx = 0, newy = 0)
			@x = newx
			@y = newy
		end #end setXY
		def y
			return @y
		end #end y
		def x
			return @x
		end #end x
	end #end XY Class
	
	# Cannonball class
	class ClsCannonball 
		Drag_Cofficient = 0.47
		@place
		@props
		@blnDrag #currently does nothing here for a later version
		@vel
		@acc
		@deltat
		def initialize (x = 0, y = 0, init_vel = 100, init_angle = 45, r = 1, blnD = 0)
			@blnDrag = blnD
			@props = ClsPhysicalProperties.new(r)
			
			@place = ClsXY.new(x,y)
			
			# I'll assume they are on earth with no starting x acc
			@acc = ClsXY.new(0,-9.81)
			
			# Set deltat to a default value in case I forget to set it later
			@deltat = 1.0 / 60.0
			
			init_angle *= $Rad_Convert
			
			@vel = ClsXY.new(init_vel * Math.cos(init_angle), init_vel * Math.sin(init_angle))
			
			print "Cannonball made: at (#{@place.x},#{@place.y}) \n" if $Debug_Mode == 1
			print "Velocity: (#{@vel.x},#{@vel.y})\n" if $Debug_Mode == 1
		end #end initialize
		def setdeltat (value = 1)
			@deltat = value
		end #end setdeltat
		def update
			doDragUpdate if @blnDrag == 1
			
			_tempx = @place.x + (@vel.x * @deltat) + (0.5 * @acc.x * @deltat**2)
			_tempvx = @vel.x + @acc.x * @deltat
			_tempy = @place.y + (@vel.y * @deltat) + (0.5 * @acc.y * @deltat**2)
			_tempvy = @vel.y + @acc.y * @deltat
			
			_tempx = 0 if _tempx < 0
			_tempy = 0 if _tempy < 0
			
			@vel.setXY(_tempvx,_tempvy)
			@place.setXY(_tempx,_tempy)
			print "Cannonball position updated to: (#{_tempx.to_i},#{_tempy.to_i})\n"
		end #end update
		def getPlace_Y
			return @place.y
		end #end getPlayer_y
		def doDragUpdate
			_rho_air = 1.2754 #density of air in kg/m^3
			_flow_vel = Math.sqrt(@vel.x**2 + @vel.y**2)
			_drag_force = 0.5 * _rho_air * _flow_vel * Drag_Cofficient * @props.area
			_drag_acc = _drag_force / @props.mass
			
			_angle = Math.atan2(@vel.y, @vel.x)
			
			_temp_acc_x = @acc.x - _drag_acc * Math.cos(_angle)
			if @vel.y < 0 then
				_temp_acc_y = @acc.y + _drag_acc * Math.sin(_angle)
			else
				_temp_acc_y = @acc.y - _drag_acc * Math.sin(_angle)
			end
			
			@acc.setXY(_temp_acc_x, _temp_acc_y)
			end #ned doDragUpdate
	end #end class cannonball
	print "Classes called.\n" if $Debug_Mode == 1
} #end begin code
