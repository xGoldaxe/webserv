# Testing only request parsing, from stding

In this section, we will read from STDIN and consider it like a socket.
Used for tests, we just have to provide a file in entry and read it, or
manually file information.
	So its easy to manage timing and parsing.

## Usage:
__Run all tests__
```
bash runtest.sh
```
__Run 1 test in debug mode__
```
bash runtest.sh script_name
```
Where script_name is the path of the file, started from **./tests**.

__Write additional tests__
In folder **./tests**, add your own script.
Your script must follow 2 rules :
1. Must output on stdin the data that will be send to the program.
2. Output in file provide in first argument, the expected result.

__Understand the result__
First there is the first_line, with method, path and version.
Then all the headers parser, and finally the raw body.
If the request is not valid, at any point it will print only **error 400, bad request\n**.
( to see how data is print, look at print_and_store function ).