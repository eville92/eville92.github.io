# Hawks - Scanner & Recursive Descent Parser

## Description
A scanner and a recursive descent parser that will take as input a program written in the provided language. In addition to parsing the input programs, the program generates the proper errors when encountered.

## Instructions

1. Must have ```hawks.c``` and ```input.txt``` in the same folder.
2. Compile ```hawks.c``` via: 
   ```gcc hawks.c -o hawks```
3. Run ```hawks``` via:
   ```./hawks```
4. To test different cases, open input.txt and paste any test case from the test-cases folder. Each test case contains a script written in the custom language. Running the program will parse the contents of input.txt and display results or errors.

## Project Structure
```
hawks/
│
├── hawks.c
├── input.txt
├── test-cases/
│   ├── script1.txt
│   ├── script2.txt
│   ├── script3.txt
│   ├── script4.txt
│   ├── script5.txt
│   ├── script6.txt
│   ├── script11.txt
│   ├── script12.txt
│   ├── script13.txt
│   ├── script14.txt
│   └── script15.txt
└── README.md
```

## Notes
- The parser follows a recursive descent design based on the grammar provided in the assignment. 
- Error messages are generated when invalid tokens or grammar violations are detected.
- You can modify ```input.txt```freely to test your own scripts. 

## Optional Links
