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
# decimal_to_binary_board(576460752303423488)

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
# newstr = """00000000
# 00000000
# 00000000
# 00000000
# 00000000
# 00000000
# 00000000
# 00010000
# """
# board_to_decimal(newstr)

def decimal_to_binary(num: int) -> None:
    print(bin(int))

# example usage
# decimal_to_binary(12345)
