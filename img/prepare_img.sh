for img in *.png; do
    echo "converting $img"
    convert $img $(basename $img '.png')'.xpm'
done
