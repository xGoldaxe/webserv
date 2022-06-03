# Testing request parsing

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
1. Output the data that will be send to the program on stdin.
2. Output the expected result in file provided in first argument.

__Understand the result__

First there is the first_line, with method, path and version.
Then all the headers parser, and finally the raw body.
If the request is not valid, at any point it will print only **error 400, bad request\n**.
( to see how data is print, look at print_and_store function ).


# some notes
They may be some more data after end of body, close or delete all the additional data is requires