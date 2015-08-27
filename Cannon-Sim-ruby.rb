#!/usr/bin/ruby
# TODO Drag Mode
# TODO Class of location values?

# Make some global variables for later use.
$Program_version = "0.9.0"
$Debug_Mode = 1 # Enables Debug mode which prints more messages to the console.
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

print "\n\n"

# Start asking user for values

# Changing the starting position of the ball is currentlty disabled.
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
=begin
print"\nWould you like to enable drag? (1 for yes, 0 for no): "
_temp = gets
_enableddrag = _temp.to_i
=end

print "\n"
Tick = ClsTick.new(10)
Cannonball = ClsCannonball.new(_xplace, _yplace, _vinit, _angleinit)

Cannonball.setdeltat(Tick.getdeltat)

while Cannonball.getPlace_Y > 0 do
	Cannonball.update
	Tick.wait
end #end while

#end of code.

BEGIN {
	# declare all the classes that I will need

	# tick class
	class ClsTick
		@FPSgoal
		@SleepTime
		def initialize (value = 60)
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

	# Cannonball class
	class ClsCannonball 
		@place_x
		@place_y
		@blnDrag #currently does nothing here for a later version
		@vel_x
		@vel_y
		@acc_x
		@acc_y
		@deltat
		def initialize (x = 0, y = 0, init_vel = 100, init_angle = 45)
			@blnDrag = 0
			
			@place_x = x
			@place_y = y
			
			# I'll assume they are on earth with no starting x acc
			@acc_x = 0
			@acc_y = -9.81
			
			# Set deltat to a default value in case I forget to set it later
			@deltat = 1.0 / 60.0
			
			init_angle *= $Rad_Convert
			
			@vel_x = init_vel * Math.cos(init_angle)
			@vel_y = init_vel * Math.sin(init_angle)
			
			print "Cannonball made: at (#{@place_x},#{@place_y}) \n" if $Debug_Mode == 1
			print "Velocity: (#{@vel_x},#{@vel_y})\n" if $Debug_Mode == 1
		end #end initialize
		def setdeltat (value = 1)
			@deltat = value
		end #end setdeltat
		def update
			@place_x += (@vel_x * @deltat) + (0.5 * @acc_x * @deltat**2)
			@vel_x += @acc_x * @deltat
			
			@place_y += (@vel_y * @deltat) + (0.5 * @acc_y * @deltat**2)
			@vel_y += @acc_y * @deltat
			
			@place_x = 0 if @place_x < 0
			@place_y = 0 if @place_y < 0
			
			print "Cannonball position updated to: (#{@place_x.to_i},#{@place_y.to_i})\n"
		end #end update
		def getPlace_Y
			return @place_y
		end #end getPlayer_y
	end #end class cannonball
	print "Classes called.\n" if $Debug_Mode == 1
} #end begin code
