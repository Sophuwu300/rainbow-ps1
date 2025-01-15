# Rainbow Bash Prompt

## Installation
```bash
make
sudo install ./build/bashprompt /usr/local/bin/bashprompt
/usr/local/bin/bashprompt help
```
Ensure that the file is executable and that you use an absolute path to the file.
Copy the output to your .bashrc for automatic start up. **DO NOT** USE .bash_profile or .profile.

## Options:
Environment variables can be set to customize the prompt.

* `IPCOLOR=alt` IP address will be colored with an alternate color palette.
* `IPCOLOR=none` IP address and username will not be printed.



