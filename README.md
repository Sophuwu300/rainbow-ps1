# Rainbow Bash Prompt
An extension for bash that generates a rainbow-colored PS1 with IP address shown as 4 blocks of color.

## Install with apt (ubuntu/debian)
### Add the repository
You can add the repository by running the following command:
```bash
sudo wget -O /usr/share/keyrings/soph-deb.gpg "https://cdn.sophuwu.com/deb/key.gpg" && echo 'deb [signed-by=/usr/share/keyrings/soph-deb.gpg] https://cdn.sophuwu.com/deb/ sophuwu main' | sudo tee /etc/apt/sources.list.d/sophdeb.list
```
This script will do the same thing:
```bash
curl https://cdn.sophuwu.com/deb/addrepo.sh | sudo sh
```
### Install the "bash-rb-ps1" package   
```bash
sudo apt update
sudo apt install bash-rb-ps1
```
## Run for the first time
This will only work if you are using bash with xterm-like escape sequences. You can enable the generator
by running:
```bash
eval $(bash-rb-ps1 help)
```
If you experience an error, or want to disable the prompt, you can run:
```bash
unset PROMPT_COMMAND
PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
```
To see help and other information about the command, use:
```bash
man bash-rb-ps1.1
```
## Compile from source
```bash
make
sudo install ./build/bashprompt /usr/local/bin/bash-rb-ps1
sudo install ./releaser/bash-rb-ps1.1 /usr/local/share/man/man1/bash-rb-ps1.1
```

## Options:
Environment variables can be set to customize the prompt.

* `IPCOLOR=alt` IP address will be colored with an alternate color palette.
* `IPCOLOR=none` IP address and username will not be printed.

## Preview

<img src="https://cdn.sophuwu.com/img/bash-rb-ps1.png" alt="Rainbow Bash Prompt" style="display: block; margin: auto; height: 45lh;" />

