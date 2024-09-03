#!/bin/bash

# Define the output file
output_file="all_descriptions_sorted.txt"

# Empty the output file if it exists
> $output_file

# Loop through all .desc files, sorted by number, and append their content to the output file
for i in {1..20}
do
    file="${i}.desc"
    if [ -f "$file" ]; then
        cat "$file" >> $output_file
        echo "" >> $output_file  # Add a newline between files for readability
    fi
done

echo "All .desc files from 1 to 20 have been concatenated into $output_file."

