# implement a program that calculates the approximate grade level needed to comprehend some text. Your program should print as output “Grade X” where “X” is the grade level computed, rounded to the nearest integer. If the grade level is 16 or higher (equivalent to or greater than a senior undergraduate reading level), your program should output “Grade 16+” instead of giving the exact index number. If the grade level is less than 1, your program should output “Before Grade 1”.

# The Coleman-Liau index of a text is designed to output that (U.S.) grade level that is needed to understand some text. The formula is:

# index = 0.0588 * L - 0.296 * S - 15.8

# where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.

# NOTE: it is also assumed that there aren't any grammatical errors in the program

def text_details(text: str) -> tuple[int, int, int]:
    sentence_count = sum(text.count(c) for c in ".?!")
    word_count = len(text.split())   # .split() returns list of substrings, i.e. a list of words separed by whitespace
    letter_count = sum(char.isalpha() for char in text)

    return sentence_count, word_count, letter_count
    

def main():
    input_text = input("Text: ")
    
    sentences, words, letters = text_details(input_text)
    # handle 0/0 case
    if words == 0:
        print("Before Grade 1")
        return

    # index calculation
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    print(f"Index: {index}")
    
    # grade check
    index_rounded = round(index)
    
    if index_rounded >= 16:
        print("Grade 16+")
    elif index_rounded < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index_rounded}")


if __name__ == "__main__":
    main()
    