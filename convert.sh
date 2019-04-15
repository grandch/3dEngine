for oldFileName in *.tga; do
    [ -f "$oldFileName" ] || break         #break out if no .tga files found
    newFileName=${oldFileName//.tga/.png}
    convert $oldFileName $newFileName
    rm $oldFileName
    echo "Converted $oldFileName to $newFileName"
done