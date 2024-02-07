#!/bin/zsh

if [ $# -lt 2 ];
 then
    echo "Usage: $0 <input_file> <output_file>"
    exit 1
fi

filename=$1
output_file=$2

if [ ! -f "$filename" ]
 then
    echo "File $filename does not exist."
    exit 7
fi

content=$(cat "$filename")

capitalized_content=$(echo "$content" | perl -pe 'BEGIN{undef $/;} s/([.!?]\s+|^)(\w)/$1\u$2/gsm')

echo "$capitalized_content" > "$output_file"

echo "Capitalized content has been written to $output_file"
