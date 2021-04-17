# feed it in as an int
def decimal_to_binary_board(num: int) -> None:
    string = ""
    
    count = 0
    while count < 64:
        
        if count % 8 == 0:
            string += "\n"
        
        if num % 2 == 0:
            string += "0"
        else:
            string += "1"
        num = num // 2
        
        count += 1
        
    
    print(string + "\n")
    return None
    
# example usage
# print("occupied")
# decimal_to_binary_board(10735178551994414071)
# decimal_to_binary_board(10519005769880313856)
# decimal_to_binary_board(288230376152028151)
# decimal_to_binary_board(288230376151711744)
decimal_to_binary_board(268435456)
decimal_to_binary_board(524288)
decimal_to_binary_board(578721382704613384)



def board_to_decimal(board: str) -> None:
    board = board.replace("\n", "")
    
    count = 0
    num = 0
    while board:
        if board[-1] == "1":
            num += 2 ** count
        count += 1
        board = board[:-1]
    
    print(num)
    return None
    
# example usage   
# example of input board string
newstr = """00000000
00000000
00000000
00000000
00000000
00000000
00000000
00010000
"""
board_to_decimal(newstr)

def decimal_to_64binary(num) -> None:
    string = ""
    
    count = 0
    while count < 64:
        if num % 2 == 0:
            string =  "0" + string
        else:
            string = "1" + string
        num = num // 2
        
        count += 1
        
    
    print(string)
    return None

# example usage
# decimal_to_64binary(18446462598732906495)
# decimal_to_64binary(18446462598732906496)
# decimal_to_64binary(140737488420863)
# decimal_to_64binary(72340172838076673)
# print(bin(576179277326778368))

def binary_to_decimal(numstr) -> None:
    count = 0
    num = 0
    while numstr:
        if numstr[-1] == "1":
            num += 2 ** count
        count += 1
        numstr = numstr[:-1]
    
    print(num)
    return None

# must feed argument as a string
# binary_to_decimal("0000000000000000100000000000000000000000000000001111111111100000")

import itertools

tst = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1]

print(test.length())

a = list(itertools.permutations(tst))

print(a.length())
