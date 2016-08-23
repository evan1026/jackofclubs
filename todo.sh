files=$(cd $(dirname $0); grep -R TODO)

set -f              # turn off globbing
IFS='
'                   # split at newlines only'
for file in $files; do
  if [[ "$file" != *"$(basename $0)"* && "$file" != *.git/* ]]; then
    echo $file
  fi
done

unset IFS
set +f
