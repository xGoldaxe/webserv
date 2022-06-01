## Expected results:

__For the nginx server:__
```
xxx.xxx.xxx.xxx - - [30/May/2022:18:07:32 +0000] "GET / HTTP/1.1" 200 34 "-" "Tester"
xxx.xxx.xxx.xxx - - [30/May/2022:18:07:38 +0000] "POST / HTTP/1.1" 405 157 "-" "Tester"
xxx.xxx.xxx.xxx - - [30/May/2022:18:07:38 +0000] "POST / HTTP/1.1" 405 157 "-" "Tester"
xxx.xxx.xxx.xxx - - [30/May/2022:18:07:39 +0000] "ay=Hi&to=Mom" 400 157 "-" "-"
xxx.xxx.xxx.xxx - - [30/May/2022:18:07:45 +0000] "POST / HTTP/1.1" 405 157 "-" "Tester"
```