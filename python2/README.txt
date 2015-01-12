open terminal and position yourself inside python2 folder

in terminal, run command: chmod u+x start.sh

To start program you need to pass 3 arguments:

- path to pattern file (FASTA format)
- path to text file (FASTA format)
- k

where k is the maximal number of allowed differences.

How to start the program:
./start.sh patternFilePath textFilePath k

Example of starting the program:
./start.sh pattern5.fa text8.fa 4
