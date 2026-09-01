# Program to check whether an inputted number is a valid credit card number among the american express, mastercard or visa. The rules for checking is explained in the context below.

# CONTEXT

# luns algorithm says: 
# Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.

# Add the sum to the sum of the digits that weren’t multiplied by 2.

# If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

# AMEX: starts with 34, 37. 15 digits
# MASTERCARD: starts with 51, 52, 53, 54, 55. 16 digits
# VISA: starts with 4. 13 and 16 digits

def check_luhnsAlgo_validity(card_no: str) -> bool:
    reversed_card = card_no[::-1]
    total = 0
    
    for i, digit in enumerate(reversed_card):
        digit = int(digit)
        if i % 2 == 1:
            digit *= 2
        
        if digit > 9:
            digit -= 9
        
        total += digit
    
    return total % 10 == 0

def main():
    while True:
        input_card = input("Enter Card No: ")
        
        # keep asking till you get input thats actually a number
        if not(input_card.isdigit()):
            print("INVALID")
        else:
            break
        
    card_len = len(input_card)
    result = "INVALID"

    if check_luhnsAlgo_validity(input_card):
        match card_len:
            case 15:
                if input_card[:2] in {"34", "37"}:
                    result = "AMEX"

            case 13:
                if input_card[:1] == "4":
                    result = "VISA"

            case 16:
                if input_card[:2] in {"51", "52", "53", "54", "55"}:
                    result = "MASTERCARD"
                elif input_card[0] == "4":
                    result = "VISA"
    
    print(result)


if __name__ == "__main__":
    main()
