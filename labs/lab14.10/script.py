with open("main.py", "r") as file:
	data = file.read().replace(" . ", ".")
	out = open("out.py", "w")
	out.write(data)
	out.close()
