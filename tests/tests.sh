#!/bin/sh

pass=0
failed=0

test()
{
	LD_PRELOAD=./libmalloc.so "$@" >/dev/null 2>&1
	err=$?
	echo "$@"
	if [ $err -eq 0 ]; then
		pass=$((pass+1))
	else
		failed=$((failed+1))
		echo $@
	fi
}
test factor 10
test factor 10 20
test factor 10 20 30 40 50 60 70 80 90
test ip a
test ls
test ls -a
test ls -l
test ls -la
test od libmalloc.so
test find .
test find ~
test git status
test git log 
test git log --oneline
test man man
test man 3 malloc
test man bash
test man git
test man 2 signal
test less Makefile
test tree
test tree -a
test cat Makefile
test cat -n Makefile
test cat -e Makefile
test cat ~/.bashrc
test printf "Hello World"
test printf "%s" "Hello World"
test printf "%d" 10
test printf "%c" 'a'
echo "pass: ${pass} failed: ${failed}"
