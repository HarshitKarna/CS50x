# Same, but prints:
#        #  #
#       ##  ##
#      ###  ###
#     ####  ####

while True:
    try:
        height = int(input("Enter the height: "))
        if height > 0:
            break
    except ValueError:
        pass

for row in range(1, height+1):
    print(" " * (height - row) + "#" * row + "  " + "#" * row)
