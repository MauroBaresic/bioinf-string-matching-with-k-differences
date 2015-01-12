INSTALLATION:
- no installation is required
- only prerequisite is that Java Runtime Environment is installed (https://java.com/en/download/)

USAGE:
- program uses 3 arguments provided by the user (via command line)
    - path to file (FASTA format) containing text
    - path to file (FASTA format) containing pattern
    - number of allowed differences - k
    
    - in each of the provided FASTA files only FIRST sequence is taken into account (there can be more than one in file
    but they are discarded)
 
- one example usage would be as follows:
    java -jar kdiff.jar text.fa pattern.fa 5
    
OUTPUT:
- output format is as follows:
    - startIndexInText endIndexInText numberOfDifferences
- end index is INCLUSIVE
    
- for example:
1 5 0   --> meaning pattern of length 5 was matched with substring from text starting at index 1 with 0 differences
2 6 1

- for each starting index in text only FIRST BEST match is printed out
    