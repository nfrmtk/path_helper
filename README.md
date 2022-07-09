# path_helper
CLI project that searches for duplicates in your PATH

You can download application by [link](https://drive.google.com/uc?export=download&id=1_rxTL4eo8fquA2OJSSmqoWzH1N0-2VCa) 

Usage:
__pathh %NAME OF DESIRED APPLICATION HERE%

e.g.:
```
C:\Users\HalseLimon>__pathh code
C:\Users\HalseLimon\AppData\Local\Programs\Microsoft VS Code\bin\code.cmd; version is -> 1.69.0

C:\Users\HalseLimon>__pathh gcc
C:\Users\HalseLimon\scoop\apps\gcc\current\bin\gcc.exe; version is -> 11.2.0

```

Note, that if you end application with SIGINT (Ctrl+C) next use of application might be incorrect.

To avoid such situation, after killing app with SIGINT, delete data.txt file, located in the same folder with __path.exe.
