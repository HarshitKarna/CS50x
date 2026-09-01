CARD_RULES = {
    "AMEX": {
        "lengths": {15},
        "prefixes": {"34", "37"}
    },
    "MASTERCARD": {
        "lengths": {16},
        "prefixes": {"51", "52", "53", "54", "55"}
    },
    "VISA": {
        "lengths": {13, 16},
        "prefixes": {"4"}
    }
}


def check_luhnsAlgo_validity(card_no: str) -> bool:
    reversed_card = card_no[::-1]
    total = 0

    for index, digit in enumerate(reversed_card):
        digit = int(digit)

        if index % 2 == 1:
            digit *= 2

            if digit > 9:
                digit -= 9

        total += digit

    return total % 10 == 0


def matches_prefix(card_no: str, prefixes: set[str]) -> bool:
    return any(card_no.startswith(prefix) for prefix in prefixes)


def identify_card(card_no: str) -> str:
    if not check_luhnsAlgo_validity(card_no):
        return "INVALID"

    for card_type, rules in CARD_RULES.items():
        if len(card_no) not in rules["lengths"]:
            continue

        if matches_prefix(card_no, rules["prefixes"]):
            return card_type

    return "INVALID"


def main():
    card_no = input("Enter Card No: ")

    if not card_no.isdigit():
        print("INVALID")
        return

    print(identify_card(card_no))


if __name__ == "__main__":
    main()
