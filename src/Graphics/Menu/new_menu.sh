#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <menu name>"
  exit 1
fi

cd $(dirname $0)

sed s/\{NAME\}/$1/g skeleton/menu_skeleton.cpp > "$1"Menu.cpp.tmp
sed s/\{NAME\}/$1/g skeleton/menu_skeleton.h > "$1"Menu.h.tmp

upperName=$(echo "$1" | sed -r 's/([a-z0-9])([A-Z])/\1_\L\2/g')
upperName=$(echo $upperName | awk '{print toupper($0)}')

sed s/\{NAME_UPPER}/$upperName/g "$1"Menu.cpp.tmp > "$1"Menu.cpp
sed s/\{NAME_UPPER}/$upperName/g "$1"Menu.h.tmp > "$1"Menu.h

rm "$1"Menu.cpp.tmp
rm "$1"Menu.h.tmp

echo "Don't forget to add the new menu type!"
