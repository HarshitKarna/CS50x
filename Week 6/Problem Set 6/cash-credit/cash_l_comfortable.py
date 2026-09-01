# Suppose you work at a store and a customer gives you $1.00 (100 cents) for candy that costs $0.50 (50 cents). You’ll need to pay them their “change,” the amount leftover after paying for the cost of the candy. When making change, odds are you want to minimize the number of coins you’re dispensing for each customer, lest you run out (or annoy the customer!). Implement a program in C that prints the minimum coins(per type) needed to make the given amount of change, in cents. (1, 5, 10, 20 cent coins)

while True:
    try:
        change_owed = float(input("Change (in cents): "))
        if (change_owed > 0):
            break
    except ValueError:
        pass

cent_coins = [20, 10, 5, 1]

for coin_val in cent_coins:
    print(f"No of {coin_val} owed: {int(change_owed / coin_val)}")
    change_owed %= coin_val
