some_variable = 7 * 6

-- Values for initialization
config = {
	title = "Legends Engine",
	fullscreen = false,
	resolution = {
		width = 800,
		height = 600,
	},
}

-- Factorial function
function factorial(n)
	if n == 1 then
		return 1
	end
	return n * factorial(n - 1)
end

print("Factorial of 5 is " .. factorial(5))

print("The cube of the number 3 is equal to " .. cube(3))
