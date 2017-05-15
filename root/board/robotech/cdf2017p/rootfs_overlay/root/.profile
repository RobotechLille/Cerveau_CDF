#/bin/bash

alias la='ls -al'
alias ll='ls -l'
alias cp="cp -i"
alias mv="mv -i"
alias free='free -m'
alias df='df -h'

export PS1="\[\e]2;\u@\h \w\a\]\[\e[0;37m\][\[\e[0;31m\]\u\[\e[0;37m\]@\[\e[0;34m\]\h \[\e[0;36m\]\W\[\e[0;37m\]]\$\[\e[0m\] "
export PS2="> "
export PS3="+ "
export PS4="- "
