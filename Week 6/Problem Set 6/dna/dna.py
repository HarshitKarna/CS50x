import csv
import sys


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Incorrect use of command line. Usage: {program name} {database.csv} {sequence_file.txt}")
        sys.exit(1)

    # Read database file into a variable
    with open(sys.argv[1], newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        database = list(reader)   # converts csv reader into a list of dicts containing all the rows
    
    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as sequence_file:
        sequence = sequence_file.read().strip()
        
    strs = reader.fieldnames[1:]   # excluding the name field/key

    # Find longest match of each STR in DNA sequence
    sequence_profile = {}
    
    for str_name in strs:
        # gets a dict containing "XXXX": max_no_of_repeatsof_XXXX, adds another "YYYY": ... after, creating a sequence profile
        sequence_profile[str_name] = longest_match(sequence, str_name)

    # TODO: Check database for matching profiles
    for person in database:
        match = True
        
        for str_name in strs:
            # making sure all str markers match, not just 1
            if int(person[str_name]) != sequence_profile[str_name]:
                match = False
                break
            
        if match:
            print(person["name"])
            return
    print("No match")


if __name__ == "__main__":
    main()
