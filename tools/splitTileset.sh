# Usage:
#
# sh crop.sh <tileset_image_file> <tileset_image_width> <tileset_image_height> <tile_size_X> <tile_size_y>
#
# Example:
#   sh crop.sh tileset01.png 128 192 32 32
#
# - Will generate 24 tiles of 32x32 named tile1.png, tile2.png, ..., tile24.png
#

# Your tileset file. I've tested with a png file.
origin=$1

# Control variable. Used to name each tile.
counter=0

# Location of the tool that we are using to extract the files. I had to create a shortcut of the tool in the same folder as the script.
program=convert

# The size of the tile (32x32)
tile_size_x=$4
tile_size_y=$5

# Number of rows (horizontal) in the tileset.
rows=$2
let rows/=tile_size_x

# Number of columns (vertical) in the tileset.
columns=$3
let columns/=tile_size_y

# Tile name prefix.
prefix=tile

# Tile name sufix.
sufix=.png

echo Extracting $((rows * $columns)) tiles...

for i in $(seq 0 $((columns - 1))); do

    for j in $(seq 0 $((rows - 1))); do

        # Calculate next cut offset.
        offset_y=$((i * tile_size_y))
        offset_x=$((j * tile_size_x))

        # Update naming variable.
        counter=$((counter + 1))

        tile_name=$prefix$counter$sufix

        echo $program -extract $tile_size"x"$tile_size"+"$offset_x"+"$offset_y $origin $tile_name
        $program -extract $tile_size_x"x"$tile_size_y"+"$offset_x"+"$offset_y $origin $tile_name
    done
done
echo Done!
